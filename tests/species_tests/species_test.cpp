#include "config_parser.h"
#include "species.h"

#include <gtest/gtest.h>

TEST(SPECIESTEST, ConstructionTest)
{
    Species s = Species(1, 1);
    ASSERT_EQ(s.key, 1);
    ASSERT_EQ(s.generation_created, 1);
    ASSERT_EQ(s.generation_last_improved, 1);
    ASSERT_FALSE(s.representative);
    ASSERT_EQ(s.members.size(), 0);
    ASSERT_EQ(s.fitness, 0.0F);
    ASSERT_EQ(s.fitness_history.size(), 0);
}

TEST(SPECIESSET, ConstructionTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfig.cfg");

    SpeciesSet_ptr s = std::make_shared<SpeciesSet>(config);

    ASSERT_EQ(s->species.size(), 0);
}

TEST(SPECIESSET, SpeciateTest)
{
    ConfigParser_ptr config = std::make_shared<ConfigParser>("config/ValidConfig.cfg");
    ConfigParser_ptr genomeConfig1 = std::make_shared<ConfigParser>("config/GenomeConfig1.cfg");
    ConfigParser_ptr genomeConfig2 = std::make_shared<ConfigParser>("config/GenomeConfig2.cfg");
    SpeciesSet_ptr s = std::make_shared<SpeciesSet>(config);

    std::map<int, Genome_ptr> pop;
    // Set up parent of population
    int gid = 0;
    pop[gid] = std::make_shared<Genome>(gid, genomeConfig1);
    gid++;

    for (; gid < 10; gid++)
    {
        pop[gid] = std::make_shared<Genome>(gid, pop[0], pop[0], genomeConfig1);
    }

    pop[gid] = std::make_shared<Genome>(gid, genomeConfig2);
    gid++;

    for (; gid < 20; gid++)
    {
        pop[gid] = std::make_shared<Genome>(gid, pop[10], pop[10], genomeConfig1);
    }

    s->speciate(pop, 0);

    ASSERT_EQ(s->species.size(), 2);
    for (std::pair<const int, Species_ptr> sit : s->species)
    {
        Species_ptr sp = sit.second;
        ASSERT_EQ(sp->members.size(), 10);
        ASSERT_EQ(sp->generation_created, 0);
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}