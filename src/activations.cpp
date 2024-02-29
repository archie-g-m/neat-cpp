#include "activations.h"

float activate_value(float x, std::string method)
{
    if (act_map.count(method) == 0)
    {
        throw std::invalid_argument("Invalid Activation '" + method + "' provided");
    }
    switch (act_map[method])
    {
    case (linear_act):
        return linear_activation(x);
    case (sigmoid_act):
        return sigmoid_activation(x);
    case (tanh_act):
        return tanh_activation(x);
    case (sin_act):
        return sin_activation(x);
    case (gauss_act):
        return gauss_activation(x);
    case (relu_act):
        return relu_activation(x);
    case (softplus_act):
        return softplus_activation(x);
    case (clamped_act):
        return clamped_activation(x);
    case (abs_act):
        return abs_activation(x);
    case (square_act):
        return square_activation(x);
    case (cubed_act):
        return cubed_activation(x);
    };
}