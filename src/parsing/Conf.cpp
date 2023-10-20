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
        line = Utility::trim(line, " ");
        if (line.empty() || line[0] == '#')
            continue;

        if (line[0] == '-') 
        {
            if (!currentRoute.path.empty()) 
            {
                routesList.push_back(currentRoute);
                currentRoute = t_routes();
            }
        } 
        else 
        {
            size_t pos = line.find(':');
            if (pos != std::string::npos) 
            {
                std::string key = Utility::trim(line.substr(0, pos), " ");
                std::string value = Utility::trim(line.substr(pos + 1), " ");

                if (key == "path")
                    currentRoute.path = value;
                else if (key == "root")
                    currentRoute.root = value;
                else if (key == "methods") 
                {
                    size_t start = value.find('[') + 1;
                    size_t end = value.find(']');
                    std::string methodsStr = value.substr(start, end - start);
                    std::stringstream ss(methodsStr);
                    std::string method;
                    while (std::getline(ss, method, ',')) 
                    {
                        currentRoute.method.push_back(Utility::trim(method, " "));
                    }
                }
                else if (key == "redirect")
                    currentRoute.redirection = value;
                else if (key == "directory_listing")
                    currentRoute.dirListing = (value == "on");
                else if (key == "default_file")
                    currentRoute.defaultFile = value;
                else if (key == "cgi")
                    currentRoute.cgi = parseCgiSection(configFile);
                else if (key == "upload")
                    currentRoute.upload = parseUploadSection(configFile);
            }
            else 
            {
                // If we encounter a line without ':', it means we've reached the end of the routes section
                break;
            }
        }
    }

    if (!currentRoute.path.empty()) 
    {
        routesList.push_back(currentRoute);
    }

    return routesList;
}


bool Conf::parseConfigFile(const std::string& filePath, t_globalConf& globalConfig, std::vector<t_serverConf>& serverConfigs) 
{
    std::ifstream configFile(filePath);
    if (!configFile.is_open()) 
	{
        std::cerr << "Failed to open config file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    bool inServerSection = false;
    t_serverConf currentServer;

    while (std::getline(configFile, line)) 
	{
        line = Utility::trim(line, " ");
        if (line.empty() || line[0] == '#') 
			continue;

        if (line == "servers:") 
		{
            inServerSection = true;
            continue;
        }

        if (inServerSection) 
		{
            if (line.find("host:") != std::string::npos) 
			{
                if (!currentServer.host.empty()) 
				{
                    serverConfigs.push_back(currentServer);
                    currentServer = t_serverConf();
                }
                currentServer.host = Utility::trim(line.substr(line.find(':') + 1), " ");
            } 
			else if (line.find("port:") != std::string::npos) 
			{
                currentServer.port = atoi(Utility::trim(line.substr(line.find(':') + 1), " ").c_str());
            } 
			else if (line.find("server_name:") != std::string::npos) {
                currentServer.serverName = Utility::trim(line.substr(line.find(':') + 1), " ");
            } 
			else if (line.find("default:") != std::string::npos) 
			{
                currentServer.isDefault = (Utility::trim(line.substr(line.find(':') + 1), " ") == "true");
            } 
			if (line.find("routes:") != std::string::npos) 
			{
				std::vector<t_routes> newRoutes = parseRoutesSection(configFile);
				for (std::vector<t_routes>::iterator it = newRoutes.begin(); it != newRoutes.end(); ++it) 
				{
					currentServer.routes.push_back(*it);
				}
			}
        } 
		else 
		{
            if (line.find("default_error_pages:") != std::string::npos) 
			{
                while (std::getline(configFile, line) && line[0] != '-') 
				{
                    int errorCode = atoi(line.substr(0, line.find(':')).c_str());
                    std::string errorPage = Utility::trim(line.substr(line.find(':') + 1), " ");
                    globalConfig.default_error_pages[errorCode] = errorPage;
                }
            } 
			else if (line.find("client_body_limit:") != std::string::npos) 
			{
                globalConfig.client_body_limit = atoi(Utility::trim(line.substr(line.find(':') + 1), " ").c_str());
            }
        }
    }

    if (!currentServer.host.empty()) 
	{
        serverConfigs.push_back(currentServer);
    }

    return true;
}
