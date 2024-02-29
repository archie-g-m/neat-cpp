#ifndef GENES_H
#define GENES_H

#include <map>
#include <vector>
#include <utility>
#include <memory>
#include "attributes.h"

class Gene
{
public:
    // Public Variables
    std::map<std::string, std::shared_ptr<Attribute>> attributes;

    // Member Function Declaration
    bool has_attribute(std::string _key);
    std::shared_ptr<Attribute> get_attribute(std::string _key);
    void mutate();
    std::string to_string();

protected:
    bool verify_ind_attribute(std::string _key, std::string _type);
    bool verify_ind_attribute(std::string _key, std::vector<std::string> _types);
    virtual void verify_attributes() = 0;

    std::map<std::string, AttributeTypes> type_map = {
        {"FloatAttribute", AttributeTypes::float_attribute},
        {"IntAttribute", AttributeTypes::int_attribute},
        {"BoolAttribute", AttributeTypes::bool_attribute},
        {"StringAttribute", AttributeTypes::string_attribute}};
};

class NodeGene : public Gene
{
public:
    int key;
    bool operator<(const std::shared_ptr<NodeGene> other) { return this->key < other->key; }
    NodeGene(int _key, std::vector<std::shared_ptr<Attribute>> _attributes);
    float distance(std::shared_ptr<NodeGene> &other);
    std::shared_ptr<NodeGene> copy();
    std::shared_ptr<NodeGene> crossover(std::shared_ptr<NodeGene> &gene2);
    std::string to_string();

private:
    void verify_attributes();
};

class ConnectionGene : public Gene
{
public:
    std::pair<int, int> key;
    bool operator<(const std::shared_ptr<ConnectionGene> other) { return std::abs(this->key.first + this->key.second) < std::abs(other->key.first + other->key.second); }
    ConnectionGene(std::pair<int, int> _key, std::vector<std::shared_ptr<Attribute>> _attributes);
    float distance(std::shared_ptr<ConnectionGene> &other);
    std::shared_ptr<ConnectionGene> copy();
    std::shared_ptr<ConnectionGene> crossover(std::shared_ptr<ConnectionGene> &gene2);
    std::string to_string();
    void disable();
    void enable();

private:
    void verify_attributes();
};

#endif // GENES_H