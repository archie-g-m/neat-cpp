#include "genome.h"

#include <map>
#include <string>
#include "genes.h"
#include "config_parser.h"

GenomeConfig::GenomeConfig(ConfigParser *_config)
{
    // Configure from Parser
    if (!(_config->data.count("DefaultGenome")))
    { // if parser doesnt have the DefaultGenome category throw error
        throw(std::invalid_argument("Provided ConfigParser does not contain the DefaultGenome category"));
    }

    std::map<std::string, std::string> GenomeData = _config->data["DefaultGenome"];

    // Distance Parameters
    this->compatibility_disjoint_coefficient = std::stof(GenomeData["compatibility_disjoint_coefficient"]);
    this->compatibility_weight_coefficient = std::stof(GenomeData["compatibility_weight_coefficient"]);

    // Mutate Parameters
    this->conn_add_prob = std::stof(GenomeData["conn_add_prob"]);
    this->conn_delete_prob = std::stof(GenomeData["conn_delete_prob"]);

    this->node_add_prob = std::stof(GenomeData["node_add_prob"]);
    this->node_delete_prob = std::stof(GenomeData["node_delete_prob"]);

    // Node Parameters
    this->num_hidden = std::stoi(GenomeData["num_hidden"]);
    this->num_inputs = std::stoi(GenomeData["num_inputs"]);
    this->num_outputs = std::stoi(GenomeData["num_outputs"]);

    this->bias_init_mean = std::stof(GenomeData["bias_init_mean"]);
    this->bias_init_stdev = std::stof(GenomeData["bias_init_stdev"]);
    this->bias_init_type = GenomeData["bias_init_type"];
    this->bias_max_value = std::stof(GenomeData["bias_max_value"]);
    this->bias_min_value = std::stof(GenomeData["bias_min_value"]);
    this->bias_mutate_power = std::stof(GenomeData["bias_mutate_power"]);
    this->bias_mutate_rate = std::stof(GenomeData["bias_mutate_rate"]);
    this->bias_replace_rate = std::stof(GenomeData["bias_replace_rate"]);

    this->response_init_mean = std::stof(GenomeData["response_init_mean"]);
    this->response_init_stdev = std::stof(GenomeData["response_init_stdev"]);
    this->response_init_type = GenomeData["response_init_type"];
    this->response_max_value = std::stof(GenomeData["response_max_value"]);
    this->response_min_value = std::stof(GenomeData["response_min_value"]);
    this->response_mutate_power = std::stof(GenomeData["response_mutate_power"]);
    this->response_mutate_rate = std::stof(GenomeData["response_mutate_rate"]);
    this->response_replace_rate = std::stof(GenomeData["response_replace_rate"]);

    // Connection Parameters
    this->initial_connection = GenomeData["initial_connection"];

    this->activation_default = GenomeData["activation_default"];
    this->activation_mutate_rate = std::stof(GenomeData["activation_mutate_rate"]);
    this->activation_options = GenomeData["activation_options"];

    this->aggregation_default = GenomeData["aggregation_default"];
    this->aggregation_mutate_rate = std::stof(GenomeData["aggregation_mutate_rate"]);
    this->aggregation_options = GenomeData["aggregation_options"];

    this->enabled_default = to_bool(GenomeData["enabled_default"]);
    this->enabled_mutate_rate = std::stof(GenomeData["enabled_mutate_rate"]);
    this->enabled_rate_to_true_add = std::stof(GenomeData["enabled_rate_to_true_add"]);
    this->enabled_rate_to_false_add = std::stof(GenomeData["enabled_rate_to_false_add"]);

    this->weight_init_mean = std::stof(GenomeData["weight_init_mean"]);
    this->weight_init_stdev = std::stof(GenomeData["weight_init_stdev"]);
    this->weight_init_type = GenomeData["weight_init_type"];
    this->weight_max_value = std::stof(GenomeData["weight_max_value"]);
    this->weight_min_value = std::stof(GenomeData["weight_min_value"]);
    this->weight_mutate_power = std::stof(GenomeData["weight_mutate_power"]);
    this->weight_mutate_rate = std::stof(GenomeData["weight_mutate_rate"]);
    this->weight_replace_rate = std::stof(GenomeData["weight_replace_rate"]);
}
/**
 * @brief converts string to boolean if it is 'true', 'false', '1' or '0' (case doesn't matter)
 *
 * @param str
 * @return true
 * @return false
 */
