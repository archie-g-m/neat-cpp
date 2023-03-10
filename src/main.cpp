#include <iostream>
#include <string>
#include <random>
#include <vector>
#include "config_parser.h"
#include "genome.h"

int main(int argc, char *argv[])
{
    if(argc < 1){throw(std::runtime_error("You must provide a file name"));}
    srand(time(NULL));
    std::string fname = argv[1];
    
    ConfigParser* config = new ConfigParser(fname);

    std::cout << config->to_string() << std::endl;

    GenomeConfig* genome_config = new GenomeConfig(config);

    Genome *genome = new Genome(1, genome_config);
    std::cout << "Activating Network" << std::endl;
    genome->activate();
    std::cout << "Running Forward Computation on Network" << std::endl;
    std::vector<float> test_in ;
    for(int i=1; i<=genome_config->num_inputs; i++){test_in.push_back((float)i);}
    std::vector<float> out = genome->forward(test_in);
    std::cout <<"Output [";
    for(float o : out){
        std::cout << o << ", ";
    }
    std::cout << "]" << std::endl;



    std::cout << "Genome Size: " << sizeof(*genome) << " Bytes" << std::endl;

    return 0;
}