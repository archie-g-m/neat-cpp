#include "attributes.h"
#include "random_generator.h"
#include <string>
#include <random>
#include <iostream>

/// ------------ BoolAttribute Definitions ------------///

// Constructors
/**
 * @brief Construct a new Bool Attribute:: Bool Attribute object
 *
 * @param _name name of the Attribute
 * @param _mutate_rate probability of mutating the value
 */
BoolAttribute::BoolAttribute(const std::string &_name,
                             const float &_mutate_rate)
{
    name = _name;
    mutate_rate = _mutate_rate;

    validate();

    value = rand_bool(_mutate_rate);
}
/**
 * @brief Construct a new Bool Attribute:: Bool Attribute object
 *
 * @param _name
 * @param _default_value
 * @param _mutate_rate
 */
BoolAttribute::BoolAttribute(const std::string &_name,
                             const bool &_default_value,
                             const float &_mutate_rate)
{
    name = _name;
    mutate_rate = _mutate_rate;
    value = _default_value;

    validate();
}

// Getters
/**
 * @brief Gets the Float Equivalent of the Attribute's Value
 *
 * @return float
 */
float BoolAttribute::get_float_value() { return value ? 1.F : 0.F; }
/**
 * @brief Gets Bool Equivalent of Attribute's Value
 *
 * @return true
 * @return false
 */
bool BoolAttribute::get_bool_value() { return value; }
/**
 * @brief Gets String Equivalent of Attribute's Value
 *
 * @return std::string
 */
std::string BoolAttribute::get_string_value() { return value ? "true" : "false"; }
/**
 * @brief Gets Mutate Rate of Attribute
 *
 * @return float
 */
float BoolAttribute::get_mutate_rate() { return mutate_rate; }

// Other Methods
/**
 * @brief Creates a pointer to a new copy of this Attribute
 *
 * @return BoolAttribute*
 */
Attribute_ptr BoolAttribute::copy()
{
    BoolAttribute_ptr ret_attr = std::make_shared<BoolAttribute>(name,
                                                                 mutate_rate);
    ret_attr->value = value;
    return ret_attr;
}
/**
 * @brief Mutates the Attribute's Value
 */
void BoolAttribute::mutate_value()
{
    if (rand_bool(mutate_rate))
    {
        value = rand_bool(0.5);
    }
}
/**
 * @brief Provides a string detailing the class for debugging
 *
 * @return std::string
 */
std::string BoolAttribute::to_string()
{
    return "Bool Attribute: '" + name + "' with value " + std::to_string(value) +
           "(mr: " + std::to_string(mutate_rate) +
           ")";
}
/**
 * @brief Validates the variables of the Attribute
 *
 * @return true
 * @return false
 */
bool BoolAttribute::validate()
{
    if (mutate_rate < 0)
    {
        throw(std::invalid_argument("Mutate Rate (" + std::to_string(mutate_rate) + ") must be greater than 0"));
    }
    else if (mutate_rate > 1.0F)
    {
        throw(std::invalid_argument("Mutate Rate (" + std::to_string(mutate_rate) + ") must be less than 1"));
    }
    return true;
}

/// ------------ IntAttribute Definitions ------------///

// Constructors
/**
 * @brief Construct a new Int Attribute:: Int Attribute object
 *
 * @param _name Name of the Attribute
 * @param _mutate_rate probability of mutating the value
 * @param _mutate_power Gaussian Standard deviation of mutations
 * @param _min_value Minimum value of the attribute
 * @param _max_value Maximum Value of the attribute
 */
