#include "Conf.hpp"

Conf::Conf()
{
}

Conf::~Conf()
{
}

t_cgi Conf::parseCgiSection(std::ifstream& configFile) 
{
    t_cgi currentCgi;
    std::string line;

    while (std::getline(configFile, line)) 
    {
        line = Utility::trim(line, " ");
        if (line.empty() || line[0] == '#')
            continue;

        size_t pos = line.find(':');
        if (pos != std::string::npos) 
        {
            std::string key = Utility::trim(line.substr(0, pos), " ");
            std::string value = Utility::trim(line.substr(pos + 1), " ");

            if (key == "extension")
                currentCgi.extension = value;
            else if (key == "executable")
                currentCgi.executable = value;
        }
        else 
        {
            // If we encounter a line without ':', it means we've reached the end of the cgi section
            break;
        }
    }

    return currentCgi;
}



t_upload Conf::parseUploadSection(std::ifstream& configFile) 
{
    t_upload currentUpload;
    std::string line;

    while (std::getline(configFile, line)) 
    {
        line = Utility::trim(line, " ");
        if (line.empty() || line[0] == '#')
            continue;

        size_t pos = line.find(':');
        if (pos != std::string::npos) 
        {
            std::string key = Utility::trim(line.substr(0, pos), " ");
            std::string value = Utility::trim(line.substr(pos + 1), " ");

            if (key == "enabled")
                currentUpload.enabled = (value == "true");
            else if (key == "save_path")
                currentUpload.savePath = value;
        }
        else 
        {
            // If we encounter a line without ':', it means we've reached the end of the upload section
            break;
        }
    }

    return currentUpload;
}

std::vector<t_routes> Conf::parseRoutesSection(std::ifstream& configFile) 
{
    std::vector<t_routes> routesList;
    t_routes currentRoute;
    std::string line;

    while (std::getline(configFile, line)) 
    {
        // std::cout << serverConfigs.back().routes.size() << std::endl;
        line = Utility::trim(line, " ");
        if (line.empty() || line[0] == '#')
            break;

        std::cout << "route read " << line  << std::endl;
        if (line[0] == '-') 
        {
            if (line.substr(0, 7) == "- path:") 
            {
                    serverConfigs.back().routes.push_back(currentRoute);
                    currentRoute = t_routes(); // Reset the currentRoute
            }
        }  
    }

    if (!currentRoute.path.empty()) 
    {
        routesList.push_back(currentRoute);
    }

    return routesList;
}


bool Conf::parseConfigFile(const std::string& filePath) 
{
    std::ifstream configFile(filePath);
    if (!configFile.is_open()) 
	{
        std::cerr << "Failed to open config file: " << filePath << std::endl;
        return false;
    }

    std::string line;

	while (std::getline(configFile, line) && line != "servers:") 
    {
        line = Utility::trim(line, " ");
        if (line.empty() || line[0] == '#') 
            continue;

		if (line.find("default_error_pages:") != std::string::npos) 
		{
			while (std::getline(configFile, line) && line[0] == ' ')  // Check for lines that are indented
			{
				int errorCode = atoi(line.substr(0, line.find(':')).c_str());
				std::string errorPage = Utility::trim(line.substr(line.find(':') + 1), " ");
				globalConfig.default_error_pages[errorCode] = errorPage;
			}
		}
		if (line.find("client_body_limit:") != std::string::npos) 
		{
			globalConfig.client_body_limit = atoi(Utility::trim(line.substr(line.find(':') + 1), " ").c_str());
		}
	}
	while (std::getline(configFile, line)) 
    {

        line = Utility::trim(line, " ");
        if (line.empty() || line[0] == '#') 
            continue;

        // std::cout << "line : " << line << std::endl;
        if (line.substr(0, 7) == "- host:") 
        {
            t_serverConf newserverconf = t_serverConf();
            serverConfigs.push_back(newserverconf);
            std::cout << "Detected a new host configuration. Total: " << serverConfigs.size() << std::endl;
            // continue;
        }

        size_t pos = line.find(':');
        if (pos != std::string::npos) 
        {
            std::string key = Utility::trim(line.substr(0, pos), " ");
            std::string value = Utility::trim(line.substr(pos + 1), " ");

            // std::cout << "check!!" << std::endl;
            // std::cout << "key: " << key << std::endl;
            // std::cout << "value: " << value << std::endl;
            if (key == "- host")
                serverConfigs.back().host = value;
            else if (key == "port")
                serverConfigs.back().port = atoi(value.c_str());
            else if (key == "server_name")
                serverConfigs.back().serverName = value;
            else if (key == "default")
                serverConfigs.back().isDefault = (value == "true");
            else if (key == "routes")
            {
                serverConfigs.back().routes = parseRoutesSection(configFile);
//                 // std::vector<t_routes> newRoutes = parseRoutesSection(configFile);
//                 // for (std::vector<t_routes>::iterator it = newRoutes.begin(); it != newRoutes.end(); ++it)
//                 // {
//                 //     serverConfigs.back().routes.push_back(*it);
//                 // }
            }
//             // ... (handle other keys similarly)
        }
    }
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;

    return true;
}



