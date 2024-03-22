#ifndef POPULATION_H
#define POPULATION_H

// #define TEST_MODE

#include <string>
#include <functional>

#include "genome.h"
#include "species.h"
#include "config_parser.h"

typedef std::shared_ptr<class PopulationConfig> PopulationConfig_ptr;

class PopulationConfig : SpecialConfig
{
public:
    // Population Runtime Config
    std::string fitness_criterion;
    float fitness_threshold;
    int pop_size;
    bool reset_on_extinction;
    bool no_fitness_termination;
    // Stagnation Config
    std::string species_fitness_func;
    int max_stagnation;
    int species_elitism;
    // Reproduction Config
    int elitism;
    float survival_threshold;
    int min_species_size;

    PopulationConfig(ConfigParser_ptr _config);
};

typedef std::shared_ptr<class Population> Population_ptr;

class Population
{
#ifdef TEST_MODE
public:
#else
public:
#endif
    ConfigParser_ptr raw_config;
    PopulationConfig_ptr config;
    int total_genomes;
    SpeciesSet_ptr species_set;
    std::map<int, Genome_ptr> population;

public:
    Population(ConfigParser_ptr _config);
    Genome_ptr run(std::function<float(Genome_ptr)> fitness_function, int n = -1);

#ifdef TEST_MODE
public:
#else
private:
#endif
    std::map<int, Genome_ptr> new_population(ConfigParser_ptr _config, int pop_size);
    std::vector<int> get_stagnant_species(int generation);
    std::map<int, int> calc_spawns(std::map<int, float> adj_fitnesses, std::map<int, int> prev_sizes);
    std::map<int, Genome_ptr> reproduce(int generation);
};

#endif // POPULATION_H