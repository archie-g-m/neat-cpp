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
    std::map<std::string, std::string> get_subdata(std::string header);
    std::string to_string();

private:
    void strip_whitespace(std::string *value);
};

typedef std::shared_ptr<class SpecialConfig> SpecialConfigParser_ptr;

class SpecialConfig
{
public:
    template <class T>
    T get_value(const std::string _str);
    std::string to_string();

protected:
    std::map<std::string, std::string> data;

private:
    std::set<std::string> split_string(std::string str);
};

#endif // CONFIGPARSER_H
