#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include <map>
#include <string>
#include <cmath>

enum valid_activations
{
    linear_act,
    sigmoid_act,
    tanh_act,
    sin_act,
    gauss_act,
    relu_act,
    softplus_act,
    clamped_act,
    abs_act,
    square_act,
    cubed_act
};

static std::map<std::string, valid_activations> act_map = {
    {"linear", linear_act},
    {"sigmoid", sigmoid_act},
    {"tanh", tanh_act},
    {"sin", sin_act},
    {"gauss", gauss_act},
    {"relu", relu_act},
    {"softplus", softplus_act},
    {"clamped", clamped_act},
    {"abs", abs_act},
    {"square", square_act},
    {"cubed", cubed_act}
};

inline float linear_activation(float &x) { return x; }

inline float sigmoid_activation(float &x) { return 1.0F / (1.0F + std::expf(-x)); }

inline float tanh_activation(float &x) { return std::tanhf(x); }

inline float sin_activation(float &x) { return std::sinf(x); }

inline float gauss_activation(float &x) { return std::expf(std::powf(x, 2)); }

inline float relu_activation(float &x) { return x > 0 ? x : 0.F; }

inline float softplus_activation(float &x) { return std::log1pf(std::exp(x)); }

inline float clamped_activation(float &x) { return (x > 1.F) ? 1.F : (x < -1.F) ? -1.F : x; }

inline float abs_activation(float &x) { return std::fabs(x); }

inline float square_activation(float &x) { return std::powf(x, 2); }

inline float cubed_activation(float &x) { return std::powf(x, 3); }

float activate_value(float value, std::string method);

#endif // ACTIVATIONS_H