IntAttribute::IntAttribute(const std::string &_name,
                           const float &_mean,
                           const float &_stdev,
                           const std::string &_init_type,
                           const float &_mutate_rate,
                           const float &_mutate_power,
                           const int &_min_value,
                           const int &_max_value)
{
    name = _name;
    mean = _mean;
    stdev = _stdev;
    init_type = _init_type;
    mutate_rate = _mutate_rate;
    mutate_power = _mutate_power;
    min_value = _min_value;
    max_value = _max_value;

    distribution = std::normal_distribution<float>(0, mutate_power);

    validate();

    if (!init_type.compare("gauss") || !init_type.compare("gaussian") || !init_type.compare("normal"))
    {
        std::normal_distribution<float> init_dist(mean, stdev);
        value = static_cast<int>(init_dist(generator));
    }
    else if (!init_type.compare("uniform"))
    {
        std::uniform_int_distribution<int> init_dist(min_value, max_value);
        value = init_dist(generator);
    }
    else
    {
        throw std::invalid_argument("Invalid init_type for Int Attribute");
    }
    value = std::clamp(value, min_value, max_value);
}

// Getters
/**
 * @brief Gets the Float Equivalent of the Attribute's Value
 *
 * @return float
 */
float IntAttribute::get_float_value() { return static_cast<float>(value); }
/**
 * @brief Gets Bool Equivalent of Attribute's Value
 *
 * @return true
 * @return false
 */
bool IntAttribute::get_bool_value() { return (value == 0) ? false : true; }
/**
 * @brief Gets String Equivalent of Attribute's Value
 *
 * @return std::string
 */
std::string IntAttribute::get_string_value() { return std::to_string(value); }
/**
 * @brief Gets Mutate Rate
 *
 * @return float
 */
float IntAttribute::get_mutate_rate() { return mutate_rate; }
/**
 * @brief Gets Mutate Power
 *
 * @return float
 */
float IntAttribute::get_mutate_power() { return mutate_power; }
/**
 * @brief Gets Minimum Value of Attribute
 *
 * @return int
 */
int IntAttribute::get_min_value() { return min_value; }
/**
 * @brief Gets Maximum Value of Attribute
 *
 * @return int
 */
int IntAttribute::get_max_value() { return max_value; }

// Other Methods
/**
 * @brief Creates a pointer to a new copy of this Attribute
 *
 * @return IntAttribute*
 */
Attribute_ptr IntAttribute::copy()
{
    IntAttribute_ptr ret_attr = std::make_shared<IntAttribute>(name,
                                                               mean,
                                                               stdev,
                                                               init_type,
                                                               mutate_rate,
                                                               mutate_power,
                                                               min_value,
                                                               max_value);
    ret_attr->value = value;
    return ret_attr;
}
/**
 * @brief Randomly Mutates the Attribute's Value
 *
 */
void IntAttribute::mutate_value()
{
    if (rand_bool(mutate_rate))
    {
        value += static_cast<int>(std::round(distribution(generator)));
        value = (value < min_value) ? min_value : (max_value > value) ? value
                                                                      : max_value;
    }
}
/**
 * @brief Provides a string detailing the class for debugging
 *
 * @return std::string
 */
std::string IntAttribute::to_string()
{
    return "Integer Attribute: '" + name + "' with value " + std::to_string(value) +
           "(mr: " + std::to_string(mutate_rate) +
           ", mp: " + std::to_string(mutate_power) +
           ", min: " + std::to_string(min_value) +
           ", max: " + std::to_string(max_value) +
           ")";
}
/**
 * @brief Validates the variables of the Attribute
 *
 * @return true
 * @return false
 */
bool IntAttribute::validate()
{
    if (min_value > max_value)
    {
        throw(std::invalid_argument("Min Value: " + std::to_string(min_value) + " must be less than Max Value: " + std::to_string(max_value)));
    }
    if (mean > max_value)
    {
        throw(std::invalid_argument("Mean Value: " + std::to_string(mean) + " must be less than Max Value: " + std::to_string(max_value)));
    }
    if (mean < min_value)
    {
        throw(std::invalid_argument("Mean Value: " + std::to_string(mean) + " must be greater than Min Value: " + std::to_string(min_value)));
    }
    if (stdev < 0)
    {
        throw(std::invalid_argument("Standard Deviation (" + std::to_string(stdev) + ") must be greater than 0"));
    }
    if (mutate_rate < 0)
    {
        throw(std::invalid_argument("Mutate Rate (" + std::to_string(mutate_rate) + ") must be greater than 0"));
    }
    else if (mutate_rate > 1.0F)
    {
        throw(std::invalid_argument("Mutate Rate (" + std::to_string(mutate_rate) + ") must be less than 1"));
    }
    if (mutate_power < 0)
    {
        throw(std::invalid_argument("Mutate Power (" + std::to_string(mutate_rate) + ") must be greater than 0"));
    }

    return true;
}

