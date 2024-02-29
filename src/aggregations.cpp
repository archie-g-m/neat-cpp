#include "aggregations.h"

float aggregate_vector(std::vector<float> values, std::string method)
{
    if (agg_map.count(method) == 0)
    {
        throw std::invalid_argument("Invalid Aggregation '" + method + "' Provided");
    }
    switch (agg_map[method])
    {
    case (sum):
        return sum_aggregate(values);
    case (mean):
        return mean_aggregate(values);
    case (max):
        return max_aggregate(values);
    case (min):
        return min_aggregate(values);
    case (median):
        return median_aggregate(values);
    };
}