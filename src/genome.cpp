#include "genome.h"

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include <cassert>
#include "genes.h"
#include "aggregations.h"
#include "activations.h"
#include "config_parser.h"

GenomeConfig::GenomeConfig(ConfigParser_ptr _config)
{
    // Configure from Parser
    data = _config->get_subdata("DefaultGenome");

    // Distance Parameters
    compatibility_disjoint_coefficient = get_value<float>("compatibility_disjoint_coefficient");
    compatibility_weight_coefficient = get_value<float>("compatibility_weight_coefficient");

    // Mutate Parameters
    conn_add_prob = get_value<float>("conn_add_prob");
    conn_delete_prob = get_value<float>("conn_delete_prob");

    node_add_prob = get_value<float>("node_add_prob");
    node_delete_prob = get_value<float>("node_delete_prob");

    // Node Parameters
    num_hidden = get_value<int>("num_hidden");
    num_inputs = get_value<int>("num_inputs");
    num_outputs = get_value<int>("num_outputs");

    bias_init_mean = get_value<float>("bias_init_mean");
    bias_init_stdev = get_value<float>("bias_init_stdev");
    bias_init_type = get_value<std::string>("bias_init_type");
    bias_max_value = get_value<float>("bias_max_value");
    bias_min_value = get_value<float>("bias_min_value");
    bias_mutate_power = get_value<float>("bias_mutate_power");
    bias_mutate_rate = get_value<float>("bias_mutate_rate");
    bias_replace_rate = get_value<float>("bias_replace_rate");

    response_init_mean = get_value<float>("response_init_mean");
    response_init_stdev = get_value<float>("response_init_stdev");
    response_init_type = get_value<std::string>("response_init_type");
    response_max_value = get_value<float>("response_max_value");
    response_min_value = get_value<float>("response_min_value");
    response_mutate_power = get_value<float>("response_mutate_power");
    response_mutate_rate = get_value<float>("response_mutate_rate");
    response_replace_rate = get_value<float>("response_replace_rate");

    // Connection Parameters
    initial_connection = get_value<std::string>("initial_connection");

    activation_default = get_value<std::string>("activation_default");
    activation_mutate_rate = get_value<float>("activation_mutate_rate");
    activation_options = get_value<std::set<std::string>>("activation_options");

    aggregation_default = get_value<std::string>("aggregation_default");
    aggregation_mutate_rate = get_value<float>("aggregation_mutate_rate");
    aggregation_options = get_value<std::set<std::string>>("aggregation_options");

    enabled_default = get_value<bool>("enabled_default");
    enabled_mutate_rate = get_value<float>("enabled_mutate_rate");
    enabled_rate_to_true_add = get_value<float>("enabled_rate_to_true_add");
    enabled_rate_to_false_add = get_value<float>("enabled_rate_to_false_add");

    weight_init_mean = get_value<float>("weight_init_mean");
    weight_init_stdev = get_value<float>("weight_init_stdev");
    weight_init_type = get_value<std::string>("weight_init_type");
    weight_max_value = get_value<float>("weight_max_value");
    weight_min_value = get_value<float>("weight_min_value");
    weight_mutate_power = get_value<float>("weight_mutate_power");
    weight_mutate_rate = get_value<float>("weight_mutate_rate");
    weight_replace_rate = get_value<float>("weight_replace_rate");
}

