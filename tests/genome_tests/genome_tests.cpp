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


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}