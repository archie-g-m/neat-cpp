#include "population.h"
#include "aggregations.h"

#include <limits>
#include <iostream>

PopulationConfig::PopulationConfig(ConfigParser_ptr _config)
{
    // Configure from Parser
    data = _config->get_subdata("NEAT");

    // Population Parameters
    fitness_criterion = get_value<std::string>("fitness_criterion");
    fitness_threshold = get_value<float>("fitness_threshold");
    pop_size = get_value<int>("pop_size");
    reset_on_extinction = get_value<bool>("reset_on_extinction");
    no_fitness_termination = get_value<bool>("no_fitness_termination");

    // Configure from Parser
    data = _config->get_subdata("DefaultStagnation");

    species_fitness_func = get_value<std::string>("species_fitness_func");
    max_stagnation = get_value<int>("max_stagnation");
    species_elitism = get_value<int>("species_elitism");

    // Configure from Parser
    data = _config->get_subdata("DefaultReproduction");

    elitism = get_value<int>("elitism");
    survival_threshold = get_value<float>("survival_threshold");
    min_species_size = get_value<int>("min_species_size");
}

Population::Population(ConfigParser_ptr _config)
{
    raw_config = _config;
    config = std::make_shared<PopulationConfig>(_config);
    species_set = std::make_shared<SpeciesSet>(_config);
    total_genomes = 1;
    population = new_population(_config, config->pop_size);
    species_set->speciate(population, 0);
}

/**
 * @brief Runs the NEAT algorithm against the provided fitness function for n generations
 *
 * @param fitness_function fitness function to run genomes against
 * @param n Number of generations to run the experiment for
 * @return Genome_ptr Best Genome found
 */
Genome_ptr Population::run(std::function<float(Genome_ptr)> fitness_function, int n)
{
    float best_fitness = std::numeric_limits<float>::min();
    Genome_ptr best;
    for (int gen = 0; gen != n; gen++)
    {
        // Run fitness function on each genome
        float gen_best_fitness = std::numeric_limits<float>::min();
        std::vector<float> fitnesses = {};
        Genome_ptr gen_best;

        for (std::pair<const int, Genome_ptr> git : population)
        {
            int gid = git.first;
            Genome_ptr g = git.second;

            g->activate();
            g->fitness = fitness_function(g);
            if (g->fitness > gen_best_fitness)
            {
                gen_best_fitness = g->fitness;
                best = g;
            }
            fitnesses.push_back(g->fitness);
        }

        if (gen_best_fitness > best_fitness)
        {
            best_fitness = gen_best_fitness;
            best = gen_best;
        }

        if (!config->no_fitness_termination)
        {
            float fitness_value = aggregate_vector(fitnesses, config->fitness_criterion);
            if (fitness_value >= config->fitness_threshold)
            {
                std::cout << "Fitness Criterion Reached!!" << std::endl;
                break;
            }
        }

        population = reproduce(gen);
        // If we have reached complete extinction
        if (species_set->species.empty() && gen > 0)
        {
            if (config->reset_on_extinction)
            {
                population = new_population(raw_config, config->pop_size);
            }
            else
            {
                // TODO: Log extinction
                std::cout << "No more species, ending" << std::endl;
                break;
            }
        }

        // Speciate the updated
        species_set->speciate(population, gen);
        std::vector<float> distances = {};
        for (std::pair<const int, Genome_ptr> git1 : population)
        {
            const int gid1 = git1.first;
            Genome_ptr g1 = git1.second;
            for (std::pair<const int, Genome_ptr> git2 : population)
            {
                const int gid2 = git2.first;
                Genome_ptr g2 = git2.second;
                if (gid1 != gid2)
                {
                    distances.push_back(g1->distance(g2));
                }
            }
        }

        float sum = std::accumulate(distances.begin(), distances.end(), 0.0);
        float mean = sum / distances.size();

        std::vector<float> diff(distances.size());
        std::transform(distances.begin(), distances.end(), diff.begin(), [mean](float x)
                       { return x - mean; });

        float sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        float stdev = std::sqrt(sq_sum / distances.size());

        std::cout << "Generation: " << gen << std::endl;
        std::cout << "  Population of " << population.size() << " in " << species_set->species.size() << " species:" << std::endl;
        std::cout << "  Mean genetic distance " << mean << ", standard deviation " << stdev << std::endl;
        std::cout << "  ID\tage\tsize\tfitness\tstag" << std::endl;
        std::cout << "  ==\t===\t====\t=======\t====" << std::endl;
        for (std::pair<const int, Species_ptr> sit : species_set->species)
        {
            const int sid = sit.first;
            Species_ptr s = sit.second;
            std::cout << "  " << sid << "\t" << gen - s->generation_created << "\t" << s->members.size() << "\t" << s->fitness << "\t" << gen - s->generation_last_improved << "\t" << std::endl;
        }
    }

    return best;
}

