#include "species.h"
#include <set>
#include <iostream>
SpeciesSetConfig::SpeciesSetConfig(ConfigParser_ptr _config)
{
    // Configure from Parser
    data = _config->get_subdata("DefaultSpeciesSet");

    // Distance Parameters
    compatibility_threshold = get_value<float>("compatibility_threshold");
}

Species::Species(int _key, int _generation)
{
    key = _key;
    generation_created = _generation;
    generation_last_improved = _generation;
    representative = nullptr;
    members = {};
    fitness = 0.0F;
    fitness_history = {};
}

/**
 * @brief Update the species's representative and members to the provided values
 *
 * @param rep new representative
 * @param mem new members
 */
void Species::update(Genome_ptr rep, std::map<int, Genome_ptr> mem)
{
    representative = rep;
    members = mem;
}

/**
 * @brief Get the fitnesses of all members
 *
 * @return std::vector<float>
 */
std::vector<float> Species::get_fitnesses()
{
    std::vector<float> fitness_list = {};
    fitness_list.reserve(members.size());
    for (std::pair<const int, Genome_ptr> &m : members)
    {
        fitness_list.push_back(m.second->fitness);
    }
    return fitness_list;
}

SpeciesSet::SpeciesSet(ConfigParser_ptr _config)
{
    config = std::make_shared<SpeciesSetConfig>(_config);
    species = {};
    genome_species_map = {};
    num_species = 1;
}

/**
 * @brief Get the distance between provided genomes if already calculated,
 *        otherwise calculate the distance and cache it
 *
 * @param g1
 * @param g2
 * @param distance_cache
 * @return float
 */
float SpeciesSet::getDistanceFromCache(Genome_ptr &g1, Genome_ptr &g2, std::map<std::pair<int, int>, float> &distance_cache)
{
    float distance;
    std::pair<int, int> did(g1->key, g2->key);
    if (distance_cache.count(did))
    {
        distance = distance_cache[did];
    }
    else
    {
        distance = g1->distance(g2);
        distance_cache[did] = distance;
        distance_cache[std::pair<int, int>(did.second, did.first)] = distance;
    }
    return distance;
};

/**
 * @brief Speciate the provided population into the current species and create new species when needed
 *
 * @param population population to speciate
 * @param generation current generation
 */
void SpeciesSet::speciate(std::map<int, Genome_ptr> &population, int generation)
{
    std::set<int> unspeciated_ids = {};
    std::transform(population.begin(), population.end(),
                   std::inserter(unspeciated_ids, unspeciated_ids.end()),
                   [](std::pair<int, Genome_ptr> pair)
                   { return pair.first; });
    std::map<std::pair<int, int>, float> distance_cache = {};
    std::map<int, int> new_representatives = {};
    std::map<int, std::vector<int>> new_members = {};

    // 1. Find new representative for every existing species
    for (std::pair<const int, Species_ptr> &sit : species)
    {
        const int sid = sit.first;
        Species_ptr &s = sit.second;
        std::vector<std::pair<int, float>> candidates = {};
        candidates.reserve(unspeciated_ids.size());
        for (int gid : unspeciated_ids)
        {
            Genome_ptr g = population[gid];
            float distance = getDistanceFromCache(s->representative, g, distance_cache);
            candidates.emplace_back(gid, distance);
        }
        // Find the closest Genome to the current representative
        int new_rep = (*std::min_element(candidates.begin(),
                                         candidates.end(),
                                         [](std::pair<int, float> &d1, std::pair<int, float> &d2)
                                         {
                                             return d1.second < d2.second;
                                         }))
                          .first;
        // Set closest genome as representative and add to members
        new_representatives[sid] = new_rep;
        new_members[sid] = {new_rep};
        // Now genome is member it is not unspeciated
        unspeciated_ids.erase(new_rep);
    }

    // 2. Now partition every remaining Genome into a Species
    for (int gid : unspeciated_ids)
    {
        Genome_ptr g = population[gid];

        std::vector<std::pair<int, float>> candidates = {};
        candidates.reserve(new_representatives.size());
        if (new_representatives.size())
        {
            for (std::pair<const int, int> &new_rep_pair : new_representatives)
            {
                int sid = new_rep_pair.first;
                int rid = new_rep_pair.second;
                std::pair<int, int> did(rid, gid);
                float distance = getDistanceFromCache(population[rid], population[gid], distance_cache);
                if (distance < config->compatibility_threshold)
                {
                    candidates.emplace_back(sid, distance);
                }
            }
        }
        if (candidates.size())
        {
            std::pair<int, float> sp = *std::min_element(candidates.begin(),
                                                         candidates.end(),
                                                         [](std::pair<int, float> &d1, std::pair<int, float> &d2)
                                                         {
                                                             return d1.second < d2.second;
                                                         });
            int sid = sp.first;
            float sdist = sp.second;
            new_members[sid].push_back(gid);
        }
        else
        {
            int sid = num_species++;
            new_representatives[sid] = gid;
            new_members[sid] = {gid};
        }
    }

    //  3. Update species based upon aggregation of genomes
    for (std::pair<const int, int> &new_rep_pair : new_representatives)
    {
        int sid = new_rep_pair.first;
        int rid = new_rep_pair.second;
        //  If Species doesnt exist yet. Create it
        if (!species.count(sid))
        {
            Species_ptr new_species = std::make_shared<Species>(sid, generation);
            species[sid] = new_species;
        }

        // Update all Species
        std::map<int, Genome_ptr> member_dict;
        for (int gid : new_members[sid])
        {
            member_dict[gid] = population[gid];
        }
        species[sid]->update(population[rid], member_dict);
    }
}