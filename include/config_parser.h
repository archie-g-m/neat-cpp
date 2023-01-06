#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <map>
#include <string>

class ConfigParser{
    public:
        ConfigParser(std::string fname);

        std::map<std::string, std::map<std::string, std::string>> data;

        std::string to_string();

    private:
        void strip_whitespace(std::string* value);
};

#endif //CONFIGPARSER_H
