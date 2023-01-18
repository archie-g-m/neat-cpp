#include "config_parser.h"
#include "genome.h"

#include <gtest/gtest.h>

TEST(GENOMECONFIG, IncorrectCategoryTest){
    ConfigParser* config = new ConfigParser("NoDefaultGenome.cfg");
    ASSERT_THROW(new GenomeConfig(config), std::invalid_argument);
}

TEST(GENOMECONFIG, MissingFieldTest){
    ConfigParser* config = new ConfigParser("MissingField.cfg");
    ASSERT_THROW(new GenomeConfig(config), std::invalid_argument);
}

TEST(GENOMECONFIG, CorrectConversionTest){
    ConfigParser* config = new ConfigParser("ValidConfigDirect.cfg");
    GenomeConfig* genome_config = new GenomeConfig(config);

    ASSERT_EQ(genome_config->activation_default, "relu");
    ASSERT_FLOAT_EQ(genome_config->activation_mutate_rate, 1.0);
    std::set<std::string> act_opt;
    act_opt.insert("relu");
    ASSERT_EQ(genome_config->activation_options, act_opt);

    ASSERT_EQ(genome_config->aggregation_default, "sum");
    ASSERT_FLOAT_EQ(genome_config->aggregation_mutate_rate, 0.0);
    std::set<std::string> agg_opt;
    agg_opt.insert("sum");
    ASSERT_EQ(genome_config->aggregation_options, agg_opt);

    ASSERT_FLOAT_EQ(genome_config->bias_init_mean, 3.0);
    ASSERT_FLOAT_EQ(genome_config->bias_init_stdev, 1.0);
    ASSERT_EQ(genome_config->bias_init_type, "gaussian");
    ASSERT_FLOAT_EQ(genome_config->bias_max_value, 30.0);
    ASSERT_FLOAT_EQ(genome_config->bias_min_value, -30.0);
    ASSERT_FLOAT_EQ(genome_config->bias_mutate_power, 0.5);
    ASSERT_FLOAT_EQ(genome_config->bias_mutate_rate, 0.7);
    ASSERT_FLOAT_EQ(genome_config->bias_replace_rate, 0.1);

    ASSERT_FLOAT_EQ(genome_config->compatibility_disjoint_coefficient, 1.0);
    ASSERT_FLOAT_EQ(genome_config->compatibility_weight_coefficient, 0.5);

    ASSERT_FLOAT_EQ(genome_config->conn_add_prob, 0.5);
    ASSERT_FLOAT_EQ(genome_config->conn_delete_prob, 0.5);

    ASSERT_EQ(genome_config->enabled_default, true);
    ASSERT_FLOAT_EQ(genome_config->enabled_mutate_rate, 0.01);
    ASSERT_FLOAT_EQ(genome_config->enabled_rate_to_true_add, 0);
    ASSERT_FLOAT_EQ(genome_config->enabled_rate_to_false_add, 0);

    ASSERT_EQ(genome_config->initial_connection, "full_direct");

    ASSERT_FLOAT_EQ(genome_config->node_add_prob, 0.2);
    ASSERT_FLOAT_EQ(genome_config->node_delete_prob, 0.2);

    ASSERT_EQ(genome_config->num_hidden, 10);
    ASSERT_EQ(genome_config->num_inputs, 2);
    ASSERT_EQ(genome_config->num_outputs, 4);

    ASSERT_FLOAT_EQ(genome_config->response_init_mean, 1.0);
    ASSERT_FLOAT_EQ(genome_config->response_init_stdev, 0.0);
    ASSERT_EQ(genome_config->response_init_type, "gaussian");
    ASSERT_FLOAT_EQ(genome_config->response_max_value, 30.0);
    ASSERT_FLOAT_EQ(genome_config->response_min_value, -30.0);
    ASSERT_FLOAT_EQ(genome_config->response_mutate_power, 0.0);
    ASSERT_FLOAT_EQ(genome_config->response_mutate_rate, 0.0);
    ASSERT_FLOAT_EQ(genome_config->response_replace_rate, 0.0);

    ASSERT_FLOAT_EQ(genome_config->weight_init_mean, 0.0);
    ASSERT_FLOAT_EQ(genome_config->weight_init_stdev, 1.0);
    ASSERT_EQ(genome_config->weight_init_type, "gaussian");
    ASSERT_FLOAT_EQ(genome_config->weight_max_value, 30);
    ASSERT_FLOAT_EQ(genome_config->weight_min_value, -30);
    ASSERT_FLOAT_EQ(genome_config->weight_mutate_power, 0.5);
    ASSERT_FLOAT_EQ(genome_config->weight_mutate_rate, 0.8);
    ASSERT_FLOAT_EQ(genome_config->weight_replace_rate, 0.1);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}