#include "attributes.h"
#include <gtest/gtest.h>

TEST(FLOATATTR, ConstructionTest){
    ASSERT_THROW(FloatAttribute("bad_attr", 0.5F, 3.0F, 10.F, -10.F), std::invalid_argument);
}

TEST(FLOATATTR, MutateTest){
  FloatAttribute test = FloatAttribute("test1", 1.0F, 100.0F, -10.F, 10.F);
  test.mutate_value();
  ASSERT_TRUE(test.get_float_value() <= 10.F);
  ASSERT_TRUE(test.get_float_value() >= -10.F);
}

TEST(FLOATATTR, CopyTest){
  FloatAttribute* test = new FloatAttribute("test2", 0.3F, 3.0F, -10.F, 10.F);
  FloatAttribute* test2 = test->copy();
  ASSERT_EQ(test->get_float_value(), test2->get_float_value());
  ASSERT_EQ(test->get_mutate_power(), test2->get_mutate_power());
  ASSERT_EQ(test->get_min_value(), test2->get_min_value());
  ASSERT_EQ(test->get_max_value(), test2->get_max_value());
}

TEST(FLOATATTR, ConvTest){
  FloatAttribute test = FloatAttribute("test1", 1.0F, 100.0F, -10.F, 10.F);
  ASSERT_TRUE(test.get_bool_value() == true);
  ASSERT_TRUE(test.get_float_value() == static_cast<float>(test.get_float_value()));
}

TEST(INTATTR, ConstructionTest){
    ASSERT_THROW(IntAttribute("bad_attr", 0.5F, 3.0F, 10, -10), std::invalid_argument);
}

TEST(INTATTR, MutateTest){
  IntAttribute test = IntAttribute("test1", 1.0F, 100.0F, -10, 10);
  test.mutate_value();
  ASSERT_TRUE(test.get_float_value() <= 10.F);
  ASSERT_TRUE(test.get_float_value() >= -10.F);
}

TEST(INTATTR, CopyTest){
  IntAttribute* test = new IntAttribute("test2", 0.3F, 3.0F, -10, 10);
  IntAttribute* test2 = test->copy();
  ASSERT_EQ(test->get_float_value(), test2->get_float_value());
  ASSERT_EQ(test->get_mutate_power(), test2->get_mutate_power());
  ASSERT_EQ(test->get_min_value(), test2->get_min_value());
  ASSERT_EQ(test->get_max_value(), test2->get_max_value());
}

TEST(INTATTR, ConvTest){
  IntAttribute test = IntAttribute("test1", 1.0F, 100.0F, -10, 10);
  ASSERT_TRUE(test.get_bool_value() == true);
  ASSERT_TRUE(test.get_float_value() == static_cast<float>(test.get_float_value()));
}

TEST(BOOLATTR, CopyTest){
  IntAttribute* test = new IntAttribute("test2", 0.3F, 3.0F, -10, 10);
  IntAttribute* test2 = test->copy();
  ASSERT_EQ(test->get_float_value(), test2->get_float_value());
  ASSERT_EQ(test->get_mutate_power(), test2->get_mutate_power());
  ASSERT_EQ(test->get_min_value(), test2->get_min_value());
  ASSERT_EQ(test->get_max_value(), test2->get_max_value());
}

TEST(BOOLATTR, ConvTest){
  IntAttribute test = IntAttribute("test1", 1.0F, 100.0F, -10, 10);
  ASSERT_TRUE(test.get_bool_value() == true);
  ASSERT_TRUE(test.get_float_value() == static_cast<float>(test.get_float_value()));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}