bool GenomeConfig::to_bool(std::string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    if (str == "true" || str == "1")
    {
        return true;
    }
    else if (str == "false" || str == "0")
    {
        return false;
    }
    else
    {
        throw std::invalid_argument("Invalid argument '" + str + "' provided to Genome::to_bool");
    }
}

Genome::Genome(int _key, GenomeConfig *_genome_config)
{
    key = _key;
    fitness = 0.F;

    // Point to the container containing the configuration for all genomes
    // (So the memory of each genome doesnt have to maintain a copy of each parameter,
    //  instead they can merely access the information stored inside the GenomeConfig)
    this->config = _genome_config;

    // Create all input nodes
    for (int in_node = 0; in_node < config->num_inputs; in_node++)
    {
        // All input nodes have negative values
        int node_key = -1 * in_node;
        // Create New Node
        this->nodes[node_key] = new_node(node_key);
        // Add key to list of output keys
        this->input_keys.push_back(node_key);
    }
    // Create all output nodes
    for (int out_node = 0; out_node < config->num_outputs; out_node++)
    {
        // All output nodes have positive values starting at 0
        int node_key = out_node;
        // Create New Node
        this->nodes[node_key] = new_node(node_key);
        // Add key to list of output keys
        this->output_keys.push_back(node_key);
    }
    // Create all hidden nodes
    for (int hid_node = 0; hid_node < config->num_hidden; hid_node++)
    {
        // All hidden nodes have positive values starting at num_outputs
        int node_key = hid_node + config->num_outputs;
        // Create New Node
        this->nodes[node_key] = new_node(node_key);
        // Add key to list of hidden keys
        this->hidden_keys.push_back(node_key);
    }
    // Create connections between nodes
    std::vector<std::pair<int, int>> connection_list;
    if (config->initial_connection == "full_direct")
    {
        connection_list = generate_full_connections(true);
    }
    else if (config->initial_connection == "full_indirect")
    {
        connection_list = generate_full_connections(false);
    }
    else
    {
        throw(std::invalid_argument("Incorrect initial condition provided"));
    }

    for (std::pair<int, int> conn_key : connection_list)
    {
        connections[conn_key] = new_connection(conn_key);
    }

    // has this node been activated (raw nodes and connections turned into feed forward layers)
    activated = false;
}
// Get sizes from key vectors as size can change after initialization from config
/**
 * @brief return number of input nodes
 *
 * @return int
 */
int Genome::get_num_inputs()
{
    return input_keys.size();
}
/**
 * @brief return number of output nodes
 *
 * @return int
 */
int Genome::get_num_outputs()
{
    return output_keys.size();
}
/**
 * @brief return number of hidden nodes
 *
 * @return int
 */
int Genome::get_num_hidden()
{
    return hidden_keys.size();
}
/**
 * @brief return number of connections
 *
 * @return int
 */
int Genome::get_num_connections()
{
    return connections.size();
}
/**
 * @brief Generate a new node from the config with the provided node_key
 *
 * @param node_key
 * @return NodeGene*
 */
