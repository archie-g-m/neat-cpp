#include "aggregations.h"
#include <stdexcept>

float aggregate_vector(std::vector<float> values, std::string method)
{
    if (agg_map.count(method) == 0)
    {
        throw std::invalid_argument("Invalid Aggregation '" + method + "' Provided");
    }
    switch (agg_map[method])
    {
    case (valid_aggregations::sum):
        return sum_aggregate(values);
    case (valid_aggregations::mean):
        return mean_aggregate(values);
    case (valid_aggregations::max):
        return max_aggregate(values);
    case (valid_aggregations::min):
        return min_aggregate(values);
    case (valid_aggregations::median):
        return median_aggregate(values);
    };
    return 0.0F;
}