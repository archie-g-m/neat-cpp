#include "config_parser.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <map>

ConfigParser::ConfigParser(std::string fname)
{
    // Open the .cfg file
    std::ifstream file;
    file.open(fname);

    // Parse the .cfg file
    std::string category = "";
    std::string line;
    while (std::getline(file, line))
    {
        // Determine if its a section header
        size_t header_lead = line.find("[");
        size_t header_tail = line.find("]");
        // If it is a Header then strip the [] and
        // set it as the current category
        // and initialize it in the data map
        if (header_lead != std::string::npos && header_tail != std::string::npos)
        {
            // Strip the leading '[' symbol
            line.erase(0, line.find("[") + 1);
            // Strip the trailing ']' symbol
            line.erase(line.find("]"), line.size() - 1);
            // Strip all whitespace between brackets
            strip_whitespace(&line);
            // track the current category
            category = line;
            // create an empty map for the data of this category
            data[category] = {};
            continue;
        }

        // Split the line into key and value
        size_t pos = line.find("=");
        if (pos == std::string::npos)
        {
            continue; // Skip invalid lines
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // Strip the leading and trailing whitespace from the key and value
        strip_whitespace(&key);
        strip_whitespace(&value);

        data[category][key] = value;
    }
}

std::string ConfigParser::to_string()
{
    std::string ret_str = "";
    // If there is no data, return empty string
    if (data.empty())
    {
        return ret_str;
    }

    // Otherwise Print out Map
    std::map<std::string, std::string> value;
    for (auto it = data.begin(); it != data.end(); it++)
    {
        // Get the key and value
        ret_str = ret_str + it->first + "\n";
        value = it->second;
        if (!value.empty())
        {
            for (auto it2 = value.begin(); it2 != value.end(); it2++)
            {
                ret_str = ret_str + "  - " + it2->first + " : " + it2->second + "\n";
            }
        }
    }

    return ret_str;
}

void ConfigParser::strip_whitespace(std::string *value)
{
    value->erase(value->begin(),
                 std::find_if(value->begin(),
                              value->end(),
                              [](int ch)
                              { return !std::isspace(ch); }));
    value->erase(std::find_if(value->rbegin(),
                              value->rend(),
                              [](int ch)
                              { return !std::isspace(ch); })
                     .base(),
                 value->end());
}
/**
 * @brief Get the subdata from the provided config
 *
 * @param _config
 * @param header
 * @return std::map<std::string, std::string>
 */
std::map<std::string, std::string> ConfigParser::get_subdata(std::string _header)
{
    // Configure from Parser
    if (data.count(_header))
    { // if parser doesnt have the NEAT category throw error
        return data[_header];
    }
    else
    {
        throw(std::invalid_argument("Provided ConfigParser does not contain the " + _header + " category"));
    }
}

/**
 * @brief splits the input string based on commas
 *
 * @param _str
 * @return std::set<std::string>
 */
std::set<std::string> SpecialConfig::split_string(const std::string _str)
{
    std::set<std::string> result;
    std::stringstream ss(_str);
    std::string item;
    while (std::getline(ss, item, ','))
    {
        // Trim leading and trailing whitespace
        item.erase(item.begin(), std::find_if(item.begin(), item.end(), [](int ch)
                                              { return !std::isspace(ch); }));
        item.erase(std::find_if(item.rbegin(), item.rend(), [](int ch)
                                { return !std::isspace(ch); })
                       .base(),
                   item.end());
        result.insert(item);
    }
    return result;
}

/**
 * @brief gets value at provided key as a set of strings split at the commas
 *
 * @tparam
 * @param _str
 * @return std::set<std::string>
 */
template <>
std::set<std::string> SpecialConfig::get_value<std::set<std::string>>(const std::string _str)
{
    if (data.count(_str))
    {
        return split_string(data[_str]);
    }
    else
    {
        throw std::invalid_argument("Invalid key '" + _str + "' provided to 'int SpecialConfig::get_value'");
    }
}

/**
 * @brief gets value at provided key as a string
 *
 * @tparam
 * @param _str
 * @return std::string
 */
template <>
std::string SpecialConfig::get_value<std::string>(const std::string _str)
{
    if (data.count(_str))
    {
        return data[_str];
    }
    else
    {
        throw std::invalid_argument("Invalid key '" + _str + "' provided to 'int SpecialConfig::get_value'");
    }
}

/**
 * @brief converts string to boolean if it is 'true', 'false', '1' or '0' (case doesn't matter)
 *
 * @param _str
 * @return true
 * @return false
 */
template <>
bool SpecialConfig::get_value<bool>(const std::string _str)
{
    if (data.count(_str))
    {
        std::string &value_string = data[_str];
        transform(value_string.begin(), value_string.end(), value_string.begin(), ::tolower);
        if (value_string == "true" || value_string == "1")
        {
            return true;
        }
        else if (value_string == "false" || value_string == "0")
        {
            return false;
        }
        else
        {
            throw std::invalid_argument("Invalid value '" + value_string + "' provided to 'bool SpecialConfig::get_value'");
        }
    }
    else
    {
        throw std::invalid_argument("Invalid key '" + _str + "' provided to 'bool SpecialConfig::get_value'");
    }
}

/**
 * @brief gets value at provided key as an int
 *
 * @tparam
 * @param _str
 * @return int
 */
template <>
int SpecialConfig::get_value<int>(const std::string _str)
{
    if (data.count(_str))
    {
        std::string &value_string = data[_str];

        try
        {
            return std::stoi(value_string);
        }
        catch (std::invalid_argument &e)
        {
            throw std::invalid_argument("Invalid value '" + value_string + "' provided to 'int SpecialConfig::get_value'");
        }
    }
    else
    {
        throw std::invalid_argument("Invalid key '" + _str + "' provided to 'int SpecialConfig::get_value'");
    }
}

/**
 * @brief gets value at provided key as a float
 *
 * @tparam
 * @param _str
 * @return float
 */
template <>
float SpecialConfig::get_value<float>(const std::string _str)
{
    if (data.count(_str))
    {
        std::string &value_string = data[_str];

        try
        {
            return std::stof(value_string);
        }
        catch (std::invalid_argument &e)
        {
            throw std::invalid_argument("Invalid value '" + value_string + "' provided to 'float SpecialConfig::get_value'");
        }
    }
    else
    {
        throw std::invalid_argument("Invalid key '" + _str + "' provided to 'float SpecialConfig::get_value'");
    }
}

// Helper function for automatic type deduction
template <typename T>
T get_value(const std::string _str)
{
    return get_value<T>(_str);
}

std::string SpecialConfig::to_string()
{
    std::string ret_str = "";
    for (auto &value : data)
    {
        ret_str = ret_str + "  - " + value.first + " : " + value.second + "\n";
    }
    return ret_str;
}
