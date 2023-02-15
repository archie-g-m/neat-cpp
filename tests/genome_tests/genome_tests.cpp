#include "config_parser.h"
#include "genome.h"

#include <gtest/gtest.h>

TEST(GENOMETEST, ConstructionTestFullDirect)
{
    ConfigParser *config = new ConfigParser("ValidConfigDirect.cfg");
    GenomeConfig *genome_config = new GenomeConfig(config);

    Genome *genome = new Genome(1, genome_config);

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
    ConfigParser *config = new ConfigParser("ValidConfigIndirect.cfg");
    GenomeConfig *genome_config = new GenomeConfig(config);

    Genome *genome = new Genome(2, genome_config);

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
    ConfigParser *config = new ConfigParser("ValidConfigDirectNoHidden.cfg");
    GenomeConfig *genome_config = new GenomeConfig(config);

    Genome *genome = new Genome(3, genome_config);

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
    ConfigParser *config = new ConfigParser("ValidConfigIndirectNoHidden.cfg");
    GenomeConfig *genome_config = new GenomeConfig(config);

    Genome *genome = new Genome(4, genome_config);

    // Make sure correct number of nodes were made
    ASSERT_EQ(genome->get_num_inputs(), 2);
    ASSERT_EQ(genome->get_num_outputs(), 4);
    ASSERT_EQ(genome->get_num_hidden(), 0);
    ASSERT_EQ(genome->get_num_nodes(), 6);

    // Make sure correct number of connections were made
    int calc_num_nodes = 2 * 4;
    ASSERT_EQ(genome->get_num_connections(), calc_num_nodes);
}
TEST(GENOMETEST, ForwardRepeatabilityTest){
    ConfigParser *config = new ConfigParser("ValidConfigDirect.cfg");
    GenomeConfig *genome_config = new GenomeConfig(config);

    Genome *genome = new Genome(1, genome_config);    
    std::vector<float> test_in ;
    for(int i=1; i<=genome_config->num_inputs; i++){test_in.push_back((float)i);}
    std::vector<std::vector<float>> outs;
    for(int i=0; i<10; i++){
        genome->activate();
        std::vector<float> out = genome->forward(test_in);
        outs.push_back(out);
    }
    for(int i=1; i<outs.size(); i++){
        for(int j=0; j<outs[i].size(); j++){
            ASSERT_EQ(outs[i-1][j], outs[i][j]);
        }
    }
}
TEST(GENOMETEST, MutateAddNodeTest)
{
    ConfigParser *config = new ConfigParser("MutateNodeAdd.cfg");
    GenomeConfig *genome_config = new GenomeConfig(config);
    Genome *genome = new Genome(5, genome_config);

    int before_nodes = genome->get_num_nodes();
    int before_conns = genome->get_num_connections();
    genome->mutate();
    int after_nodes = genome->get_num_nodes();
    int after_conns = genome->get_num_connections();

    ASSERT_EQ(after_nodes-before_nodes, 1);
    ASSERT_EQ(after_conns-before_conns, 2);
}
TEST(GENOMETEST, MutateDeleteNodeTest)
{
    ConfigParser *config = new ConfigParser("MutateNodeDel.cfg");
    GenomeConfig *genome_config = new GenomeConfig(config);
    Genome *genome = new Genome(5, genome_config);

    int before_nodes = genome->get_num_nodes();
    genome->mutate();
    int after_nodes = genome->get_num_nodes();

    ASSERT_EQ(before_nodes-after_nodes, 1);
}
TEST(GENOMETEST, MutateDeleteConnTest)
{
    ConfigParser *config = new ConfigParser("MutateConnDel.cfg");
    GenomeConfig *genome_config = new GenomeConfig(config);
    Genome *genome = new Genome(5, genome_config);

    int before_conns = genome->get_num_connections();
    genome->mutate();
    int after_conns = genome->get_num_connections();

    ASSERT_EQ(before_conns-after_conns, 1);
}
TEST(GENOMETEST, MutateAddConnTest)
{
    ConfigParser *config = new ConfigParser("MutateConnAdd.cfg");
    GenomeConfig *genome_config = new GenomeConfig(config);
    Genome *genome = new Genome(5, genome_config);
    int before_conns = genome->get_num_connections();
    genome->mutate();
    int after_conns = genome->get_num_connections();

    ASSERT_EQ(after_conns-before_conns, 1);
}
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}