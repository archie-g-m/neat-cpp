#ifndef AGGREGATIONS_H
#define AGGREGATIONS_H

#include <string>
#include <map>
#include <vector>
#include <numeric>

enum valid_aggregations{
    sum,
    mean,
    max,
    min,
    median
};

static std::map<std::string, valid_aggregations> agg_map = {
    {"sum", sum},
    {"mean", mean},
    {"max", max},
    {"min", min},
    {"median", median}
};

inline float sum_aggregate(std::vector<float> values){return std::accumulate(values.begin(), values.end(), 0);}

inline float mean_aggregate(std::vector<float> values){return std::accumulate(values.begin(), values.end(), 0) / (float)values.size();}

inline float max_aggregate(std::vector<float> values){return *std::max_element(values.begin(), values.end());}

inline float min_aggregate(std::vector<float> values){return *std::min_element(values.begin(), values.end());}

inline float median_aggregate(std::vector<float> values){
    std::sort(values.begin(), values.end());
    if (values.size() % 2 == 0) {
        return (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;
    } else {
        return values[values.size() / 2];
    }
}

float aggregate_vector(std::vector<float> values, std::string method);


#endif //AGGREGATIONS_H
