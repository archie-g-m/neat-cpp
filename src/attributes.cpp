#include "attributes.h"

#include <string>
#include <random>
#include <iostream>

/// ------------ BoolAttribute Definitions ------------///

// Constructors
/**
 * @brief Construct a new Bool Attribute:: Bool Attribute object
 *
 */
BoolAttribute::BoolAttribute()
{
    name = "";
    mutate_rate = 0;

    this->validate();

    value = true;
}
/**
 * @brief Construct a new Bool Attribute:: Bool Attribute object
 *
 * @param _name name of the Attribute
 * @param _mutate_rate probability of mutating the value
 */
BoolAttribute::BoolAttribute(std::string _name, float _mutate_rate)
{
    name = _name;
    mutate_rate = _mutate_rate;

    this->validate();

    value = rand_bool(0.5);
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
 * @brief Gets Mutate Rate of Attribute
 *
 * @return float
 */
float BoolAttribute::get_mutate_rate() { return mutate_rate; };

// Other Methods
/**
 * @brief Creates a pointer to a new copy of this Attribute
 *
 * @return BoolAttribute*
 */
BoolAttribute *BoolAttribute::copy()
{
    BoolAttribute *ret_attr = new BoolAttribute(this->name, this->mutate_rate);
    ret_attr->value = this->value;
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
    };
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
    return true;
}

/// ------------ IntAttribute Definitions ------------///

// Constructors
/**
 * @brief Construct a new Int Attribute:: Int Attribute object
 *
 */
IntAttribute::IntAttribute()
{
    name = "";
    mutate_rate = 0.F;
    mutate_power = 0.F;
    min_value = 0;
    max_value = 1;

    this->validate();

    value = 0;
}
/**
 * @brief Construct a new Int Attribute:: Int Attribute object
 *
 * @param _name Name of the Attribute
 * @param _mutate_rate probability of mutating the value
 * @param _mutate_power Gaussian Standard deviation of mutations
 * @param _min_value Minimum value of the attribute
 * @param _max_value Maximum Value of the attribute
 */
IntAttribute::IntAttribute(std::string _name, float _mutate_rate, float _mutate_power, int _min_value, int _max_value)
{
    name = _name;
    mutate_rate = _mutate_rate;
    mutate_power = _mutate_power;
    min_value = _min_value;
    max_value = _max_value;
    distribution = std::normal_distribution<float>(0, mutate_power);
    generator = std::default_random_engine();

    this->validate();

    value = min_value + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max_value - min_value));
};

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
 * @brief Gets Mutate Rate
 *
 * @return float
 */
float IntAttribute::get_mutate_rate() { return mutate_rate; };
/**
 * @brief Gets Mutate Power
 *
 * @return float
 */
float IntAttribute::get_mutate_power() { return mutate_power; };
/**
 * @brief Gets Minimum Value of Attribute
 *
 * @return int
 */
int IntAttribute::get_min_value() { return min_value; };
/**
 * @brief Gets Maximum Value of Attribute
 *
 * @return int
 */
int IntAttribute::get_max_value() { return max_value; };

// Other Methods
/**
 * @brief Creates a pointer to a new copy of this Attribute
 *
 * @return IntAttribute*
 */
IntAttribute *IntAttribute::copy()
{
    IntAttribute *ret_attr = new IntAttribute(this->name,
                                              this->mutate_rate,
                                              this->mutate_power,
                                              this->min_value,
                                              this->max_value);
    ret_attr->value = this->value;
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
};
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
    return true;
}

/// ------------ FloatAttribute Definitions ------------///

// Constructors
/**
 * @brief Construct a new Float Attribute:: Float Attribute object
 *
 */
FloatAttribute::FloatAttribute()
{
    name = "";
    mutate_rate = 0.F;
    mutate_power = 0.F;
    min_value = 0.F;
    max_value = 1.F;
    this->validate();
    value = 0.F;
}
/**
 * @brief Construct a new Int Attribute:: Int Attribute object
 *
 * @param _name Name of the Attribute
 * @param _mutate_rate probability of mutating the value
 * @param _mutate_power Gaussian Standard deviation of mutations
 * @param _min_value Minimum value of the attribute
 * @param _max_value Maximum Value of the attribute
 */
FloatAttribute::FloatAttribute(std::string _name, float _mutate_rate, float _mutate_power, float _min_value, float _max_value)
{
    name = _name;
    mutate_rate = _mutate_rate;
    mutate_power = _mutate_power;
    min_value = _min_value;
    max_value = _max_value;
    distribution = std::normal_distribution<float>(0, mutate_power);
    generator = std::default_random_engine();

    this->validate();

    value = min_value + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max_value - min_value));
};

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
 * @brief Gets Mutate Rate
 *
 * @return float
 */
float FloatAttribute::get_mutate_rate() { return mutate_rate; };
/**
 * @brief Gets Mutate Power
 *
 * @return float
 */
float FloatAttribute::get_mutate_power() { return mutate_power; };
/**
 * @brief Gets Minimum Value of Attribute
 *
 * @return float
 */
float FloatAttribute::get_min_value() { return min_value; };
/**
 * @brief Gets Maximum Value of Attribute
 *
 * @return float
 */
float FloatAttribute::get_max_value() { return max_value; };

// Other Methods
/**
 * @brief Creates a pointer to a new copy of this Attribute
 *
 * @return FloatAttribute*
 */
FloatAttribute *FloatAttribute::copy()
{
    FloatAttribute *ret_attr = new FloatAttribute(this->name,
                                                  this->mutate_rate,
                                                  this->mutate_power,
                                                  this->min_value,
                                                  this->max_value);
    ret_attr->value = this->value;
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
};
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
    return true;
}