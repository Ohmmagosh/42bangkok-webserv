#include "Server.hpp"
#include "TBucket.hpp"
#include "Request.hpp"
#include "configparser.hpp"



bool ConfigParser::parse(const std::string& configFileName) 
{
    std::ifstream configFile(configFileName.c_str());
    if (!configFile) {
        std::cerr << "Failed to open config file: " << configFileName << std::endl;
        return false;
    }
}