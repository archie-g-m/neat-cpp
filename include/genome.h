#ifndef GENOME_H
#define GENOME_H

#include <map>
#include <string>
#include <vector>
#include "genes.h"
#include "config_parser.h"

class GenomeConfig{
    public:
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

        std::string initial_condition;
    private:

    public:
    GenomeConfig(ConfigParser* _config);
}

class Genome {
    public:
        int key;
        float fitness;
    private:
        GenomeConfig* config;

        std::map<int, ConnectionGene*> connections;
        std::map<int, NodeGene*> nodes;

        std::vector<int> input_keys;
        std::vector<int> output_keys;
        std::vector<int> hidden_keys;

    // Create all input node        

    public:
        //Constructor
        Genome(int _key, GenomeConfig* _genome_config);
    private:
        NodeGene* new_node(int node_key);
        ConnectionGene* new_connection(std::pair<int, int> connection_key);
        std::vector<std::pair<int,int>> Genome::generate_full_connections(bool direct);
};

#endif //GENOME_H