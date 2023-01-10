#ifndef GENOME_H
#define GENOME_H

#include <map>
#include <string>
#include <vector>
#include "genes.h"
#include "config_parser.h"

class GenomeConfig
{
public:
    // Distance Parameters
    float compatibility_disjoint_coefficient;
    float compatibility_weight_coefficient;

    // Mutate Parameters
    float conn_add_prob;
    float conn_delete_prob;

    float node_add_prob;
    float node_delete_prob;

    // Node Parameters
    int num_inputs;
    int num_outputs;
    int num_hidden;

    float bias_init_mean;
    float bias_init_stdev;
    std::string bias_init_type;
    float bias_max_value;
    float bias_min_value;
    float bias_mutate_power;
    float bias_mutate_rate;
    float bias_replace_rate;

    float response_init_mean;
    float response_init_stdev;
    std::string response_init_type;
    float response_max_value;
    float response_min_value;
    float response_mutate_power;
    float response_mutate_rate;
    float response_replace_rate;

    // Connection Parameters
    std::string initial_connection;

    std::string activation_default;
    float activation_mutate_rate;
    std::string activation_options;

    std::string aggregation_default;
    float aggregation_mutate_rate;
    std::string aggregation_options;

    bool enabled_default;
    float enabled_mutate_rate;
    float enabled_rate_to_true_add;
    float enabled_rate_to_false_add;

    float weight_init_mean;
    float weight_init_stdev;
    std::string weight_init_type;
    float weight_max_value;
    float weight_min_value;
    float weight_mutate_power;
    float weight_mutate_rate;
    float weight_replace_rate;

    //Mutation Parameters

    GenomeConfig(ConfigParser *_config);

private:
    bool to_bool(std::string str);
};

class Genome
{
public:
    int key;
    float fitness;


private:
    GenomeConfig *config;

    std::vector<int> input_keys;
    std::vector<int> output_keys;
    std::vector<int> hidden_keys;

    std::map<std::pair<int,int>, ConnectionGene *> connections;
    std::map<int, NodeGene *> nodes;

    std::vector<int> forward_order;

    bool activated;

    // Create all input node

public:
    // Constructor
    Genome(int _key, GenomeConfig *_genome_config);

    int get_num_inputs();
    int get_num_outputs();
    int get_num_hidden();
    int get_num_connections();

    std::vector<float> forward(std::vector<float> inputs);

private:
    NodeGene *new_node(int node_key);
    ConnectionGene *new_connection(std::pair<int, int> connection_key);
    std::vector<std::pair<int, int>> generate_full_connections(bool direct);
};

#endif // GENOME_H