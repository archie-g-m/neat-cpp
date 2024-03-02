#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <string>
#include <random>
#include <set>
#include <memory>

enum AttributeTypes
{
    float_attribute,
    int_attribute,
    bool_attribute,
    string_attribute
};
constexpr float RAND_MAX_INV = 1.0F / static_cast<float>(RAND_MAX);
static float rand_dec() { return static_cast<float>(rand()) * RAND_MAX_INV; }
static bool rand_bool(float cutoff) { return (rand_dec() < cutoff); }


typedef std::shared_ptr<class Attribute> Attribute_ptr;

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
    virtual Attribute_ptr copy() = 0;
    virtual void mutate_value() = 0;
    virtual std::string to_string() = 0;
    virtual bool validate() = 0;

protected:
    float mutate_rate;
};

typedef std::shared_ptr<class BoolAttribute> BoolAttribute_ptr;

class BoolAttribute : public Attribute
{
public:
    bool value;

public:
    // Constructors
    BoolAttribute(const std::string &_name, const float &_mutate_rate);
    BoolAttribute(const std::string &_name, const bool &_default_value, const float &_mutate_rate);

    // Getters
    float get_float_value();
    bool get_bool_value();
    std::string get_string_value();
    float get_mutate_rate();

    // Other Methods
    Attribute_ptr copy();
    void mutate_value();
    std::string to_string();
    bool validate();
};

typedef std::shared_ptr<class IntAttribute> IntAttribute_ptr;

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
    IntAttribute(const std::string &_name, const float &_mutate_rate, const float &_mutate_power, const int &_min_value, const int &_max_value);

    // Getters
    float get_float_value();
    bool get_bool_value();
    std::string get_string_value();
    float get_mutate_rate();
    float get_mutate_power();
    int get_min_value();
    int get_max_value();

    // Other Methods
    Attribute_ptr copy();
    void mutate_value();
    std::string to_string();
    bool validate();
};

typedef std::shared_ptr<class FloatAttribute> FloatAttribute_ptr;

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
    FloatAttribute(const std::string &_name, const float &_mutate_rate, const float &_mutate_power, const float &_min_value, const float &_max_value);

    // Getters
    float get_float_value();
    bool get_bool_value();
    std::string get_string_value();
    float get_mutate_rate();
    float get_mutate_power();
    float get_min_value();
    float get_max_value();

    // Other Methods
    Attribute_ptr copy();
    void mutate_value();
    std::string to_string();
    bool validate();
};

typedef std::shared_ptr<class StringAttribute> StringAttribute_ptr;

class StringAttribute : public Attribute
{
public:
    std::string value;

private:
    std::set<std::string> options;
    std::uniform_int_distribution<int> distribution;
    std::default_random_engine generator;

public:
    StringAttribute(const std::string &_name, const float &_mutate_rate, const std::set<std::string> &_options);
    float get_float_value();
    bool get_bool_value();
    std::string get_string_value();
    float get_mutate_rate();
    std::set<std::string> get_options();

    Attribute_ptr copy();
    void mutate_value();
    std::string to_string();
    bool validate();

private:
    std::string random_option();
};
#endif // ATTRIBUTES_H