/// ------------ FloatAttribute Definitions ------------///

// Constructors
/**
 * @brief Construct a new Int Attribute:: Int Attribute object
 *
 * @param _name Name of the Attribute
 * @param _mean Mean Value of the Attribue
 * @param _stdev Standard Deviation of Values
 * @param _mutate_rate probability of mutating the value
 * @param _mutate_power Gaussian Standard deviation of mutations
 * @param _min_value Minimum value of the attribute
 * @param _max_value Maximum Value of the attribute
 */
FloatAttribute::FloatAttribute(const std::string &_name,
                               const float &_mean,
                               const float &_stdev,
                               const std::string &_init_type,
                               const float &_mutate_rate,
                               const float &_mutate_power,
                               const float &_min_value,
                               const float &_max_value)
{
    name = _name;
    mean = _mean;
    stdev = _stdev;
    init_type = _init_type;
    mutate_rate = _mutate_rate;
    mutate_power = _mutate_power;
    min_value = _min_value;
    max_value = _max_value;

    distribution = std::normal_distribution<float>(0, mutate_power);

    validate();

    if (!init_type.compare("gauss") || !init_type.compare("gaussian") || !init_type.compare("normal"))
    {
        std::normal_distribution<float> init_dist(mean, stdev);
        value = init_dist(generator);
    }
    else if (!init_type.compare("uniform"))
    {
        std::uniform_real_distribution<float> init_dist(min_value, max_value);
        value = init_dist(generator);
    }
    else
    {
        throw std::invalid_argument("Invalid init_type for Float Attribute");
    }

    value = std::clamp(value, min_value, max_value);
}

// Getters
/**
 * @brief Gets the Float Equivalent of the Attribute's Value
 *
 * @return float
 */
float FloatAttribute::get_float_value() { return value; }
/**
 * @brief Gets Bool Equivalent of Attribute's Value
 *
 * @return true
 * @return false
 */
bool FloatAttribute::get_bool_value() { return (value == 0) ? false : true; }
/**
 * @brief Gets String Equivalent of Attribute's Value
 *
 * @return std::string
 */
std::string FloatAttribute::get_string_value() { return std::to_string(value); }
/**
 * @brief Gets Mutate Rate
 *
 * @return float
 */
float FloatAttribute::get_mutate_rate() { return mutate_rate; }
/**
 * @brief Gets Mutate Power
 *
 * @return float
 */
float FloatAttribute::get_mutate_power() { return mutate_power; }
/**
 * @brief Gets Minimum Value of Attribute
 *
 * @return float
 */
float FloatAttribute::get_min_value() { return min_value; }
/**
 * @brief Gets Maximum Value of Attribute
 *
 * @return float
 */
float FloatAttribute::get_max_value() { return max_value; }

// Other Methods
/**
 * @brief Creates a pointer to a new copy of this Attribute
 *
 * @return FloatAttribute*
 */
Attribute_ptr FloatAttribute::copy()
{
    FloatAttribute_ptr ret_attr = std::make_shared<FloatAttribute>(name,
                                                                   mean,
                                                                   stdev,
                                                                   init_type,
                                                                   mutate_rate,
                                                                   mutate_power,
                                                                   min_value,
                                                                   max_value);
    ret_attr->value = value;
    return ret_attr;
}
/**
 * @brief Randomly Mutates the Attribute's Value
 */
void FloatAttribute::mutate_value()
{
    if (rand_bool(mutate_rate))
    {

        value += distribution(generator);
        value = (value < min_value) ? min_value : (max_value > value) ? value
                                                                      : max_value;
    }
}
/**
 * @brief Provides a string detailing the class for debugging
 *
 * @return std::string
 */
