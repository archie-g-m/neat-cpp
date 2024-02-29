#include "genes.h"

#include <map>
#include <iostream>
#include <vector>
#include <utility>
#include "attributes.h"

/// ------------ Gene Definitions ------------///
/**
 * @brief Mutates all attributes for this gene
 *
 */
void Gene::mutate()
{
    for (std::map<std::string, std::shared_ptr<Attribute>>::iterator it = attributes.begin(); it != attributes.end(); it++)
    {
        it->second->mutate_value();
    }
}
/**
 * @brief Checks whether Gene has attribute
 *
 * @param _key
 * @return true
 * @return false
 */
bool Gene::has_attribute(std::string _key)
{
    return static_cast<bool>(attributes.count(_key));
}
/**
 * @brief Verifies the Gene has _key attribute and it is _type
 *
 * @param _key
 * @param _type
 * @return true
 * @return false
 */
bool Gene::verify_ind_attribute(std::string _key, std::string _type)
{
    if (!this->has_attribute(_key))
    {
        throw std::invalid_argument("Attributes must contain a '" + _key + "' Attribute");
    }

    switch (type_map[_type])
    {
    case float_attribute:
        if (std::dynamic_pointer_cast<FloatAttribute>(attributes[_key]) == nullptr)
        {
            throw std::invalid_argument("Attribute '" + _key + "' must be of type '" + _type + "'");
        }
        break;
    case int_attribute:
        if (std::dynamic_pointer_cast<IntAttribute>(attributes[_key]) == nullptr)
        {
            throw std::invalid_argument("Attribute '" + _key + "' must be of type '" + _type + "'");
        }
        break;
    case bool_attribute:
        if (std::dynamic_pointer_cast<BoolAttribute>(attributes[_key]) == nullptr)
        {
            throw std::invalid_argument("Attribute '" + _key + "' must be of type '" + _type + "'");
        }
        break;
    case string_attribute:
        if (std::dynamic_pointer_cast<StringAttribute>(attributes[_key]) == nullptr)
        {
            throw std::invalid_argument("Attribute '" + _key + "' must be of type '" + _type + "'");
        }
        break;
    default:
        throw std::invalid_argument("Invalid Attribute Type '" + _type + "' Requested for Attribute'" + _key + "'");
        break;
    }
    return true;
}
/**
 * @brief Verifies Gene has _key attribute and it is one of _types
 *
 * @param _key
 * @param _types
 * @return true
 * @return false
 */
bool Gene::verify_ind_attribute(std::string _key, std::vector<std::string> _types)
{
    if (!(this->has_attribute(_key)))
    {
        throw std::invalid_argument("Attributes must contain a '" + _key + "' Attribute");
    }

    bool correct_type = false;
    for (std::string _type : _types)
    {
        switch (type_map[_type])
        {
        case float_attribute:
            if (std::dynamic_pointer_cast<FloatAttribute>(attributes[_key]) != nullptr)
            {
                correct_type = true;
            }
            break;
        case int_attribute:
            if (std::dynamic_pointer_cast<IntAttribute>(attributes[_key]) != nullptr)
            {
                correct_type = true;
            }
            break;
        case bool_attribute:
            if (std::dynamic_pointer_cast<BoolAttribute>(attributes[_key]) != nullptr)
            {
                correct_type = true;
            }
            break;
        case string_attribute:
            if (std::dynamic_pointer_cast<StringAttribute>(attributes[_key]) != nullptr)
            {
                correct_type = true;
            }
            break;
        default:
            throw std::invalid_argument("Invalid Attribute Type '" + _type + "' Requested for Attribute'" + _key + "'");
            break;
        }
        if (correct_type)
        {
            break;
        }
    }
    if (!correct_type)
    {
        std::string err_msg = "Attribute '" + _key + "' must be one of type [";
        for (int i = 0; i < _types.size(); i++)
        {
            err_msg += (" '" + _types[i] + "'");
        }
        err_msg += "]";
        std::invalid_argument(err_msg.c_str());
    }
    return false;
}
/**
 * @brief Gets Pointer to Attribute with given key
 *
 * @param _key
 * @return Attribute*
 */
std::shared_ptr<Attribute> Gene::get_attribute(std::string _key)
{
    if (this->has_attribute(_key))
    {
        return attributes[_key];
    }
    else
    {
        throw std::invalid_argument("Could not find attribute '" + _key + "' in Gene");
    }
}
/// ------------ NodeGene Definitions ------------///

// Constructors
/**
 * @brief Construct a new Node Gene:: Node Gene object
 *
 * @param _key
 * @param _attributes
 */
NodeGene::NodeGene(int _key, std::vector<std::shared_ptr<Attribute>> _attributes)
{
    key = _key;
    for (int i = 0; i < _attributes.size(); i++)
    {
        attributes[_attributes[i]->name] = _attributes[i];
    }

    verify_attributes();
}

