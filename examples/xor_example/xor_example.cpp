#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include "population.h"
#include "config_parser.h"

// 2-input XOR inputs and expected outputs.
std::vector<std::vector<float>> xor_inputs = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
std::vector<std::vector<float>> xor_outputs = {{0.0}, {1.0}, {1.0}, {0.0}};

float eval_genomes(Genome_ptr g)
{
    float fitness = 4.0;
    for (int i = 0; i < xor_inputs.size(); i++)
    {
        std::vector<float> output = g->forward(xor_inputs[i]);
        fitness -= std::pow((output[0] - xor_outputs[i][0]), 2);
    }
    return fitness;
}

void run(const std::string &config_file)
{
    std::cout << "Running Experiment with config at: " << config_file << std::endl;

    // Load configuration.
    ConfigParser_ptr config = std::make_shared<ConfigParser>(config_file);

    std::cout << config->to_string() << std::endl;

    // Create the population, which is the top-level object for a NEAT run.
    Population p(config);

    // Run for up to 300 generations.
    Genome_ptr best_genome = p.run(eval_genomes, 300);

    // Display the winning genome.
    std::cout << "\nBest genome:\n"
              << best_genome->key << std::endl;
    std::cout << "Fitness:\n"
              << best_genome->fitness << std::endl;
    std::cout << "Nodes:" << std::endl;
    for (std::pair<const int, NodeGene_ptr> &ngit : best_genome->nodes)
    {
        const int nid = ngit.first;
        NodeGene_ptr n = ngit.second;
        std::cout << "\t" << nid << " DefaultNodeGene(key=" << n->key << ", bias=" << n->get_attribute("bias")->get_string_value() << ", response=" << n->get_attribute("response")->get_string_value() << ", activation=" << n->get_attribute("activation")->get_string_value() << ", aggregation=" << n->get_attribute("aggregation")->get_string_value()
                  << ")" << std::endl;
    }
    std::cout << "Connections:" << std::endl;
    for (std::pair<const std::pair<int, int>, ConnectionGene_ptr> &cgit : best_genome->connections)
    {
        const std::pair<int, int> cid = cgit.first;
        ConnectionGene_ptr n = cgit.second;
        std::cout << "\t(" << cid.first << ", " << cid.second << ") DefaultConnectionGene(key=(" << n->key.first << ", " << n->key.second << "), weight=" << n->get_attribute("weight")->get_string_value() << ", enable=" << n->get_attribute("enable")->get_string_value()
                  << ")" << std::endl;
    }

    // Show output of the most fit genome against training data.
    std::cout << "\nOutput:" << std::endl;
    // best_genome->activate();
    for (size_t i = 0; i < xor_inputs.size(); ++i)
    {
        std::vector<float> output = best_genome->forward(xor_inputs[i]);
        std::cout << "input {" << xor_inputs[i][0] << ", " << xor_inputs[i][1]
                  << "}, expected output {" << xor_outputs[i][0] << "}, got {" << output[0] << "}" << std::endl;
    }

    // Visualization code
}

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        throw std::runtime_error("You must provide a file name");
        return 1;
    }
    srand(time(NULL));
    std::string fname = argv[1];

    ConfigParser_ptr config = std::make_shared<ConfigParser>(fname);

    std::cout << config->to_string() << std::endl;
    run(fname);
    return 0;
}