//getter
std::map<int, std::string>	Conf::getDefaultErrorPages() const
{
	return globalConfig.default_error_pages;
}

int Conf::getClientBodyLimit() const
{
	return globalConfig.client_body_limit;
}

std::string Conf::getHost(size_t serverIndex) const
{
    if (serverIndex < serverConfigs.size())
    {
        return serverConfigs[serverIndex].host;
    }
    else
    {
        std::cerr << "Invalid server index provided." << std::endl;
        return "";
    }
}

int Conf::getPort(size_t serverIndex) const
{
    if (serverIndex < serverConfigs.size())
    {
        return serverConfigs[serverIndex].port;
    }
    else
    {
        std::cerr << "Invalid server index provided." << std::endl;
        return -1;
    }
}

std::string Conf::getServerName(size_t serverIndex) const
{
    if (serverIndex < serverConfigs.size())
    {
        return serverConfigs[serverIndex].serverName;
    }
    else
    {
        std::cerr << "Invalid server index provided." << std::endl;
        return "";
    }
}

bool Conf::getDefault(size_t serverIndex) const
{
    if (serverIndex < serverConfigs.size())
    {
        return serverConfigs[serverIndex].isDefault;
    }
    else
    {
        std::cerr << "Invalid server index provided." << std::endl;
        return false;
    }
}

//printer
void Conf::printDefaultErrorPages() const
{
	std::map<int, std::string>::const_iterator it;
	for (it = globalConfig.default_error_pages.begin(); it != globalConfig.default_error_pages.end(); ++it)
	{
		std::cout << "Error " << it->first << ": " << it->second << std::endl;
	}
}

void Conf::printGlobalConfig() const
{
	printDefaultErrorPages();
	std::cout << "client_body_limit : " << globalConfig.client_body_limit << std::endl;
}

void Conf::printHost(size_t serverIndex) const
{
    if (serverIndex < serverConfigs.size())
    {
        std::cout << "Host server number " << serverIndex << " : " << serverConfigs[serverIndex].host << std::endl;
    }
}

void Conf::printPort(size_t serverIndex) const
{
    if (serverIndex < serverConfigs.size())
    {
        std::cout << "Port server number " << serverIndex << " : " << serverConfigs[serverIndex].port << std::endl;
    }
}

void Conf::printServerName(size_t serverIndex) const
{
    if (serverIndex < serverConfigs.size())
    {
        std::cout << "server name number " << serverIndex << " : " << serverConfigs[serverIndex].serverName << std::endl;
    }
}

void Conf::printDefault(size_t serverIndex) const
{
    if (serverIndex < serverConfigs.size())
    {
        std::cout << "Default server number " << serverIndex << " : " << serverConfigs[serverIndex].isDefault << std::endl;
    }
}

void Conf::printServerConf() const
{
    for (size_t serverIndex = 0; serverIndex < serverConfigs.size(); ++serverIndex)
    {
        const t_serverConf& server = serverConfigs[serverIndex];
        // const t_serverConf& server = serverConfigs[1];

        std::cout << "Server Configuration #" << serverIndex + 1 << ":" << std::endl;
        std::cout << "Host: " << server.host << std::endl;
        std::cout << "Port: " << server.port << std::endl;
        std::cout << "Server Name: " << server.serverName << std::endl;
        std::cout << "Is Default: " << (server.isDefault ? "Yes" : "No") << std::endl;

        // Print routes for this server
        std::cout << "server " << serverIndex + 1 << " route size " << server.routes.size() << std::endl;
        for (size_t i = 0; i < server.routes.size(); ++i)
        {
            const t_routes& route = server.routes[i];
            std::cout << "  Route #" << i + 1 << ":" << std::endl;
            std::cout << "    Path: " << route.path << std::endl;
            std::cout << "    Root: " << route.root << std::endl;
            // ... (You can add more details for each route if needed)
        }

        std::cout << "------------------------------------" << std::endl;
    }
}