// Other Functions
/**
 * @brief Find distance to another gene
 *
 * @param other
 * @return float
 */
float NodeGene::distance(std::shared_ptr<NodeGene> &other)
{
    // Get Values of Each Attribute
    float bias = attributes["bias"]->get_float_value();
    float response = attributes["response"]->get_float_value();
    std::string activation = attributes["activation"]->get_string_value();
    std::string aggregation = attributes["aggregation"]->get_string_value();

    float bias_other = other->attributes["bias"]->get_float_value();
    float response_other = other->attributes["response"]->get_float_value();
    std::string activation_other = other->attributes["activation"]->get_string_value();
    std::string aggregation_other = other->attributes["aggregation"]->get_string_value();

    // Calculate Distance between the
    float dist = std::abs(bias - bias_other) + std::abs(response - response_other);
    dist += (activation == activation_other ? 0 : 1);
    dist += (aggregation == aggregation_other ? 0 : 1);
    return dist;
}
/**
 * @brief Creates a pointer to a new copy of this NodeGene
 *
 * @return NodeGene*
 */
std::shared_ptr<NodeGene> NodeGene::copy()
{
    std::vector<std::shared_ptr<Attribute>> attributes_copy;
    for (std::map<std::string, std::shared_ptr<Attribute>>::iterator it = attributes.begin(); it != attributes.end(); it++)
    {
        std::shared_ptr<Attribute> new_attr = (it->second)->copy();
        attributes_copy.push_back(new_attr);
    }
    return std::make_shared<NodeGene>(key, attributes_copy);
}
/**
 * @brief Creates an std::make_shared<NodeGene>  with a mix of attributes between each gene
 *
 * @return NodeGene*
 */
std::shared_ptr<NodeGene> NodeGene::crossover(std::shared_ptr<NodeGene> &gene2)
{
    if (key != gene2->key)
    {
        throw std::invalid_argument("Invalid Crossover between NodeGeene: " +
                                    std::to_string(key) + ", " +
                                    std::to_string(gene2->key));
    }

    std::vector<std::shared_ptr<Attribute>> attributes_copy;
    // Find all
    for (std::map<std::string, std::shared_ptr<Attribute>>::iterator it = attributes.begin(); it != attributes.end(); it++)
    {
        std::shared_ptr<Attribute> new_attr;
        // If the Attribute is shared between the genes then randomly pick one of the parents to inherit from
        if (gene2->has_attribute(it->first))
        {
            if (rand_bool(0.5))
            {
                std::cout << "Using Other Attribute in Crossover: " << gene2->attributes[it->first]->to_string() << std::endl;
                new_attr = gene2->attributes[it->first]->copy();
            }
            else
            {
                std::cout << "Using Original Attribute in Crossover: " << (it->second)->to_string() << std::endl;
                new_attr = (it->second)->copy();
            }
            attributes_copy.push_back(new_attr);
        }
    }

    return std::make_shared<NodeGene>(key, attributes_copy);
}
/**
 * @brief Generates a printable string for this Gene
 *
 * @return std::string
 */
std::string NodeGene::to_string()
{
    std::string ret_str = "NodeGene '" + std::to_string(key) + "'";
    for (std::map<std::string, std::shared_ptr<Attribute>>::iterator it = attributes.begin(); it != attributes.end(); it++)
    {
        ret_str = ret_str + "\n\t" + (it->second)->to_string();
    }
    return ret_str;
}
/**
 * @brief Verify All Attributes are valid
 */
void NodeGene::verify_attributes()
{
    // Check whether FloatAttribute/IntAttribute 'Bias' is in Gene
    std::vector<std::string> bias_types;
    bias_types.push_back("FloatAttribute");
    bias_types.push_back("IntAttribute");
    verify_ind_attribute("bias", bias_types);

    // Check whether FloatAttribute/IntAttribute 'Response' is in Gene
    std::vector<std::string> resp_types;
    resp_types.push_back("FloatAttribute");
    resp_types.push_back("IntAttribute");
    verify_ind_attribute("response", resp_types);

    // Check whether StringAttribute 'Activation' is in Gene
    std::vector<std::string> act_types;
    act_types.push_back("StringAttribute");
    verify_ind_attribute("activation", act_types);

    // Check whether StringAttribute 'Aggregation' is in Gene
    std::vector<std::string> agg_types;
    agg_types.push_back("StringAttribute");
    verify_ind_attribute("aggregation", agg_types);

    for (std::map<std::string, std::shared_ptr<Attribute>>::iterator it = attributes.begin(); it != attributes.end(); it++)
    {
        (it->second)->validate();
    }
}

/// ------------ ConnectionGene Definitions ------------///

// Constructors
/**
 * @brief Construct a new Connection Gene:: Connection Gene object
 *
 * @param _key
 * @param _attributes
 */
ConnectionGene::ConnectionGene(std::pair<int, int> _key, std::vector<std::shared_ptr<Attribute>> _attributes)
{
    key = _key;
    for (int i = 0; i < _attributes.size(); i++)
    {
        attributes[_attributes[i]->name] = _attributes[i];
    }

    verify_attributes();
}

