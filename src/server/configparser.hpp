#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include "../stdlib.hpp"

struct ServerConf
{
    int port;
    std::string name;
    std::string landingPagePath;
    bool is_default;
    std::string redirect;
    bool directory_listing;
    std::vector<std::string> methods;

};

struct NonServerConf
{
    int MAX_BODY_SIZE;
    std::string dlpath;
    std::string dlname;
    std::string defaultErrorPage;
    // ... any other global configurations
};

class ConfigParser
{
    public:
        std::map<std::string, std::string> globalConfig;
        std::vector<ServerConf> servers;

        bool parse(const std::string& configFileName);
    
    private:
        std::string trim(const std::string& str);

};

#endif
