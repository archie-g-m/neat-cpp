#ifndef GENES_H
#define GENES_H

#include <map>
#include <vector>
#include <utility>
#include <memory>
#include "attributes.h"

typedef std::shared_ptr<class Gene> Gene_ptr;

class Gene
{
public:
    // Public Variables
    std::map<std::string, Attribute_ptr> attributes;

    // Member Function Declaration
    bool has_attribute(std::string _key);
    Attribute_ptr get_attribute(std::string _key);
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

typedef std::shared_ptr<class NodeGene> NodeGene_ptr;

class NodeGene : public Gene
{
public:
    int key;
    bool operator<(const NodeGene_ptr other) { return this->key < other->key; }
    NodeGene(int _key, std::vector<Attribute_ptr> _attributes);
    float distance(NodeGene_ptr &other, float compatability_weight = 1.0F);
    NodeGene_ptr copy();
    NodeGene_ptr crossover(NodeGene_ptr &gene2);
    std::string to_string();

private:
    void verify_attributes();
};

typedef std::shared_ptr<class ConnectionGene> ConnectionGene_ptr;

class ConnectionGene : public Gene
{
public:
    std::pair<int, int> key;
    bool operator<(const ConnectionGene_ptr other) { return std::abs(this->key.first + this->key.second) < std::abs(other->key.first + other->key.second); }
    ConnectionGene(std::pair<int, int> _key, std::vector<Attribute_ptr> _attributes);
    float distance(ConnectionGene_ptr &other, float compatability_weight = 1.0F);
    ConnectionGene_ptr copy();
    ConnectionGene_ptr crossover(ConnectionGene_ptr &gene2);
    std::string to_string();
    void disable();
    void enable();

private:
    void verify_attributes();
};

#endif // GENES_H