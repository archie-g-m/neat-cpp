#include "config_parser.h"
#include "population.h"

#include <gtest/gtest.h>

TEST(POPULATIONTEST, ConstructionTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfig.cfg");
    Population p = Population(config);

    ASSERT_EQ(p.population.size(), 40);
}

TEST(POPULATIONTEST, CalcStagnantTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfig.cfg");
    Population p = Population(config);

    int stagnant_sid = 1;
    for (int gen = 0; gen < 20; gen++)
    {
        for (std::pair<const int, Species_ptr> sit : p.species_set->species)
        {
            const int sid = sit.first;
            Species_ptr s = sit.second;
            for (std::pair<const int, Genome_ptr> git : s->members)
            {
                const int gid = git.first;
                Genome_ptr g = git.second;
                if (sid != stagnant_sid)
                {
                    g->fitness += 1;
                }
            }
        }
        std::vector<int> stagnant_species = p.get_stagnant_species(gen);
        if (gen < 15)
        {
            ASSERT_EQ(stagnant_species.size(), 0);
        }
        else
        {
            ASSERT_EQ(stagnant_species.size(), 1);
            ASSERT_EQ(stagnant_species[0], 1);
        }
    }
}

TEST(POPULATIONTEST, CalcStagnantTest2)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfig.cfg");
    Population p = Population(config);

    int stagnant_sid = 1;
    for (int gen = 0; gen < 20; gen++)
    {
        std::vector<int> stagnant_species = p.get_stagnant_species(gen);
        if (gen < 15)
        {
            ASSERT_EQ(stagnant_species.size(), 0);
        }
        else
        {
            ASSERT_EQ(stagnant_species.size(), 38);
            for (int i = 1; i <= 38; i++)
            {
                ASSERT_EQ(stagnant_species[i - 1], i);
            }
        }
    }
}

TEST(POPULATIONTEST, CalcSpawnsTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfig.cfg");
    Population p = Population(config);

    std::map<int, float> adj_fitnesses = {};
    std::map<int, int> prev_sizes = {};
    std::map<int, int> spawns = {};

    adj_fitnesses[0] = 1.F;
    adj_fitnesses[1] = 0.F;

    prev_sizes[0] = 20;
    prev_sizes[1] = 20;

    spawns = p.calc_spawns(adj_fitnesses, prev_sizes);
    ASSERT_EQ(spawns[0], 27);
    ASSERT_EQ(spawns[1], 13);

    spawns = p.calc_spawns(adj_fitnesses, spawns);
    ASSERT_EQ(spawns[0], 30);
    ASSERT_EQ(spawns[1], 10);

    spawns = p.calc_spawns(adj_fitnesses, spawns);
    ASSERT_EQ(spawns[0], 31);
    ASSERT_EQ(spawns[1], 10);

    spawns = p.calc_spawns(adj_fitnesses, spawns);
    ASSERT_EQ(spawns[0], 31);
    ASSERT_EQ(spawns[1], 10);
}

TEST(POPULATIONTEST, CalcSpawnsTest2)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfig.cfg");
    Population p = Population(config);

    std::map<int, float> adj_fitnesses = {};
    std::map<int, int> prev_sizes = {};
    std::map<int, int> spawns = {};

    adj_fitnesses[0] = 0.5F;
    adj_fitnesses[1] = 0.5F;

    prev_sizes[0] = 20;
    prev_sizes[1] = 20;

    spawns = p.calc_spawns(adj_fitnesses, prev_sizes);
    ASSERT_EQ(spawns[0], 20);
    ASSERT_EQ(spawns[1], 20);
}

TEST(POPULATIONTEST, CalcSpawnsTest3)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfig.cfg");
    Population p = Population(config);

    std::map<int, float> adj_fitnesses = {};
    std::map<int, int> prev_sizes = {};
    std::map<int, int> spawns = {};

    adj_fitnesses[0] = 0.5F;
    adj_fitnesses[1] = 0.5F;

    prev_sizes[0] = 30;
    prev_sizes[1] = 10;

    spawns = p.calc_spawns(adj_fitnesses, prev_sizes);
    ASSERT_EQ(spawns[0], 25);
    ASSERT_EQ(spawns[1], 15);

    spawns = p.calc_spawns(adj_fitnesses, spawns);
    ASSERT_EQ(spawns[0], 22);
    ASSERT_EQ(spawns[1], 18);

    spawns = p.calc_spawns(adj_fitnesses, spawns);
    ASSERT_EQ(spawns[0], 21);
    ASSERT_EQ(spawns[1], 19);

    spawns = p.calc_spawns(adj_fitnesses, spawns);
    ASSERT_EQ(spawns[0], 20);
    ASSERT_EQ(spawns[1], 20);

    spawns = p.calc_spawns(adj_fitnesses, spawns);
    ASSERT_EQ(spawns[0], 20);
    ASSERT_EQ(spawns[1], 20);
}

TEST(POPULATIONTEST, ReproductionTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfig.cfg");
    Population p = Population(config);

    p.species_set->species.clear();
    p.species_set->num_species = 1;

    // Simplify the population down to 3 species
    int num_species = 4;
    for (int gid = 1; gid <= p.population.size(); gid++)
    {
        if ((gid % int(p.population.size() / num_species)) - 1)
        {
            int parent_id = gid - ((gid - 1) % int(p.population.size() / num_species));
            p.population[gid] = std::make_shared<Genome>(gid, p.population[parent_id], p.population[parent_id], config);
        }
    }

    p.species_set->speciate(p.population, 0);

    int stagnant_id = 1;
    for (int generation = 1; generation <= 21; generation++)
    {
        for (std::pair<const int, Species_ptr> sit : p.species_set->species)
        {
            for (std::pair<const int, Genome_ptr> git : sit.second->members)
            {
                if (sit.first == stagnant_id)
                {
                    git.second->fitness = 0;
                }
                else
                {
                    git.second->fitness = sit.first;
                }
            }
        }

        p.population = p.reproduce(generation);

        p.species_set->speciate(p.population, generation);
    }

    ASSERT_EQ(p.species_set->species.size(), 2);
    ASSERT_EQ(p.species_set->species[3]->members.size(), 10);
    ASSERT_EQ(p.species_set->species[4]->members.size(), 31);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}