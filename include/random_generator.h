#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
#include <random>

int seed = 8675309;
std::default_random_engine generator(seed);

#endif // RANDOM_GENERATOR_H
