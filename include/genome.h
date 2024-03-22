#ifndef GENOME_H
#define GENOME_H

#include <map>
#include <string>
#include <vector>
#include <set>
#include "genes.h"
#include "config_parser.h"

typedef std::shared_ptr<class GenomeConfig> GenomeConfig_ptr;

class GenomeConfig : SpecialConfig
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
    std::set<std::string> activation_options;

    std::string aggregation_default;
    float aggregation_mutate_rate;
    std::set<std::string> aggregation_options;

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

    // Mutation Parameters

    GenomeConfig(ConfigParser_ptr _config);
};

typedef std::shared_ptr<class Genome> Genome_ptr;

class Genome
{
public:
    int key;
    float fitness;

    std::map<std::pair<int, int>, ConnectionGene_ptr> connections;
    std::map<int, NodeGene_ptr> nodes;

private:
    GenomeConfig_ptr config;

    std::set<int> input_keys;
    std::set<int> output_keys;
    std::set<int> hidden_keys;

    std::vector<int> forward_order;
    std::map<int, std::set<int>> node_inputs_map;
    bool activated;

public:
    // Constructor
    Genome(int _key, ConfigParser_ptr _config);
    Genome(int _key, Genome_ptr &g1, Genome_ptr &g2, ConfigParser_ptr _config);

    int get_num_inputs();
    int get_num_outputs();
    int get_num_hidden();
    int get_num_connections();
    int get_num_nodes();

    void mutate();
    void activate();
    float distance(Genome_ptr &other);
    std::vector<float> forward(std::vector<float> inputs);

private:
    NodeGene_ptr new_node(int node_key);
    ConnectionGene_ptr new_connection(std::pair<int, int> connection_key);
    std::vector<std::pair<int, int>> generate_full_connections(bool direct);
    void generate_node_inputs();
    void mutate_add_node();
    void mutate_delete_node();
    void mutate_add_conn();
    void mutate_delete_conn();
    bool creates_cycle(std::pair<int, int> conn);
};

#endif // GENOME_H