NodeGene *Genome::new_node(int node_key)
{
    // std::string bias_key = "gene" + std::to_string(this->key) + "_node" + std::to_string(node_key) + "_bias";
    std::string bias_key = "bias";
    FloatAttribute *bias = new FloatAttribute(bias_key,
                                              this->config->bias_mutate_rate,
                                              this->config->bias_mutate_power,
                                              this->config->bias_min_value,
                                              this->config->bias_max_value);
    // Create Key that can identify where this attribute is in the network
    // std::string response_key = "gene" + std::to_string(this->key) + "_node" + std::to_string(node_key) + "_response";
    std::string response_key = "response";
    FloatAttribute *response = new FloatAttribute(response_key,
                                                  this->config->response_mutate_rate,
                                                  this->config->response_mutate_power,
                                                  this->config->response_min_value,
                                                  this->config->response_max_value);
    std::vector<Attribute *> node_attributes;
    node_attributes.push_back(bias);
    node_attributes.push_back(response);
    NodeGene *node = new NodeGene(node_key, node_attributes);
    return node;
}
/**
 * @brief generates a new connection from the provided key
 *
 * @param connection_key
 * @return ConnectionGene*
 */
ConnectionGene *Genome::new_connection(std::pair<int, int> connection_key)
{
    std::string weight_key = "weight";
    FloatAttribute *weight = new FloatAttribute(weight_key,
                                                this->config->weight_mutate_rate,
                                                this->config->weight_mutate_power,
                                                this->config->weight_min_value,
                                                this->config->weight_max_value);

    std::string enable_key = "enable";
    BoolAttribute *enable = new BoolAttribute(enable_key,
                                              this->config->enabled_mutate_rate);

    std::vector<Attribute *> connection_attributes;
    connection_attributes.push_back(weight);
    connection_attributes.push_back(enable);

    ConnectionGene *conneciton = new ConnectionGene(connection_key, connection_attributes);
    return conneciton;
}
/**
 * @brief generates the full set of connections.
 *        if direct is true then connections will be generated from:
 *          - inputs -> hidden (if there are any hidden nodes)
 *          - inputs -> outputs
 *          - hidden -> outputs (if there are any hidden nodes)
 *        if direct if false then connections will only be generated from:
 *          - inputs -> hidden
 *          - hidden -> outputs
 *
 * @param direct
 * @return std::vector<std::pair<int,int>>
 */
std::vector<std::pair<int, int>> Genome::generate_full_connections(bool direct)
{
    // TODO This can probably be optimized more
    // currently O(inputs*(hidden+outputs) + hidden*outputs)
    std::vector<std::pair<int, int>> connections;
    if (direct || hidden_keys.empty())
    {
        for (int in_key : input_keys)
        {
            // inputs -> hidden (if possible)
            if (!hidden_keys.empty())
            {
                for (int hd_key : hidden_keys)
                {
                    std::pair<int, int> conn_key;
                    conn_key.first = in_key;
                    conn_key.second = hd_key;
                    connections.push_back(conn_key);
                }
            }
            // inputs -> outputs
            for (int ou_key : output_keys)
            {
                std::pair<int, int> conn_key;
                conn_key.first = in_key;
                conn_key.second = ou_key;
                connections.push_back(conn_key);
            }
        }
        if (!hidden_keys.empty())
        {
            // hidden -> outputs
            for (int hd_key : hidden_keys)
            {
                for (int ou_key : output_keys)
                {
                    std::pair<int, int> conn_key;
                    conn_key.first = hd_key;
                    conn_key.second = ou_key;
                    connections.push_back(conn_key);
                }
            }
        }
    }
    else
    {
        // inputs -> outputs
        for (int in_key : input_keys)
        {
            for (int ou_key : output_keys)
            {
                std::pair<int, int> conn_key;
                conn_key.first = in_key;
                conn_key.second = ou_key;
                connections.push_back(conn_key);
            }
        }
        // hidden -> outputs
        for (int hd_key : hidden_keys)
        {
            for (int ou_key : output_keys)
            {
                std::pair<int, int> conn_key;
                conn_key.first = hd_key;
                conn_key.second = ou_key;
                connections.push_back(conn_key);
            }
        }
    }
    return connections;
}
/**
 * @brief mutates the genome according to the configuration
 */
void mutate(){
    return;
}
/**
 * @brief activates this network to be efficiently computed in the forward function
 * 
 */
void activate(){
    return;
}
std::vector<float> Genome::forward(std::vector<float> inputs){
    assert(this->activated);

    std::map<int, float> input_values;

    for(int node_key : this->forward_order){
        continue;
    }

}