#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include "../stdlib.hpp"

class ConfigParser
{
    public:
        std::map<std::string, std::string> globalConfig;
        std::vector<std::map<std::string, std::string> > servers;

        bool parse(const std::string& configFileName);
};

#endif