// Other Functions
/**
 * @brief Find distance to another gene
 *
 * @param other
 * @return float
 */
float ConnectionGene::distance(std::shared_ptr<ConnectionGene> &other)
{
    float weight = attributes["weight"]->get_float_value();
    float other_weight = other->attributes["weight"]->get_float_value();

    float dist = std::abs(weight - other_weight);

    if (attributes["enable"]->get_bool_value() xor other->attributes["enable"]->get_bool_value())
    {
        dist += 1.0F;
    }

    return dist;
}
/**
 * @brief Creates a pointer to a new copy of this ConnectionGene
 *
 * @return ConnectionGene*
 */
std::shared_ptr<ConnectionGene> ConnectionGene::copy()
{
    std::vector<std::shared_ptr<Attribute>> attributes_copy;
    for (std::map<std::string, std::shared_ptr<Attribute>>::iterator it = attributes.begin(); it != attributes.end(); it++)
    {
        std::shared_ptr<Attribute> new_attr = (it->second)->copy();
        attributes_copy.push_back(new_attr);
    }
    return std::make_shared<ConnectionGene>(this->key, attributes_copy);
}
/**
 * @brief Creates an std::make_shared<ConnectionGene>  with a mix of attributes between each gene
 *
 * @return ConnectionGene*
 */
std::shared_ptr<ConnectionGene> ConnectionGene::crossover(std::shared_ptr<ConnectionGene> &gene2)
{
    if ((this->key.first != gene2->key.first) && (this->key.second != gene2->key.second))
    {
        throw std::invalid_argument("Invalid Crossover between ConnectionGeene: (" +
                                    std::to_string(this->key.first) + ", " +
                                    std::to_string(this->key.second) + ") and (" +
                                    std::to_string(gene2->key.first) + ", " +
                                    std::to_string(gene2->key.second) + ")");
    }

    std::vector<std::shared_ptr<Attribute>> attributes_copy;
    for (std::map<std::string, std::shared_ptr<Attribute>>::iterator it = attributes.begin(); it != attributes.end(); it++)
    {
        std::shared_ptr<Attribute> new_attr;
        // If the Attribute is shared between the genes then randomly pick one of the parents to inherit from
        if (gene2->has_attribute(it->first))
        {
            if (rand_bool(0.5))
            {
                std::cout << "Using Other Attribute in Crossover: " << gene2->attributes[it->first]->to_string() << std::endl;
                new_attr = gene2->attributes[it->first]->copy();
            }
            else
            {
                std::cout << "Using Original Attribute in Crossover: " << (it->second)->to_string() << std::endl;
                new_attr = (it->second)->copy();
            }
            attributes_copy.push_back(new_attr);
        }
    }
    return std::make_shared<ConnectionGene>(this->key, attributes_copy);
}
/**
 * @brief Generates a printable string for this Gene
 *
 * @return std::string
 */
std::string ConnectionGene::to_string()
{
    std::string ret_str = "ConnectionGene (" + std::to_string(key.first) + ", " + std::to_string(key.second) + ")";
    for (std::map<std::string, std::shared_ptr<Attribute>>::iterator it = attributes.begin(); it != attributes.end(); it++)
    {
        ret_str = ret_str + "\n\t" + (it->second)->to_string();
    }

    return ret_str;
}
/**
 * @brief Verifies all Attributes are valid
 *
 */
void ConnectionGene::verify_attributes()
{
    // Check whether FloatAttribute/IntAttribute 'weight' is in Gene
    std::vector<std::string> weight_types;
    weight_types.push_back("FloatAttribute");
    weight_types.push_back("IntAttribute");
    verify_ind_attribute("weight", weight_types);

    // Check whether BoolAttribute 'enables' is in Gene
    verify_ind_attribute("enable", "BoolAttribute");

    for (std::map<std::string, std::shared_ptr<Attribute>>::iterator it = attributes.begin(); it != attributes.end(); it++)
    {
        (it->second)->validate();
    }
}
/**
 * @brief enables the 'enable' attribute of this Connection gene
 *
 */
void ConnectionGene::enable()
{
    // if not enabled, replace the enable attribute with a new one with the default value true
    if (!get_attribute("enable")->get_bool_value())
    {
        float mr = this->get_attribute("enable")->get_mutate_rate();
        this->attributes["enable"] = std::make_shared<BoolAttribute>("enable", true, mr);
    }
}
/**
 * @brief disables the 'enable' attribute of this Connection gene
 *
 */
void ConnectionGene::disable()
{
    // if enabled, replace the enable attribute with a new one with the default value false
    if (get_attribute("enable")->get_bool_value())
    {
        float mr = this->get_attribute("enable")->get_mutate_rate();
        this->attributes["enable"] = std::make_shared<BoolAttribute>("enable", true, mr);
    }
}