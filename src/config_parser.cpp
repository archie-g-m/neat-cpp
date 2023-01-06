#include "config_parser.h"

#include <fstream>
#include <string>
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
            line.erase(0, line.find("[")+1);
            // Strip the trailing ']' symbol
            line.erase(line.find("]"), line.size() - 1);
            // Strip all whitespace between brackets
            strip_whitespace(&line);
            // track the current category
            category = line;
            // create an empty map for the data of this category
            data[category] = {};
            // std::cout << "HEADER: " << line << std::endl;
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

        // std::cout << "KEY: " << key << " VALUE: " << value << std::endl;

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