#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <map>
#include <string>
#include <set>
#include <memory>

typedef std::shared_ptr<class ConfigParser> ConfigParser_ptr;

class ConfigParser
{
public:
    ConfigParser(std::string fname);

    std::map<std::string, std::map<std::string, std::string>> data;

    std::string to_string();

private:
    void strip_whitespace(std::string *value);
};

typedef std::shared_ptr<class SpecialConfig> SpecialConfigParser_ptr;

class SpecialConfig
{
public:
    std::map<std::string, std::string> get_subdata(ConfigParser_ptr _config, std::string header);
    std::set<std::string> split_string(std::string str);
    bool to_bool(std::string str);
};

#endif // CONFIGPARSER_H
