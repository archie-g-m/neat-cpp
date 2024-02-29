#include "genes.h"
#include "attributes.h"

#include <vector>
#include <gtest/gtest.h>

std::shared_ptr<NodeGene> create_Node1()
{
  std::shared_ptr<FloatAttribute> bias1 = std::make_shared<FloatAttribute>("bias", 0.5F, 3.0F, 1.F, 1.F);
  std::shared_ptr<FloatAttribute> resp1 = std::make_shared<FloatAttribute>("response", 0.5F, 3.0F, 2.F, 2.F);
  std::set<std::string> act_options;
  act_options.insert("relu");
  std::shared_ptr<StringAttribute> act1 = std::make_shared<StringAttribute>("activation", 0.5F, act_options);
  std::set<std::string> agr_options;
  agr_options.insert("sum");
  std::shared_ptr<StringAttribute> agr1 = std::make_shared<StringAttribute>("aggregation", 0.5F, agr_options);
  std::shared_ptr<BoolAttribute> other1 = std::make_shared<BoolAttribute>("other", 0.5F);
  std::vector<std::shared_ptr<Attribute>> gene1_attributes;
  gene1_attributes.push_back(bias1);
  gene1_attributes.push_back(resp1);
  gene1_attributes.push_back(act1);
  gene1_attributes.push_back(agr1);
  gene1_attributes.push_back(other1);
  std::shared_ptr<NodeGene> gene1 = std::make_shared<NodeGene>(1, gene1_attributes);
  return gene1;
}

std::shared_ptr<NodeGene> create_Node2()
{
  std::shared_ptr<FloatAttribute> bias2 = std::make_shared<FloatAttribute>("bias", 0.5F, 3.0F, 10.F, 10.F);
  std::shared_ptr<FloatAttribute> resp2 = std::make_shared<FloatAttribute>("response", 0.5F, 3.0F, 20.F, 20.F);
  std::set<std::string> act_options;
  act_options.insert("linear");
  std::shared_ptr<StringAttribute> act2 = std::make_shared<StringAttribute>("activation", 0.5F, act_options);
  std::set<std::string> agr_options;
  agr_options.insert("max");
  std::shared_ptr<StringAttribute> agr2 = std::make_shared<StringAttribute>("aggregation", 0.5F, agr_options);
  std::vector<std::shared_ptr<Attribute>> gene2_attributes;
  gene2_attributes.push_back(bias2);
  gene2_attributes.push_back(resp2);
  gene2_attributes.push_back(act2);
  gene2_attributes.push_back(agr2);
  std::shared_ptr<NodeGene> gene2 = std::make_shared<NodeGene>(1, gene2_attributes);
  return gene2;
}

std::shared_ptr<NodeGene> create_MutableNode3()
{
  std::shared_ptr<FloatAttribute> bias2 = std::make_shared<FloatAttribute>("bias", 0.5F, 3.0F, -10.F, 10.F);
  std::shared_ptr<FloatAttribute> resp2 = std::make_shared<FloatAttribute>("response", 0.5F, 3.0F, -20.F, 20.F);
  std::set<std::string> act_options;
  act_options.insert("relu");
  act_options.insert("linear");
  std::shared_ptr<StringAttribute> act2 = std::make_shared<StringAttribute>("activation", 1.0F, act_options);
  std::set<std::string> agr_options;
  agr_options.insert("max");
  agr_options.insert("sum");
  std::shared_ptr<StringAttribute> agr2 = std::make_shared<StringAttribute>("aggregation", 1.0F, agr_options);
  std::vector<std::shared_ptr<Attribute>> gene2_attributes;
  gene2_attributes.push_back(bias2);
  gene2_attributes.push_back(resp2);
  gene2_attributes.push_back(act2);
  gene2_attributes.push_back(agr2);
  std::shared_ptr<NodeGene> gene2 = std::make_shared<NodeGene>(1, gene2_attributes);
  return gene2;
}

TEST(GENE, HasAttributeTest)
{
  // Create New Gene
  std::shared_ptr<Gene> gene1 = create_Node1();

  // Check has_attribute returns true for all attributes the gene has and false for all others
  ASSERT_TRUE(gene1->has_attribute("bias"));
  ASSERT_TRUE(gene1->has_attribute("response"));
  ASSERT_TRUE(gene1->has_attribute("activation"));
  ASSERT_TRUE(gene1->has_attribute("aggregation"));
  ASSERT_TRUE(gene1->has_attribute("other"));
  ASSERT_FALSE(gene1->has_attribute("notbias"));
}