Genome::Genome(int _key, ConfigParser_ptr _config)
{
    key = _key;
    fitness = 0.F;

    // Point to the container containing the configuration for all genomes
    // (So the memory of each genome doesnt have to maintain a copy of each parameter,
    //  instead they can merely access the information stored inside the GenomeConfig)
    config = std::make_shared<GenomeConfig>(_config);

    // Create all input nodes
    for (int in_node = 1; in_node <= config->num_inputs; in_node++)
    {
        // All input nodes have negative values starting at -1
        int node_key = -1 * in_node;
        // Create New Node
        nodes[node_key] = new_node(node_key);
        // Add key to list of output keys
        input_keys.insert(node_key);
    }
    // Create all output nodes
    for (int out_node = 0; out_node < config->num_outputs; out_node++)
    {
        // All output nodes have positive values starting at 0
        int node_key = out_node;
        // Create New Node
        nodes[node_key] = new_node(node_key);
        // Add key to list of output keys
        output_keys.insert(node_key);
    }
    // Create all hidden nodes
    for (int hid_node = 0; hid_node < config->num_hidden; hid_node++)
    {
        // All hidden nodes have positive values starting at num_outputs
        int node_key = hid_node + config->num_outputs;
        // Create New Node
        nodes[node_key] = new_node(node_key);
        // Add key to list of hidden keys
        hidden_keys.insert(node_key);
    }
    // Create connections between nodes
    std::vector<std::pair<int, int>> connection_list;
    if (config->initial_connection == "full_direct")
    {
        generate_full_connections(true, connection_list);
    }
    else if (config->initial_connection == "full_indirect")
    {
        generate_full_connections(false, connection_list);
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

Genome::Genome(int _key, Genome_ptr &g1, Genome_ptr &g2, ConfigParser_ptr _config)
{
    key = _key;
    fitness = 0.F;

    // Point to the container containing the configuration for all genomes
    // (So the memory of each genome doesnt have to maintain a copy of each parameter,
    //  instead they can merely access the information stored inside the GenomeConfig)
    config = std::make_shared<GenomeConfig>(_config);

    Genome_ptr parent1;
    Genome_ptr parent2;
    if (g1->fitness > g2->fitness)
    {
        parent1 = g1;
        parent2 = g2;
    }
    else
    {
        parent1 = g2;
        parent2 = g1;
    }

    // Crossover all nodes from parents
    for (std::pair<const int, NodeGene_ptr> &nit : parent1->nodes)
    {
        const int nid = nit.first;
        NodeGene_ptr n1 = nit.second;
        if (parent2->nodes.count(nid))
        {
            NodeGene_ptr n2 = parent2->nodes[nid];
            nodes[nid] = n1->crossover(n2);
        }
        else
        {
            nodes[nid] = n1->copy();
        }

        // Insert keys into key sets
        if (nid < 0)
        {
            input_keys.insert(nid);
        }
        else if (nid < config->num_outputs)
        {
            output_keys.insert(nid);
        }
        else
        {
            hidden_keys.insert(nid);
        }
    }

    // Crossover all Connections
    for (const std::pair<std::pair<int, int>, ConnectionGene_ptr> &cit : parent1->connections)
    {
        const std::pair<int, int> cid = cit.first;
        ConnectionGene_ptr c1 = cit.second;
        if (parent2->connections.count(cid))
        {
            ConnectionGene_ptr c2 = parent2->connections[cid];
            connections[cid] = c1->crossover(c2);
        }
        else
        {
            connections[cid] = c1->copy();
        }
    }

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
 * @brief gets total number of nodes in network
 *
 * @return int
 */
int Genome::get_num_nodes()
{
    return nodes.size();
}
/**
 * @brief Generate a new node from the config with the provided node_key
 *
 * @param node_key
 * @return NodeGene*
 */
NodeGene_ptr Genome::new_node(int node_key)
{
    // std::string bias_key = "gene" + std::to_string(key) + "_node" + std::to_string(node_key) + "_bias";
    std::string bias_key = "bias";
    FloatAttribute_ptr bias = std::make_shared<FloatAttribute>(bias_key,
                                                               config->bias_init_mean,
                                                               config->bias_init_stdev,
                                                               config->bias_init_type,
                                                               config->bias_mutate_rate,
                                                               config->bias_mutate_power,
                                                               config->bias_min_value,
                                                               config->bias_max_value);
    // Create Key that can identify where this attribute is in the network
    // std::string response_key = "gene" + std::to_string(key) + "_node" + std::to_string(node_key) + "_response";
    std::string response_key = "response";
    FloatAttribute_ptr response = std::make_shared<FloatAttribute>(response_key,
                                                                   config->response_init_mean,
                                                                   config->response_init_stdev,
                                                                   config->response_init_type,
                                                                   config->response_mutate_rate,
                                                                   config->response_mutate_power,
                                                                   config->response_min_value,
                                                                   config->response_max_value);
    std::string activation_key = "activation";
    StringAttribute_ptr activation = std::make_shared<StringAttribute>(activation_key,
                                                                       config->activation_mutate_rate,
                                                                       config->activation_options);
    std::string aggregation_key = "aggregation";
    StringAttribute_ptr aggregation = std::make_shared<StringAttribute>(aggregation_key,
                                                                        config->aggregation_mutate_rate,
                                                                        config->aggregation_options);
    std::vector<Attribute_ptr> node_attributes = {bias, response, activation, aggregation};

    NodeGene_ptr node = std::make_shared<NodeGene>(node_key, node_attributes);
    return node;
}
/**
 * @brief generates a new connection from the provided key
 *
 * @param connection_key
 * @return std::shared_pointer<>
 */
ConnectionGene_ptr Genome::new_connection(std::pair<int, int> connection_key)
{
    std::string weight_key = "weight";
    FloatAttribute_ptr weight = std::make_shared<FloatAttribute>(weight_key,
                                                                 config->weight_init_mean,
                                                                 config->weight_init_stdev,
                                                                 config->weight_init_type,
                                                                 config->weight_mutate_rate,
                                                                 config->weight_mutate_power,
                                                                 config->weight_min_value,
                                                                 config->weight_max_value);

    std::string enable_key = "enable";
    BoolAttribute_ptr enable = std::make_shared<BoolAttribute>(enable_key,
                                                               config->enabled_default,
                                                               config->enabled_mutate_rate);

    std::vector<Attribute_ptr> connection_attributes = {weight, enable};

    ConnectionGene_ptr conneciton = std::make_shared<ConnectionGene>(connection_key, connection_attributes);
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
 * @param connections
 */
void Genome::generate_full_connections(bool direct, std::vector<std::pair<int, int>> &connections)
{
    // TODO This can probably be optimized more
    // currently O(inputs*(hidden+outputs) + hidden*outputs)
    int num_inputs = get_num_inputs();
    int num_hidden = get_num_hidden();
    int num_outputs = get_num_outputs();
    connections.clear();
    if (direct || hidden_keys.empty())
    {
        int num_connections = num_inputs * (num_hidden + num_outputs) + num_hidden * num_outputs;
        connections.reserve(num_connections);
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
        int num_connections = num_inputs * num_hidden + num_hidden * num_outputs;
        connections.reserve(num_connections);
        // inputs -> hidden
        for (int in_key : input_keys)
        {
            for (int hd_key : hidden_keys)
            {
                std::pair<int, int> conn_key;
                conn_key.first = in_key;
                conn_key.second = hd_key;
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
}
/**
 * @brief finds the inputs to each node in the network
 * Computes in linear time O(N) (O(num_nodes) + O(num_connections))
 *
 * @return std::map<int, std::vector<int>*>
 */
void Genome::generate_node_inputs()
{
    node_inputs_map.clear();
    // add all node keys to input map
    for (std::map<int, NodeGene_ptr>::iterator node_it = nodes.begin(); node_it != nodes.end(); node_it++)
    {
        node_inputs_map.insert(std::pair<int, std::set<int>>(node_it->first, std::set<int>()));
    }

    // iterate through all connections and add the input to the list of the output's inputs
    for (std::map<std::pair<int, int>, ConnectionGene_ptr>::iterator conn_it = connections.begin(); conn_it != connections.end(); conn_it++)
    {
        // if Connection is enabled then add the input to the list of output's inputs
        if (conn_it->second->get_attribute("enable")->get_bool_value())
        {
            int out = conn_it->first.second; // key
            int in = conn_it->first.first;   // value
            node_inputs_map[out].insert(in);
        }
    }
}
/**
 * @brief mutates the genome according to the configuration
 */
void Genome::mutate()
{
    // maybe remove the activated assertion and just activate network every mutation
    activated = false;
    forward_order.clear();
    if (rand() * RAND_MAX_INV < config->node_add_prob)
    {
        mutate_add_node();
    }
    if (rand() * RAND_MAX_INV < config->node_delete_prob)
    {
        mutate_delete_node();
    }
    if (rand() * RAND_MAX_INV < config->conn_add_prob)
    {
        mutate_add_conn();
    }
    if (rand() * RAND_MAX_INV < config->conn_delete_prob)
    {
        mutate_delete_conn();
    }

    for (std::pair<const int, NodeGene_ptr> &nit : nodes)
    {
        NodeGene_ptr n = nit.second;
        n->mutate();
    }

    for (std::pair<const std::pair<int, int>, ConnectionGene_ptr> &cit : connections)
    {
        ConnectionGene_ptr c = cit.second;
        c->mutate();
    }
}
/**
 * @brief adds random node
 *
 */
void Genome::mutate_add_node()
{
    // You will always be adding a hidden node
    // Hidden nodes are always positive and begin their indexing at num_outputs
    int new_node_key = get_num_outputs() + get_num_hidden();
    NodeGene_ptr new_node_ptr = new_node(new_node_key);
    nodes[new_node_key] = new_node_ptr;
    hidden_keys.insert(new_node_key);

    // Choose random connection
    if (connections.size())
    {
        auto conn_it = connections.cbegin();
        std::advance(conn_it, int(rand() % connections.size()));
        std::pair<int, int> conn = conn_it->first;
        int in = conn.first;
        int out = conn.second;
        // Now Disable the connection we are splitting
        connections[conn]->disable();
        // Generate the new connections into and out of the new node
        std::pair<int, int> in_key = {in, new_node_key};
        ConnectionGene_ptr in_conn = new_connection(in_key);
        connections[in_key] = in_conn;
        std::pair<int, int> out_key = {new_node_key, out};
        ConnectionGene_ptr out_conn = new_connection(out_key);
        connections[out_key] = out_conn;
    }
}
/**
 * @brief deletes random node
 *
 */
void Genome::mutate_delete_node()
{
    // if there are no hidden keys to delete then break
    if (get_num_hidden() <= 0)
    {
        return;
    }
    // otherwise pick a random hidden node
    auto node_key = hidden_keys.cbegin();
    std::advance(node_key, rand() % get_num_hidden());
    int node_to_remove = *node_key;

    // remove the node from the node map
    nodes.erase(node_to_remove); // delete the key and pointer from map
    hidden_keys.erase(node_to_remove);
    node_inputs_map.erase(node_to_remove);

    // remove all connections to the node from the connection key
    std::vector<std::pair<int, int>> conns_to_remove = {}; // store all the keys before erasing them from the map
    conns_to_remove.reserve(nodes.size());                 // a node can at most be connected to every other node
    for (std::pair<const std::pair<int, int>, ConnectionGene_ptr> &it : connections)
    {
        std::pair<int, int> key = it.first;
        ConnectionGene_ptr conn = it.second;
        // if this node goes into or out of the node to remove then delete it
        if (key.first == node_to_remove || key.second == node_to_remove)
        {
            if (key.first == node_to_remove)
            {
                node_inputs_map[key.second].erase(key.first);
            }
            else
            {
                node_inputs_map[key.first].erase(key.second);
            }
            conns_to_remove.push_back(key); // add this connection to the list of connections to remove from the map
        }
    }
    // remove all the needed conneciotns from the map
    for (std::pair<int, int> c : conns_to_remove)
    {
        connections.erase(c);
    }
}
/**
 * @brief adds new connection
 *
 */
void Genome::mutate_add_conn()
{
    std::set<int> possible_inputs;
    std::set_union(input_keys.begin(), input_keys.end(), hidden_keys.begin(), hidden_keys.end(), std::inserter(possible_inputs, possible_inputs.begin()));

    std::set<int> possible_outputs;
    std::set_union(hidden_keys.begin(), hidden_keys.end(), output_keys.begin(), output_keys.end(), std::inserter(possible_outputs, possible_outputs.begin()));

    // Generate all permutations of inputs and outputs
    // Notes:
    // - Node cant connect to itself
    // - Node can not connect a loop
    std::set<std::pair<int, int>> possible_connections;
    for (int i : possible_inputs)
    {
        for (int o : possible_outputs)
        {
            // Construct the possible connection key
            std::pair<int, int> conn_key = {i, o};
            // Pass if attempting to make a connection to itself
            if (i == o)
            {
                continue;
            }
            // Pass if this connection already exists
            if (connections.find(conn_key) != connections.end())
            {
                continue;
            }
            // Pass if this connections completes the a cycle
            if (creates_cycle(conn_key))
            {
                continue;
            }
            // If you reached this point, its safe to add it to a possible conneciton
            possible_connections.insert(conn_key);
        }
    }
    // Don't continue if there are no possible connections to add
    if (!possible_connections.size())
    {
        return;
    }
    // Pick a random connection from the possible connections and add it to the network
    auto it = possible_connections.begin();
    std::advance(it, rand() % possible_connections.size());
    std::pair<int, int> conn_key = *it;
    connections[conn_key] = new_connection(conn_key);
}
/**
 * @brief deletes random connection
 *
 */
void Genome::mutate_delete_conn()
{
    // If there are no connections then you cant remove any
    if (!connections.size())
    {
        return;
    }
    auto it = connections.begin();
    std::advance(it, rand() % connections.size());
    connections.erase(it->first);
}

/**
 * @brief checks whether the given connection creates a cycle in the nodes
 *
 * @param conn
 * @return true
 * @return false
 */
bool Genome::creates_cycle(std::pair<int, int> conn)
{
    // Create a map of node keys and the nodes they connect to
    std::unordered_map<int, std::vector<int>> conn_map;
    for (auto it : connections)
    {
        std::pair<int, int> key = it.first;
        // If input is not in map create list
        if (conn_map.find(key.first) == conn_map.end())
        {
            std::vector<int> outs = {key.second};
            conn_map[key.first] = outs;
        }
        else
        {
            conn_map[key.first].push_back(key.second);
        }
    }

    std::set<int> visited;
    std::queue<int> q;
    q.push(conn.second);
    int curr;
    // BFS, if you get to the beginning of the test connection then
    while (!q.empty())
    {
        curr = q.front();
        q.pop();
        visited.insert(curr);
        for (int o : conn_map[curr])
        {
            // if the potential node is the beginning of the cycle then return true
            if (o == conn.first)
            {
                return true;
            }
            if (visited.find(o) == visited.end())
            {
                q.push(o);
            }
        }
    }
    // If you BFS'd through all the points and didnt create a loop then return false
    return false;
}
/**
 * @brief activates this network to be efficiently computed in the forward function
 */
void Genome::activate()
{
    std::set<int> added_keys;
    // empty the cached forward order
    forward_order.clear();
    // recalculate the inputs to each node
    generate_node_inputs();
    // inputs dont have any dependent nodes so add them first
    for (int in_key : input_keys)
    {
        forward_order.push_back(in_key);
        added_keys.insert(in_key);
    }
    // work through hidden nodes to find which ones depend on eachother
    std::set<int> hidden_key_copy(hidden_keys);
    while (!hidden_key_copy.empty())
    {
        std::set<int> keys_added_this_it = {};
        for (int hidden_key : hidden_key_copy)
        {
            bool add_node = true;
            // if one of this nodes inputs is not in the current forward order then skip it for now
            for (int input : node_inputs_map[hidden_key])
            {
                if (added_keys.count(input) == 0)
                {
                    add_node = false;
                    break;
                }
            }

            if (add_node)
            {
                forward_order.push_back(hidden_key);
                added_keys.insert(hidden_key);
                int complete_key = hidden_key;
                // Continuously loop thorugh hidden keys until theyre done
                keys_added_this_it.insert(complete_key);
            }
        }
        // If we are unable to add any more hidden nodes to the order then we are done
        if (keys_added_this_it.size() == 0)
        {
            break;
        }
        for (int key_to_remove : keys_added_this_it)
        {
            hidden_key_copy.erase(key_to_remove);
        }
    }
    // outputs dont depend on eachother so you can just add them all last
    for (int out_key : output_keys)
    {
        forward_order.push_back(out_key);
    }
    activated = true;
}

/**
 * @brief Computes the difference between this genome and the other
 *
 * @param other
 * @return float
 */
float Genome::distance(Genome_ptr &other)
{
    // Compute node gene distance component.
    float node_distance = 0.0F;

    // if the other genome has nodes
    if (nodes.size() > 0 || other->nodes.size() > 0)
    {
        float disjoint_nodes = 0.0F;

        for (std::pair<const int, NodeGene_ptr> &n2 : other->nodes)
        {
            if (!nodes.count(n2.first))
            {
                disjoint_nodes += 1.0F;
            }
        }

        for (std::pair<const int, NodeGene_ptr> &n1 : nodes)
        {

            if (!other->nodes.count(n1.first))
            {
                disjoint_nodes += 1.0F;
            }
            else
            {
                // Homologous genes compute their own distance value.
                NodeGene_ptr n2 = other->nodes[n1.first];

                node_distance += n1.second->distance(n2, config->compatibility_weight_coefficient);
            }
        }
        float max_nodes = fmax(nodes.size(), other->nodes.size());
        node_distance = (node_distance + (config->compatibility_disjoint_coefficient * disjoint_nodes)) / max_nodes;
    }

    // Compute node gene distance component.
    float connection_distance = 0.0F;
    if (connections.size() > 0 || other->connections.size() > 0)
    {
        float disjoint_connections = 0.0F;
        for (std::pair<const std::pair<int, int>, ConnectionGene_ptr> &c2 : other->connections)
        {
            if (!connections.count(c2.first))
            {
                disjoint_connections += 1.0F;
            }
        }

        for (std::pair<const std::pair<int, int>, ConnectionGene_ptr> &c1 : connections)
        {
            if (!other->connections.count(c1.first))
            {
                disjoint_connections += 1.0F;
            }
            else
            {
                // Homologous genes compute their own distance value.
                ConnectionGene_ptr &c2 = other->connections[c1.first];
                connection_distance += c1.second->distance(c2, config->compatibility_weight_coefficient);
            }
        }

        float max_connections = fmax(connections.size(), other->connections.size());
        connection_distance = (connection_distance + (config->compatibility_disjoint_coefficient * disjoint_connections)) / max_connections;
    }

    float distance = std::fabs(node_distance + connection_distance);
    return distance;
}

/**
 * @brief computes the result of providing the given inputs to the network
 *
 * @param inputs
 * @return std::vector<float>
 */
std::vector<float> Genome::forward(std::vector<float> inputs)
{
    if (!activated)
    {
        throw std::runtime_error("Genome must be activated before calling forward");
    }

    if (inputs.size() != input_keys.size())
    {
        throw std::invalid_argument("Incorrect number of keys provided, given: " +
                                    std::to_string(input_keys.size()) +
                                    ", need: " +
                                    std::to_string(inputs.size()));
    }

    std::map<int, float> input_values;
    std::set<int> node_inputs;
    float node_value;
    for (int node_key : forward_order)
    {

        NodeGene_ptr this_node = nodes[node_key];
        float bias = this_node->get_attribute("bias")->get_float_value();
        float response = this_node->get_attribute("response")->get_float_value();
        std::string activation_method = this_node->get_attribute("activation")->get_string_value();
        std::string aggregation_method = this_node->get_attribute("aggregation")->get_string_value();
        // If this node is an input pull it's value from the inputs
        std::set<int>::iterator input_ind = input_keys.find(node_key);
        if (input_ind == input_keys.end())
        {
            // If this node is not an input, aggregate the node's inputs to be the value
            node_inputs = node_inputs_map[node_key];
            std::vector<float> agg_vec;
            for (int node_input_id : node_inputs)
            {
                std::pair<int, int> con(node_input_id, node_key);
                float w = connections[con]->get_attribute("weight")->get_float_value();
                agg_vec.push_back(input_values[node_input_id] * w);
            }
            node_value = aggregate_vector(agg_vec, aggregation_method);
            // Apply activation function to the value
            node_value = activate_value((bias + (response * node_value)), activation_method);
        }
        else
        {
            node_value = inputs[std::distance(input_keys.begin(), input_ind)];
        }

        input_values[node_key] = node_value;
    }
    std::vector<float> outputs;

    for (int out_key : output_keys)
    {
        outputs.push_back(input_values[out_key]);
    }

    return outputs;
}