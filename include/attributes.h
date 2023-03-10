#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <string>
#include <random>
#include <set>

enum valid_types
{
    float_attribute,
    int_attribute,
    bool_attribute,
    string_attribute
};

static bool rand_bool(float cutoff) { return ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) < cutoff); };

class Attribute
{
public:
    std::string name;

    // Overide Operators
    bool operator<(const Attribute &other) const { return (name < other.name); };

    // Getters
    virtual float get_float_value() = 0;
    virtual bool get_bool_value() = 0;
    virtual std::string get_string_value() = 0;
    virtual float get_mutate_rate() = 0;


    // Other Methods
    virtual Attribute *copy() = 0;
    virtual void mutate_value() = 0;
    virtual std::string to_string() = 0;
    virtual bool validate() = 0;
protected:
    float mutate_rate;
};

class BoolAttribute : public Attribute
{
public:    
    bool value;

public:
    // Constructors
    BoolAttribute(std::string _name, float _mutate_rate);
    BoolAttribute(std::string _name, bool _default_value, float _mutate_rate);

    // Getters
    float get_float_value();
    bool get_bool_value();
    std::string get_string_value();
    float get_mutate_rate();

    // Other Methods
    BoolAttribute *copy();
    void mutate_value();
    std::string to_string();
    bool validate();
};

class IntAttribute : public Attribute
{
public:
    int value;

private:
    float mutate_power;
    int min_value;
    int max_value;
    std::normal_distribution<float> distribution;
    std::default_random_engine generator;

public:
    // Constructors
    IntAttribute(std::string _name, float _mutate_rate, float _mutate_power, int _min_value, int _max_value);

    // Getters
    float get_float_value();
    bool get_bool_value();
    std::string get_string_value();
    float get_mutate_rate();
    float get_mutate_power();
    int get_min_value();
    int get_max_value();

    // Other Methods
    IntAttribute *copy();
    void mutate_value();
    std::string to_string();
    bool validate();
};

class FloatAttribute : public Attribute
{
public:
    float value;

private:
    float mutate_power;
    float min_value;
    float max_value;
    std::normal_distribution<float> distribution;
    std::default_random_engine generator;

public:
    // Constructors
    FloatAttribute(std::string _name, float _mutate_rate, float _mutate_power, float _min_value, float _max_value);

    // Getters
    float get_float_value();
    bool get_bool_value();
    std::string get_string_value();
    float get_mutate_rate();
    float get_mutate_power();
    float get_min_value();
    float get_max_value();

    // Other Methods
    FloatAttribute *copy();
    void mutate_value();
    std::string to_string();
    bool validate();
};

class StringAttribute : public Attribute
{
public:
    std::string value;

private:
    std::set<std::string> options;
    std::uniform_int_distribution<int> distribution;
    std::default_random_engine generator;

public:
    StringAttribute(std::string _name, float _mutate_rate,  std::set<std::string> _options);
    float get_float_value();
    bool get_bool_value();
    std::string get_string_value();
    float get_mutate_rate();
    std::set<std::string> get_options();

    StringAttribute *copy();
    void mutate_value();
    std::string to_string();
    bool validate();
    
private:
    std::string random_option();
};
#endif // ATTRIBUTES_H