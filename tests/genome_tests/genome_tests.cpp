#include "config_parser.h"
#include "genome.h"

#include <gtest/gtest.h>

TEST(GENOMETEST, ConstructionTestFullDirect)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfigDirect.cfg");

    Genome *genome = new Genome(1, config);

    // Make sure correct number of nodes were made
    ASSERT_EQ(genome->get_num_inputs(), 2);
    ASSERT_EQ(genome->get_num_outputs(), 4);
    ASSERT_EQ(genome->get_num_hidden(), 10);
    ASSERT_EQ(genome->get_num_nodes(), 16);

    // Make sure correct number of connections were made
    int calc_num_nodes = (2 * (4 + 10)) + (4 * 10);
    ASSERT_EQ(genome->get_num_connections(), calc_num_nodes);
}
TEST(GENOMETEST, ConstructionTestFullIndirect)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfigIndirect.cfg");

    Genome *genome = new Genome(2, config);

    // Make sure correct number of nodes were made
    ASSERT_EQ(genome->get_num_inputs(), 2);
    ASSERT_EQ(genome->get_num_outputs(), 4);
    ASSERT_EQ(genome->get_num_hidden(), 10);
    ASSERT_EQ(genome->get_num_nodes(), 16);

    // Make sure correct number of connections were made
    int calc_num_nodes = (2 * 4) + (4 * 10);
    ASSERT_EQ(genome->get_num_connections(), calc_num_nodes);
}
TEST(GENOMETEST, ConstructionTestFullDirectNoHidden)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfigDirectNoHidden.cfg");

    Genome *genome = new Genome(3, config);

    // Make sure correct number of nodes were made
    ASSERT_EQ(genome->get_num_inputs(), 2);
    ASSERT_EQ(genome->get_num_outputs(), 4);
    ASSERT_EQ(genome->get_num_hidden(), 0);
    ASSERT_EQ(genome->get_num_nodes(), 6);

    // Make sure correct number of connections were made
    int calc_num_nodes = 2 * 4;
    ASSERT_EQ(genome->get_num_connections(), calc_num_nodes);
}
TEST(GENOMETEST, ConstructionTestFullIndirectNoHidden)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfigIndirectNoHidden.cfg");

    Genome *genome = new Genome(4, config);

    // Make sure correct number of nodes were made
    ASSERT_EQ(genome->get_num_inputs(), 2);
    ASSERT_EQ(genome->get_num_outputs(), 4);
    ASSERT_EQ(genome->get_num_hidden(), 0);
    ASSERT_EQ(genome->get_num_nodes(), 6);

    // Make sure correct number of connections were made
    int calc_num_nodes = 2 * 4;
    ASSERT_EQ(genome->get_num_connections(), calc_num_nodes);
}
TEST(GENOMETEST, ForwardRepeatabilityTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfigDirect.cfg");
    GenomeConfig_ptr genome_config = std::make_shared<GenomeConfig>(config);

    Genome *genome = new Genome(1, config);
    std::vector<float> test_in;
    for (int i = 1; i <= genome_config->num_inputs; i++)
    {
        test_in.push_back((float)i);
    }
    std::vector<std::vector<float>> outs;
    for (int i = 0; i < 10; i++)
    {
        genome->activate();
        std::vector<float> out = genome->forward(test_in);
        outs.push_back(out);
    }
    for (int i = 1; i < outs.size(); i++)
    {
        for (int j = 0; j < outs[i].size(); j++)
        {
            ASSERT_EQ(outs[i - 1][j], outs[i][j]);
        }
    }
}
TEST(GENOMETEST, MutateAddNodeTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/MutateNodeAdd.cfg");
    Genome *genome = new Genome(5, config);

    int before_nodes = genome->get_num_nodes();
    int before_conns = genome->get_num_connections();
    genome->mutate();
    int after_nodes = genome->get_num_nodes();
    int after_conns = genome->get_num_connections();

    ASSERT_EQ(after_nodes - before_nodes, 1);
    ASSERT_EQ(after_conns - before_conns, 2);
}
TEST(GENOMETEST, MutateDeleteNodeTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/MutateNodeDel.cfg");
    Genome *genome = new Genome(5, config);

    int before_nodes = genome->get_num_nodes();
    genome->mutate();
    int after_nodes = genome->get_num_nodes();

    ASSERT_EQ(before_nodes - after_nodes, 1);
}
TEST(GENOMETEST, MutateDeleteConnTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/MutateConnDel.cfg");
    Genome *genome = new Genome(5, config);

    int before_conns = genome->get_num_connections();
    genome->mutate();
    int after_conns = genome->get_num_connections();

    ASSERT_EQ(before_conns - after_conns, 1);
}
TEST(GENOMETEST, MutateAddConnTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/MutateConnAdd.cfg");
    Genome *genome = new Genome(5, config);
    int before_conns = genome->get_num_connections();
    genome->mutate();
    int after_conns = genome->get_num_connections();

    ASSERT_EQ(after_conns - before_conns, 1);
}

TEST(GENOMETEST, ConstructionTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/MyConfig.cfg");

    int gid = 0;
    Genome_ptr genome = std::make_shared<Genome>(gid, config);

    std::cout << "Key: " << genome->key << std::endl;
    std::cout << "Fitness: " << genome->fitness << std::endl;
    std::cout << "Nodes:" << std::endl;
    for (std::pair<const int, NodeGene_ptr> &ngit : genome->nodes)
    {
        const int nid = ngit.first;
        NodeGene_ptr n = ngit.second;
        std::cout << "\t" << nid << " DefaultNodeGene(key=" << n->key << ", bias=" << n->get_attribute("bias")->get_string_value() << ", response=" << n->get_attribute("response")->get_string_value() << ", activation=" << n->get_attribute("activation")->get_string_value() << ", aggregation=" << n->get_attribute("aggregation")->get_string_value()
                  << ")" << std::endl;
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}