#include "attributes.h"
#include <gtest/gtest.h>

// FLOAT ATTRIBUTE
TEST(FLOATATTR, ConstructionTest)
{
  ASSERT_THROW(FloatAttribute("bad_attr", 0.0F, 5.0F, "gauss", 0.5F, 3.0F, 10.F, -10.F), std::invalid_argument);
}

TEST(FLOATATTR, MutateTest)
{
  FloatAttribute test = FloatAttribute("test1", 0.0F, 5.0F, "gauss", 1.0F, 100.0F, -10.F, 10.F);
  test.mutate_value();
  ASSERT_TRUE(test.get_float_value() <= 10.F);
  ASSERT_TRUE(test.get_float_value() >= -10.F);
}

TEST(FLOATATTR, CopyTest)
{
  FloatAttribute_ptr test = std::make_shared<FloatAttribute>("test2", 0.0F, 5.0F, "gauss", 0.3F, 3.0F, -10.F, 10.F);
  FloatAttribute_ptr test2 = std::dynamic_pointer_cast<FloatAttribute>(test->copy());
  ASSERT_EQ(test->get_float_value(), test2->get_float_value());
  ASSERT_EQ(test->get_mutate_power(), test2->get_mutate_power());
  ASSERT_EQ(test->get_min_value(), test2->get_min_value());
  ASSERT_EQ(test->get_max_value(), test2->get_max_value());
}

TEST(FLOATATTR, ConvTest)
{
  FloatAttribute test = FloatAttribute("test1", 5.0F, 2.0F, "gauss", 1.0F, 100.0F, 1.F, 10.F);
  ASSERT_TRUE(test.get_bool_value() == true);
  ASSERT_TRUE(test.get_float_value() == static_cast<float>(test.get_float_value()));
}

// INT ATTRIBUTES
TEST(INTATTR, ConstructionTest)
{
  ASSERT_THROW(IntAttribute("bad_attr", 0.0F, 2.5F, "gauss", 0.5F, 3.0F, 10, -10), std::invalid_argument);
}

TEST(INTATTR, MutateTest)
{
  IntAttribute test = IntAttribute("test1", 0.0F, 2.5F, "gauss", 1.0F, 100.0F, -10, 10);
  test.mutate_value();
  ASSERT_TRUE(test.get_float_value() <= 10.F);
  ASSERT_TRUE(test.get_float_value() >= -10.F);
}

TEST(INTATTR, CopyTest)
{
  IntAttribute_ptr test = std::make_shared<IntAttribute>("test2", 0.0F, 2.5F, "gauss", 0.3F, 3.0F, -10, 10);
  IntAttribute_ptr test2 = std::dynamic_pointer_cast<IntAttribute>(test->copy());
  ASSERT_EQ(test->get_float_value(), test2->get_float_value());
  ASSERT_EQ(test->get_mutate_power(), test2->get_mutate_power());
  ASSERT_EQ(test->get_min_value(), test2->get_min_value());
  ASSERT_EQ(test->get_max_value(), test2->get_max_value());
}

TEST(INTATTR, ConvTest)
{
  IntAttribute test = IntAttribute("test1", 0.0F, 10.F, "gauss", 1.0F, 100.0F, -10, 10);
  ASSERT_TRUE(test.get_bool_value() == true);
  ASSERT_TRUE(test.get_float_value() == static_cast<float>(test.get_float_value()));
  ASSERT_TRUE(test.get_string_value() == std::to_string(test.value));
}

// BOOL ATTRIBUTES
TEST(BOOLATTR, ConstructionTest)
{
  ASSERT_THROW(BoolAttribute("bad_attr", -1), std::invalid_argument);
  ASSERT_THROW(BoolAttribute("bad_attr", 2), std::invalid_argument);
}

TEST(BOOLATTR, MutateTest)
{
  BoolAttribute_ptr test = std::make_shared<BoolAttribute>("test2", 1.0F);
  test->mutate_value();
  ASSERT_TRUE(test->value || !test->value);
}

TEST(BOOLATTR, CopyTest)
{
  BoolAttribute_ptr test = std::make_shared<BoolAttribute>("test2", 0.3F);
  BoolAttribute_ptr test2 = std::dynamic_pointer_cast<BoolAttribute>(test->copy());
  ASSERT_EQ(test->get_bool_value(), test2->get_bool_value());
  ASSERT_EQ(test->get_mutate_rate(), test2->get_mutate_rate());
}

TEST(BOOLATTR, ConvTest)
{
  IntAttribute test = IntAttribute("test1", 5.0F, 2.5F, "gauss", 1.0F, 100.0F, 1, 10);
  ASSERT_TRUE(test.get_bool_value() == static_cast<bool>(test.value));
  ASSERT_TRUE(test.get_float_value() == static_cast<float>(test.get_float_value()));
  ASSERT_TRUE(test.get_string_value() == std::to_string(test.value));
}

// STRING ATTRIBUTES
TEST(STRATTR, ConstructionTest)
{
  std::set<std::string> str_options;
  ASSERT_THROW(StringAttribute("test1", 0.5, str_options), std::invalid_argument);
  str_options.insert("option1");
  ASSERT_THROW(StringAttribute("test2", -1, str_options), std::invalid_argument);
  ASSERT_THROW(StringAttribute("test2", 2, str_options), std::invalid_argument);

  StringAttribute("test3", 0.5, str_options);
}

TEST(STRATTR, MutateTest)
{
  std::set<std::string> str_options;
  str_options.insert("option1");
  str_options.insert("option2");
  str_options.insert("option3");
  str_options.insert("option4");

  StringAttribute_ptr str_attr = std::make_shared<StringAttribute>("test1", 1.0, str_options);
  // Kind pf a bad way to test mutation
  for (int i = 0; i < 10; i++)
  {
    str_attr->mutate_value();
    ASSERT_TRUE(str_options.count(str_attr->value) > 0);
  }
}

TEST(STRATTR, CopyTest)
{
  std::set<std::string> str_options;
  str_options.insert("option1");
  str_options.insert("option2");
  str_options.insert("option3");
  str_options.insert("option4");

  StringAttribute_ptr str_attr = std::make_shared<StringAttribute>("test1", 1.0, str_options);
  StringAttribute_ptr str_attr2 = std::dynamic_pointer_cast<StringAttribute>(str_attr->copy());

  ASSERT_EQ(str_attr->value, str_attr2->value);
  ASSERT_EQ(str_attr->get_mutate_rate(), str_attr2->get_mutate_rate());
  ASSERT_EQ(str_attr->get_options(), str_attr2->get_options());
}

TEST(STRATTR, ConvTest)
{
  std::set<std::string> str_options1;
  str_options1.insert("");
  StringAttribute test1 = StringAttribute("test1", 1.0F, str_options1);

  std::set<std::string> str_options2;
  str_options2.insert("11");
  StringAttribute test2 = StringAttribute("test2", 1.0F, str_options2);

  std::set<std::string> str_options3;
  str_options3.insert("asdf");
  StringAttribute test3 = StringAttribute("test3", 1.0F, str_options3);

  ASSERT_TRUE(test1.get_bool_value() == false);
  ASSERT_TRUE(test2.get_bool_value() == true);
  ASSERT_TRUE(test1.get_float_value() == 0);
  ASSERT_TRUE(test2.get_float_value() == 11);
  ASSERT_TRUE(test3.get_float_value() == 0);
  ASSERT_TRUE(test1.get_string_value() == test1.value);
  ASSERT_TRUE(test2.get_string_value() == test2.value);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}