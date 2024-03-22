#ifndef SPECIES_H
#define SPECIES_H

#include "genome.h"

typedef std::shared_ptr<class SpeciesSetConfig> SpeciesSetConfig_ptr;

class SpeciesSetConfig : SpecialConfig
{
public:
    // Species Config
    float compatibility_threshold;

    SpeciesSetConfig(ConfigParser_ptr _config);
};

typedef std::shared_ptr<class Species> Species_ptr;

class Species
{
public:
    int key;
    int generation_created;
    int generation_last_improved;
    Genome_ptr representative;
    std::map<int, Genome_ptr> members;
    float fitness;
    std::vector<float> fitness_history;

    Species(int key, int generation);
    void update(Genome_ptr rep, std::map<int, Genome_ptr> mem);
    std::vector<float> get_fitnesses();
};

typedef std::shared_ptr<class SpeciesSet> SpeciesSet_ptr;

class SpeciesSet
{
public:
    std::map<int, Species_ptr> species;

#ifdef TEST_MODE
public:
#else
private:
#endif
    SpeciesSetConfig_ptr config;
    std::map<int, int> genome_species_map;
    int num_species;

public:
    SpeciesSet(ConfigParser_ptr _config);
    void speciate(std::map<int, Genome_ptr> &population, int generation);

private:
    float getDistanceFromCache(Genome_ptr &g1, Genome_ptr &g2, std::map<std::pair<int, int>, float> &distance_cache);
};

#endif // SPECIES_H