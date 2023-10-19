#include "Server.hpp"
#include "TBucket.hpp"
#include "Request.hpp"
#include "configparser.hpp"

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
    ServerConf currentServer;  // Note: Using ServerConf struct

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
                if (currentServer.port != 0)  // Assuming port 0 is invalid, adjust as needed
                {
                    servers.push_back(currentServer);
                    currentServer = ServerConf();  // Reset the struct
                }
            }  
            else 
            {
                size_t pos = line.find(':');
                if (pos != std::string::npos) 
                {
                    std::string key = trim(line.substr(0, pos));
                    std::string value = trim(line.substr(pos + 1));
                    
                    if (key == "port")
                        currentServer.port = std::stoi(value);
                    else if (key == "name")
                        currentServer.name = value;
                    else if (key == "landingPagePath")
                        currentServer.landingPagePath = value;
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
    if (currentServer.port != 0)
    {
        servers.push_back(currentServer);
    }
    return true;
}

