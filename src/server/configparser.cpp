#include "Server.hpp"
#include "TBucket.hpp"
#include "Request.hpp"
#include "configparser.hpp"

std::string ConfigParser::trim(const std::string& str) 
{
    std::string::size_type start = str.find_first_not_of(" \t\n\r");
    std::string::size_type end = str.find_last_not_of(" \t\n\r");

    if (start == std::string::npos || end == std::string::npos) 
    {
        return "";
    }

    return str.substr(start, end - start + 1);
}

bool ConfigParser::parse(const std::string& configFileName) 
{
    std::ifstream configFile(configFileName.c_str());
    if (!configFile) 
    {
        std::cerr << "Failed to open config file: " << configFileName << std::endl;
        return false;
    }

    std::string line;
    bool isServerSection = false;
    std::map<std::string, std::string> currentServer;

    while (std::getline(configFile, line)) 
    {
        line = trim(line);
        if (line.length() == 0 || line[0] == '#') 
            continue;

        if (line == "servers:") 
        {
            isServerSection = true;
            continue;
        }

        if (isServerSection) 
        {
            if (line[0] == '-') 
            {
                if (!currentServer.empty()) 
                {
                    servers.push_back(currentServer);
                    currentServer.clear();
                }
            } 
            else 
            {
                size_t pos = line.find(':');
                if (pos != std::string::npos) 
                {
                    std::string key = trim(line.substr(0, pos));
                    std::string value = trim(line.substr(pos + 1));
                    currentServer[key] = value;
                }
            }
        }
        else 
        {
            size_t pos = line.find(':');
            if (pos != std::string::npos) 
            {
                std::string key = trim(line.substr(0, pos));
                std::string value = trim(line.substr(pos + 1));
                globalConfig[key] = value;
            }
        }
    }
    if (!currentServer.empty()) 
    {
        servers.push_back(currentServer);
    }
    return true;
}