/**
 * @brief Generate a new population of Genomes based on the provided config
 *
 * TODO: Does pop_size need to be passed if its present in the config file?
 *
 * @param _config configuration of genomes
 * @param pop_size population size to generate
 * @return std::map<int, Genome_ptr>
 */
std::map<int, Genome_ptr> Population::new_population(ConfigParser_ptr _config, int pop_size)
{
    std::map<int, Genome_ptr> pop;
    for (int i = 0; i < pop_size; i++)
    {
        Genome_ptr g = std::make_shared<Genome>(total_genomes++, _config);
        pop[g->key] = g;
    }
    return pop;
}

/**
 * @brief Find all stagnant species based upon their current member's fitnesses
 * 
 * @param generation current generation
 * @return std::vector<int> Vector of species ID's that are stagnant
 */
std::vector<int> Population::get_stagnant_species(int generation)
{
    // 1. Update the fitness and generation last improved of every species
    std::vector<std::pair<int, Species_ptr>> species_data = {};
    for (std::pair<const int, Species_ptr> &sit : species_set->species)
    {
        const int sid = sit.first;
        Species_ptr s = sit.second;

        float prev_fitness = s->fitness_history.size() ? *std::max_element(s->fitness_history.begin(), s->fitness_history.end()) : -std::numeric_limits<float>::max();
        s->fitness = aggregate_vector(s->get_fitnesses(), config->species_fitness_func);
        s->fitness_history.push_back(s->fitness);
        // std::cout << "Species: " << sid << " pf: " << prev_fitness << " fit: " << s->fitness << " gen: " << generation << std::endl;
        if (s->fitness > prev_fitness)
        {
            // std::cout << "Species " << sid << " improved on Generation " << generation << std::endl;
            s->generation_last_improved = generation;
        }
        species_data.emplace_back(sid, s);
    }

    // 2. Sort species by their fitness
    std::sort(species_data.begin(),
              species_data.end(),
              [](std::pair<int, Species_ptr> &s1, std::pair<int, Species_ptr> &s2)
              { return s1.second->fitness == s2.second->fitness ? s1.first < s2.first : s1.second->fitness < s2.second->fitness; });

    // for (const std::pair<int, Species_ptr> &sit : species_data)
    // {
    //     std::cout << sit.first << ": Fitness = " << sit.second->fitness << std::endl;
    // }
    // 3. Mark species to be removed by their time since they were last improved
    int num_active = species_data.size();
    int it = 0;
    std::vector<int> stagnant_sid = {};
    for (std::pair<int, Species_ptr> &sit : species_data)
    {
        const int sid = sit.first;
        Species_ptr s = sit.second;
        int stagnant_time = generation - s->generation_last_improved;
        bool is_stagnant = false;
        if (num_active > config->species_elitism)
        {
            is_stagnant = (stagnant_time >= config->max_stagnation);
        }

        if ((species_data.size() - it) <= config->species_elitism)
        {
            is_stagnant = false;
        }

        if (is_stagnant)
        {
            num_active -= 1;
            stagnant_sid.push_back(sid);
        }

        it++;
    }

    return stagnant_sid;
}

/**
 * @brief Calculate the number of spawns that each species needs based upon their normalized fitnesses and previous sizes
 * 
 * @param adj_fitnesses map of species ids to adjusted fitnesses
 * @param prev_sizes map of species ids to their previous sizes
 * @return std::map<int, int> map of species ids to the desired new sizes
 */
std::map<int, int> Population::calc_spawns(std::map<int, float> adj_fitnesses, std::map<int, int> prev_sizes)
{
    float af_sum = std::accumulate(adj_fitnesses.begin(),
                                   adj_fitnesses.end(), 0.0F,
                                   [](float acc, std::pair<int, float> adj)
                                   { return acc + adj.second; });
    float total_spawns = 0.0F;
    std::map<int, float> raw_spawn_amounts = {};
    for (std::pair<const int, float> &adit : adj_fitnesses)
    {
        int sid = adit.first;
        float adj = adit.second;
        int prev = prev_sizes[sid];

        // Calculate how many members should be in each species based on their portion of the overall normalized fitness
        int s;
        if (af_sum > 0)
        {
            s = std::max(static_cast<float>(config->min_species_size),
                         adj / af_sum * static_cast<float>(config->pop_size));
        }
        else
        {
            s = config->min_species_size;
        }

        float d = (s - prev) * 0.5F;
        int c = std::round(d);
        float new_pop = prev;
        if (std::abs(c) > 0)
        {
            new_pop += c;
        }
        else if (d > 0)
        {
            new_pop += 1;
        }
        else if (d < 0)
        {
            new_pop -= 1;
        }

        total_spawns += new_pop;
        raw_spawn_amounts[sid] = new_pop;
    }

    float norm = static_cast<float>(config->pop_size) / total_spawns;

    std::map<int, int> new_pops = {};
    for (std::pair<const int, float> &sa : raw_spawn_amounts)
    {
        const int sid = sa.first;
        float raw_new_pop = sa.second;
        new_pops[sid] = std::max(config->min_species_size, int(std::round(raw_new_pop * norm)));
    }

    return new_pops;
}