std::string FloatAttribute::to_string()
{
    return "Float Attribute: '" + name + "' with value " + std::to_string(value) +
           "(mr: " + std::to_string(mutate_rate) +
           ", mp: " + std::to_string(mutate_power) +
           ", min: " + std::to_string(min_value) +
           ", max: " + std::to_string(max_value) +
           ")";
}
/**
 * @brief Validates the variables of the Attribute
 *
 * @return true
 * @return false
 */
bool FloatAttribute::validate()
{
    if (min_value > max_value)
    {
        throw(std::invalid_argument("Min Value: " + std::to_string(min_value) + " must be less than Max Value: " + std::to_string(max_value)));
    }
    if (mean > max_value)
    {
        throw(std::invalid_argument("Mean Value: " + std::to_string(mean) + " must be less than Max Value: " + std::to_string(max_value)));
    }
    if (mean < min_value)
    {
        throw(std::invalid_argument("Mean Value: " + std::to_string(mean) + " must be greater than Min Value: " + std::to_string(min_value)));
    }
    if (stdev < 0)
    {
        throw(std::invalid_argument("Standard Deviation (" + std::to_string(stdev) + ") must be greater than 0"));
    }
    if (mutate_rate < 0)
    {
        throw(std::invalid_argument("Mutate Rate (" + std::to_string(mutate_rate) + ") must be greater than 0"));
    }
    else if (mutate_rate > 1.0F)
    {
        throw(std::invalid_argument("Mutate Rate (" + std::to_string(mutate_rate) + ") must be less than 1"));
    }
    if (mutate_power < 0)
    {
        throw(std::invalid_argument("Mutate Power (" + std::to_string(mutate_rate) + ") must be greater than 0"));
    }
    return true;
}

StringAttribute::StringAttribute(const std::string &_name, const float &_mutate_rate, const std::set<std::string> &_options)
{
    name = _name;
    mutate_rate = _mutate_rate;
    options = _options;
    distribution = std::uniform_int_distribution<int>(0, options.size() - 1);

    validate();

    value = random_option();
}

std::string StringAttribute::random_option()
{
    std::set<std::string>::iterator it = options.begin();
    std::advance(it, distribution(generator));
    return *it;
}

float StringAttribute::get_float_value()
{
    float fl;
    try
    {
        fl = std::stof(value);
    }
    catch (std::invalid_argument)
    {
        fl = 0;
    }
    return fl;
}
bool StringAttribute::get_bool_value()
{
    return value != "";
}
std::string StringAttribute::get_string_value()
{
    return value;
}
float StringAttribute::get_mutate_rate()
{
    return mutate_rate;
}
std::set<std::string> StringAttribute::get_options()
{
    return options;
}

Attribute_ptr StringAttribute::copy()
{
    StringAttribute_ptr ret_attr = std::make_shared<StringAttribute>(name,
                                                                     mutate_rate,
                                                                     options);

    ret_attr->value = value;

    return ret_attr;
}
void StringAttribute::mutate_value()
{
    if (rand_bool(mutate_rate))
    {
        value = random_option();
    }
}
std::string StringAttribute::to_string()
{
    std::string ret_str = "String Attribute: '" + name + "' with value '" + value +
                          "' (mr: " + std::to_string(mutate_rate) +
                          ", options: [";
    for (std::string op : options)
    {
        ret_str += (op + ", ");
    }
    ret_str += "])";

    return ret_str;
}

bool StringAttribute::validate()
{
    if (mutate_rate < 0)
    {
        throw(std::invalid_argument("Mutate Rate (" + std::to_string(mutate_rate) + ") must be greater than 0"));
    }
    else if (mutate_rate > 1.0F)
    {
        throw(std::invalid_argument("Mutate Rate (" + std::to_string(mutate_rate) + ") must be less than 1"));
    }

    if (options.size() <= 0)
    {
        throw(std::invalid_argument("You must provide at least 1 option to a StringAttribute"));
    }
    return true;
}