TEST(GENE, GetAttributeTest)
{
  // Create New Gene
  std::shared_ptr<Gene> gene1 = create_Node1();

  // Check has_attribute returns true for all attributes the gene has and false for all others
  ASSERT_EQ(gene1->get_attribute("bias")->get_float_value(), 1.F);
  ASSERT_EQ(gene1->get_attribute("response")->get_float_value(), 2);
  ASSERT_TRUE(gene1->get_attribute("activation")->get_string_value() == "relu");
  ASSERT_TRUE(gene1->get_attribute("aggregation")->get_string_value() == "sum");
}

TEST(GENE, MutateTest)
{
  std::shared_ptr<Gene> gene1 = create_MutableNode3();
  std::shared_ptr<Gene> gene1_copy = create_MutableNode3();
  gene1->mutate();
  ASSERT_TRUE(gene1->get_attribute("bias")->get_float_value() <= 10.F);
  ASSERT_TRUE(gene1->get_attribute("bias")->get_float_value() >= -10.F);
  ASSERT_TRUE(gene1->get_attribute("bias")->get_float_value() != gene1_copy->get_attribute("bias")->get_float_value());

  ASSERT_TRUE(gene1->get_attribute("response")->get_float_value() <= 20);
  ASSERT_TRUE(gene1->get_attribute("response")->get_float_value() >= -20);
  ASSERT_TRUE(gene1->get_attribute("response")->get_float_value() != gene1_copy->get_attribute("response")->get_float_value());
}

TEST(NODEGENE, IncorrectAttributeTest)
{
  std::shared_ptr<FloatAttribute> bias1 = std::make_shared<FloatAttribute>("notbias", 0.5F, 3.0F, 1.F, 1.F);
  std::shared_ptr<FloatAttribute> resp1 = std::make_shared<FloatAttribute>("notresponse", 0.5F, 3.0F, 1.F, 1.F);
  std::vector<std::shared_ptr<Attribute>> gene1_attributes;
  gene1_attributes.push_back(bias1);
  gene1_attributes.push_back(resp1);
  ASSERT_THROW(NodeGene gene1(1, gene1_attributes), std::invalid_argument);
}

TEST(NODEGENE, ZeroDistanceTest)
{
  // Create First Node
  std::shared_ptr<NodeGene> gene1 = create_Node1();

  // Create Second Node
  std::shared_ptr<NodeGene> gene2 = create_Node1();

  // Check Distance Between Two *almost* identical genes is 0
  ASSERT_EQ(gene1->distance(gene2), 0);

  // Check Distance Between identical genes is 0
  ASSERT_EQ(gene1->distance(gene1), 0);
}

TEST(NODEGENE, NonZeroDistanceTest)
{
  // Create First Node
  std::shared_ptr<NodeGene> gene1 = create_Node1();

  // Create Second Node
  std::shared_ptr<NodeGene> gene2 = create_Node2();

  // Check distance between genes is calculated correctly and is positive
  ASSERT_EQ(gene1->distance(gene2), 29);
  ASSERT_TRUE(gene1->distance(gene2) >= 0);

  ASSERT_EQ(gene2->distance(gene1), 29);
  ASSERT_TRUE(gene2->distance(gene1) >= 0);
}

TEST(NODEGENE, CrossoverTest)
{
  // Create First Node
  std::shared_ptr<NodeGene> gene1 = create_Node1();

  // Create Second Node
  std::shared_ptr<NodeGene> gene2 = create_Node2();

  // Crosover Two Nodes
  std::shared_ptr<NodeGene> gene3 = gene1->crossover(gene2);

  // Check that crosover node contains attributed from either parent gene
  ASSERT_TRUE((gene3->get_attribute("bias")->get_float_value() == 1.F) ||
              (gene3->get_attribute("bias")->get_float_value() == 10.F));
  ASSERT_TRUE((gene3->get_attribute("response")->get_float_value() == 2.F) ||
              (gene3->get_attribute("response")->get_float_value() == 20.F));
  ASSERT_TRUE((gene3->get_attribute("activation")->get_string_value() == "linear") ||
              (gene3->get_attribute("activation")->get_string_value() == "relu"));
  ASSERT_TRUE((gene3->get_attribute("aggregation")->get_string_value() == "sum") ||
              (gene3->get_attribute("aggregation")->get_string_value() == "max"));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}