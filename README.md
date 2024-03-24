# About
neat-cpp is a C++ implementation of the popular NEAT (NeuroEvolution of Augmenting Topologies) algorithm, originally developed as the neat-python library. NEAT is a powerful neuroevolution algorithm designed to evolve artificial neural networks with complex topologies, allowing for adaptive and efficient learning in various tasks, including reinforcement learning, control problems, and more.

This project aims to provide the same functionality and ease of use as neat-python, but implemented in C++ for improved performance and integration into C++ based applications and systems.

## Features

- NEAT Algorithm: Implementations of the NEAT algorithm for evolving neural networks.  
- Flexible and Extensible: Easily customizable to suit various problem domains and experimentation needs.  
- Efficient Performance: Utilizes C++ for improved performance and scalability.
- Integration Support: Designed for seamless integration into C++ projects and applications.  

# Getting Started

## Prerequisites
To use neat-cpp, you need:

- C++20
- cmake >=3.20
- make >=4.3

## Usage
(Provide usage examples and code snippets demonstrating how to use the library)

cpp

```c++
#include "neat.h"

float eval_genomes(Genome_ptr g)
{
    /** Run Evaluation Code */
    return fitness;
}

void run(const std::string &config_file)
{

    // Load configuration.
    ConfigParser_ptr config = std::make_shared<ConfigParser>(config_file);

    // Create the population, which is the top-level object for a NEAT run.
    Population p(config);

    // Run for up to 300 generations.
    Genome_ptr best_genome = p.run(eval_genomes, 10);

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
```

## Examples
Feel free to explore the provided examples for functional uses of this library

## License
This project is licensed under the MIT License.
