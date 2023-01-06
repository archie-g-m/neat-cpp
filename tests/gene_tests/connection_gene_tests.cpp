#include <genes.h>
#include <attributes.h>

#include <vector>
#include <gtest/gtest.h>

ConnectionGene *create_Connection1()
{
    std::pair<int, int> key;
    key.first = 1;
    key.second = 2;
    FloatAttribute *weight1 = new FloatAttribute("weight", 0.5F, 3.0F, 1.F, 1.F);
    BoolAttribute *enable1 = new BoolAttribute("enable", 0.5F);
    BoolAttribute *other1 = new BoolAttribute("other", 0.5F);
    std::vector<Attribute *> gene1_attributes;
    gene1_attributes.push_back(weight1);
    gene1_attributes.push_back(enable1);
    gene1_attributes.push_back(other1);
    ConnectionGene *gene1 = new ConnectionGene(key, gene1_attributes);
    return gene1;
}

ConnectionGene *create_Connection2()
{
    std::pair<int, int> key;
    key.first = 1;
    key.second = 2;
    FloatAttribute *weight2 = new FloatAttribute("weight", 0.5F, 3.0F, 10.F, 10.F);
    BoolAttribute *enable2 = new BoolAttribute("enable", 0.5F);
    std::vector<Attribute *> gene2_attributes;
    gene2_attributes.push_back(weight2);
    gene2_attributes.push_back(enable2);
    ConnectionGene *gene2 = new ConnectionGene(key, gene2_attributes);
    return gene2;
}

TEST(CONNECTIONGENE, IncorrectAttributeTest)
{
    std::pair<int, int> key;
    key.first = 1;
    key.second = 2;
    FloatAttribute *bias1 = new FloatAttribute("bias", 0.5F, 3.0F, 1.F, 1.F);
    FloatAttribute *resp1 = new FloatAttribute("response", 0.5F, 3.0F, 2.F, 2.F);
    std::vector<Attribute *> gene1_attributes;
    gene1_attributes.push_back(bias1);
    gene1_attributes.push_back(resp1);
    ASSERT_THROW(ConnectionGene(key, gene1_attributes), std::invalid_argument);
}

TEST(CONNECTIONGENE, ZeroDistanceTest)
{
  // Create First Node
  ConnectionGene *gene1 = create_Connection1();

  // Create Second Node
  ConnectionGene *gene2 = create_Connection1();

  // Check Distance Between Two *almost* identical genes is 0
  ASSERT_EQ(gene1->distance(gene2), 0);

  // Check Distance Between identical genes is 0
  ASSERT_EQ(gene1->distance(gene1), 0);
}

TEST(CONNECTIONGENE, NonZeroDistanceTest)
{
  // Create First Node
  ConnectionGene *gene1 = create_Connection1();

  // Create Second Node
  ConnectionGene *gene2 = create_Connection2();

  // Check distance between genes is calculated correctly and is positive
  ASSERT_EQ(gene1->distance(gene2), 9);
  ASSERT_TRUE(gene1->distance(gene2) >= 0);

  ASSERT_EQ(gene2->distance(gene1), 9);
  ASSERT_TRUE(gene2->distance(gene1) >= 0);
}

TEST(CONNECTIONGENE, CrossoverTest)
{
  // Create First Node
  ConnectionGene *gene1 = create_Connection1();

  // Create Second Node
  ConnectionGene *gene2 = create_Connection2();

  // Crosover Two Nodes
  ConnectionGene *gene3 = gene1->crossover(gene2);

  // Check that crosover node contains attributed from either parent gene
  ASSERT_TRUE((gene3->get_attribute("weight")->get_float_value() == 1.F) ||
              (gene3->get_attribute("weight")->get_float_value() == 10.F));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}