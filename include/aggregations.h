#ifndef AGGREGATIONS_H
#define AGGREGATIONS_H

#include <string>
#include <map>
#include <vector>
#include <numeric>

enum class valid_aggregations
{
    sum,
    mean,
    max,
    min,
    median
};

static std::map<std::string, valid_aggregations> agg_map = {
    {"sum", valid_aggregations::sum},
    {"mean", valid_aggregations::mean},
    {"max", valid_aggregations::max},
    {"min", valid_aggregations::min},
    {"median", valid_aggregations::median}};

inline float sum_aggregate(std::vector<float> &values) { return std::accumulate(values.begin(), values.end(), 0); }

inline float mean_aggregate(std::vector<float> &values) { return std::accumulate(values.begin(), values.end(), 0) / (float)values.size(); }

inline float max_aggregate(std::vector<float> &values) { return *std::max_element(values.begin(), values.end()); }

inline float min_aggregate(std::vector<float> &values) { return *std::min_element(values.begin(), values.end()); }

inline float median_aggregate(std::vector<float> &values)
{
    std::sort(values.begin(), values.end());
    int mid_ind = values.size() / 2;
    if (values.size() % 2 == 0)
    {
        return (values[mid_ind - 1] + values[mid_ind]) * 0.5F;
    }
    else
    {
        return values[mid_ind];
    }
}

float aggregate_vector(std::vector<float> values, std::string method);

#endif // AGGREGATIONS_H
