#ifndef GENES_H
#define GENES_H

#include <map>
#include <vector>
#include <utility>
#include "attributes.h"

class Gene
{
public:
    // Public Variables
    std::map<std::string, Attribute *> attributes;

    // Member Function Declaration
    bool has_attribute(std::string _key);
    Attribute *get_attribute(std::string _key);
    void mutate();
    std::string to_string();

protected:
    bool verify_ind_attribute(std::string _key, std::string _type);
    bool verify_ind_attribute(std::string _key, std::vector<std::string> _types);
    virtual void verify_attributes() = 0;

    std::map<std::string, valid_types> type_map = {
        {"FloatAttribute", float_attribute},
        {"IntAttribute",  int_attribute},
        {"BoolAttribute",  bool_attribute},
        {"StringAttribute", string_attribute}
    };
};

class NodeGene : public Gene
{
public:
    int key;
    bool operator<(const NodeGene *other) { return this->key < other->key; }
    NodeGene(int _key, std::vector<Attribute *> _attributes);
    float distance(NodeGene *other);
    NodeGene *copy();
    NodeGene *crossover(NodeGene *gene2);
    std::string to_string();

private:
    void verify_attributes();
};

class ConnectionGene : public Gene
{
public:
    std::pair<int, int> key;
    bool operator<(const ConnectionGene *other) { return std::abs(this->key.first + this->key.second) < std::abs(other->key.first + other->key.second); }
    ConnectionGene(std::pair<int, int> _key, std::vector<Attribute *> _attributes);
    float distance(ConnectionGene *other);
    ConnectionGene *copy();
    ConnectionGene *crossover(ConnectionGene *gene2);
    std::string to_string();
    void disable();
    void enable();

private:
    void verify_attributes();
};

#endif // GENES_H