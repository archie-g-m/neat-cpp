#include <genes.h>
#include <attributes.h>

#include <vector>
#include <gtest/gtest.h>

std::shared_ptr<ConnectionGene> create_Connection1()
{
  std::pair<int, int> key;
  key.first = 1;
  key.second = 2;
  std::shared_ptr<FloatAttribute> weight1 = std::make_shared<FloatAttribute>("weight", 0.5F, 3.0F, 1.F, 1.F);
  std::shared_ptr<BoolAttribute> enable1 = std::make_shared<BoolAttribute>("enable", 0.5F);
  std::shared_ptr<BoolAttribute> other1 = std::make_shared<BoolAttribute>("other", 0.5F);
  std::vector<std::shared_ptr<Attribute>> gene1_attributes;
  gene1_attributes.push_back(weight1);
  gene1_attributes.push_back(enable1);
  gene1_attributes.push_back(other1);
  std::shared_ptr<ConnectionGene> gene1 = std::make_shared<ConnectionGene>(key, gene1_attributes);
  return gene1;
}

std::shared_ptr<ConnectionGene> create_Connection2()
{
  std::pair<int, int> key;
  key.first = 1;
  key.second = 2;
  std::shared_ptr<FloatAttribute> weight2 = std::make_shared<FloatAttribute>("weight", 0.5F, 3.0F, 10.F, 10.F);
  std::shared_ptr<BoolAttribute> enable2 = std::make_shared<BoolAttribute>("enable", 0.5F);
  std::vector<std::shared_ptr<Attribute>> gene2_attributes;
  gene2_attributes.push_back(weight2);
  gene2_attributes.push_back(enable2);
  std::shared_ptr<ConnectionGene> gene2 = std::make_shared<ConnectionGene>(key, gene2_attributes);
  return gene2;
}

TEST(CONNECTIONGENE, IncorrectAttributeTest)
{
  std::pair<int, int> key;
  key.first = 1;
  key.second = 2;
  std::shared_ptr<FloatAttribute> bias1 = std::make_shared<FloatAttribute>("bias", 0.5F, 3.0F, 1.F, 1.F);
  std::shared_ptr<FloatAttribute> resp1 = std::make_shared<FloatAttribute>("response", 0.5F, 3.0F, 2.F, 2.F);
  std::vector<std::shared_ptr<Attribute>> gene1_attributes;
  gene1_attributes.push_back(bias1);
  gene1_attributes.push_back(resp1);
  ASSERT_THROW(ConnectionGene(key, gene1_attributes), std::invalid_argument);
}

TEST(CONNECTIONGENE, ZeroDistanceTest)
{
  // Create First Node
  std::shared_ptr<ConnectionGene> gene1 = create_Connection1();

  // Create Second Node
  std::shared_ptr<ConnectionGene> gene2 = create_Connection1();

  // Check Distance Between Two *almost* identical genes is 0
  ASSERT_EQ(gene1->distance(gene2), 0);

  // Check Distance Between identical genes is 0
  ASSERT_EQ(gene1->distance(gene1), 0);
}

TEST(CONNECTIONGENE, NonZeroDistanceTest)
{
  // Create First Node
  std::shared_ptr<ConnectionGene> gene1 = create_Connection1();

  // Create Second Node
  std::shared_ptr<ConnectionGene> gene2 = create_Connection2();

  // Check distance between genes is calculated correctly and is positive
  ASSERT_EQ(gene1->distance(gene2), 9);
  ASSERT_TRUE(gene1->distance(gene2) >= 0);

  ASSERT_EQ(gene2->distance(gene1), 9);
  ASSERT_TRUE(gene2->distance(gene1) >= 0);
}

TEST(CONNECTIONGENE, CrossoverTest)
{
  // Create First Node
  std::shared_ptr<ConnectionGene> gene1 = create_Connection1();

  // Create Second Node
  std::shared_ptr<ConnectionGene> gene2 = create_Connection2();

  // Crosover Two Nodes
  std::shared_ptr<ConnectionGene> gene3 = gene1->crossover(gene2);

  // Check that crosover node contains attributed from either parent gene
  ASSERT_TRUE((gene3->get_attribute("weight")->get_float_value() == 1.F) ||
              (gene3->get_attribute("weight")->get_float_value() == 10.F));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}