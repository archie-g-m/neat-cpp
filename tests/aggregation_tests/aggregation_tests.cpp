#include "aggregations.h"
#include <gtest/gtest.h>

std::vector<float> sample_vector1 = {1, 2, 3, 4, 5};
std::vector<float> sample_vector2 = {1, 2, 3, 4, 5, 6};
std::vector<float> sample_vector3 = {-3, -1, 0, -2, -4};

TEST(AGGREGATIONS, InvalidMethod){
    ASSERT_THROW(aggregate_vector(sample_vector1, "asdf"), std::invalid_argument);
}

TEST(AGGREGATIONS, SumTest){
    ASSERT_EQ(aggregate_vector(sample_vector1, "sum"), 15);
    ASSERT_EQ(aggregate_vector(sample_vector2, "sum"), 21);
}

TEST(AGGREGATIONS, MeanTest){
    ASSERT_EQ(aggregate_vector(sample_vector1, "mean"), 3);
    ASSERT_EQ(aggregate_vector(sample_vector2, "mean"), 3.5);
    ASSERT_EQ(aggregate_vector(sample_vector3, "mean"), -2);
}

TEST(AGGREGATIONS, MaxTest){
    ASSERT_EQ(aggregate_vector(sample_vector1, "max"), 5);
    ASSERT_EQ(aggregate_vector(sample_vector2, "max"), 6);
    ASSERT_EQ(aggregate_vector(sample_vector3, "max"), 0);
}

TEST(AGGREGATIONS, MinTest){
    ASSERT_EQ(aggregate_vector(sample_vector1, "min"), 1);
    ASSERT_EQ(aggregate_vector(sample_vector2, "min"), 1);
    ASSERT_EQ(aggregate_vector(sample_vector3, "min"), -4);
}

TEST(AGGREGATIONS, MedianTest){
    ASSERT_EQ(aggregate_vector(sample_vector1, "median"), 3);
    ASSERT_EQ(aggregate_vector(sample_vector2, "median"), 3.5);
    ASSERT_EQ(aggregate_vector(sample_vector3, "median"), -2);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}