/**
 * @brief Reproduce the current population based on their current fitnesses and the provieded generation
 * 
 * @param generation current generation
 * @return std::map<int, Genome_ptr> map of new population
 */
std::map<int, Genome_ptr> Population::reproduce(int generation)
{
    // 1. Prune Stagnant Species
    std::vector<int> stagnant_species = get_stagnant_species(generation);

    for (int stag_id : stagnant_species)
    {
        species_set->species.erase(stag_id);
    }

    std::vector<float> fitnesses = {};
    fitnesses.reserve(population.size());
    std::vector<int> remaining_species = {};
    remaining_species.reserve(species_set->species.size());

    for (std::pair<const int, Species_ptr> &sit : species_set->species)
    {
        const int sid = sit.first;
        Species_ptr s = sit.second;
        remaining_species.push_back(sid);
        for (std::pair<const int, Genome_ptr> &git : s->members)
        {
            fitnesses.push_back(git.second->fitness);
        }
    }

    // 2. Find Adjusted Fitness and Previous Sizes of Current Species
    float min_fitness = *std::min_element(fitnesses.begin(),
                                          fitnesses.end());

    float max_fitness = *std::max_element(fitnesses.begin(),
                                          fitnesses.end());

    float fitness_range = std::max(1.0F, max_fitness - min_fitness);
    std::map<int, float> adujusted_fitness_map = {};
    std::map<int, int> current_sizes = {};
    for (int sid : remaining_species)
    {
        Species_ptr s = species_set->species[sid];

        float mean_fitness = std::accumulate(s->members.begin(),
                                             s->members.end(), 0.0F,
                                             [](float acc, std::pair<const int, Genome_ptr> &g1)
                                             { return acc + g1.second->fitness; }) /
                             s->members.size();
        adujusted_fitness_map[sid] = (mean_fitness - min_fitness) / fitness_range;
        current_sizes[sid] = s->members.size();
    }

    // 3. Calculate Spawns for new species
    std::map<int, int> new_sizes = calc_spawns(adujusted_fitness_map, current_sizes);

    // 4. Reprocuce the Current Population based on desired spawns
    std::map<int, Genome_ptr> new_population = {};
    for (std::pair<const int, int> &it : new_sizes)
    {
        const int sid = it.first;
        int target_size = it.second;
        int current_size = current_sizes[sid];
        Species_ptr s = species_set->species[sid];

        // Cache all the old memebrs from this species
        std::vector<Genome_ptr> old_members;
        old_members.reserve(s->members.size());
        for (std::pair<const int, Genome_ptr> &git : s->members)
        {
            old_members.push_back(git.second);
        }

        // Sort the old members by their fitness
        std::sort(old_members.begin(),
                  old_members.end(),
                  [](Genome_ptr &g1, Genome_ptr &g2)
                  { return g1->fitness > g2->fitness; });

        // Only keep the specified number of elite members
        for (int i = 0; i < old_members.size(); i++)
        {
            if (i < config->elitism)
            {
                new_population[old_members[i]->key] = old_members[i];
            }
            else
            {
                s->members.erase(old_members[i]->key);
            }
        }

        int repro_cutoff = std::ceil(config->survival_threshold * old_members.size());
        repro_cutoff = std::max(repro_cutoff, 2);
        int num_to_spawn = target_size - s->members.size();
        if (num_to_spawn)
        {
            for (int i = 0; i < num_to_spawn; i++)
            {
                Genome_ptr parent1;
                Genome_ptr parent2;
                if (old_members.size() > 1)
                {
                    parent1 = old_members[rand() % repro_cutoff];
                    parent2 = old_members[rand() % repro_cutoff];
                }
                else
                {
                    parent1 = old_members[0];
                    parent2 = old_members[0];
                }

                int gid = total_genomes++;
                Genome_ptr child = std::make_shared<Genome>(gid, parent1, parent2, raw_config);
                child->mutate();

                new_population[gid] = child;
            }
        }
    }

    return new_population;
}
