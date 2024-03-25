#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include "neat.h"

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
    Genome_ptr best_genome = p.run(eval_genomes, 300, 0);

    // Display the winning genome.
    std::cout << "\nBest genome:\n"
              << best_genome->to_string() << std::endl;

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

    run(fname);
    return 0;
}