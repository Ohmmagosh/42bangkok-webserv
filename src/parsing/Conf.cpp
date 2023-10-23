#include "Conf.hpp"

Conf::Conf()
{
}

Conf::~Conf()
{
}

bool    Conf::parseConfigFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::string line;

    if (!file.is_open()) 
    {
        return false; // Return false if the file could not be opened
    }

    parseGlobalSection(file);
    
    while (std::getline(file, line)) 
    {
        std::string trimmed = Utility::trim(line, " ");
        
        // Skip empty lines
        if (trimmed.empty()) 
            continue;

        if (trimmed == "server {") 
        {  // Start of a server section
            parseServerSection(file);
        } 
    else if (trimmed != "servers {") 
    {
        // Handle unexpected data or errors
        // Log or throw an error if needed
    }
    }

    return true;
}

void    Conf::parseGlobalSection(std::ifstream& file) 
{
    std::string line;
    
    // Flags to identify inside which block we are
    bool insideErrorPages = false;

    while (std::getline(file, line)) 
    {
        std::string trimmed = Utility::trim(line, " ");
        
        // Skip empty lines
        if (trimmed.empty()) 
            continue;
        
        // Check if we are entering or exiting a block
        if (trimmed.back() == '{') 
        {
            std::string key = Utility::trim(trimmed.substr(0, trimmed.size() - 1), " ");
            
            if (key == "default_error_pages") 
            {
                insideErrorPages = true;
            }
        } 
        else if (trimmed == "}") 
        {
            if (insideErrorPages) 
            {
                insideErrorPages = false;
            }
        } 
        else 
        {
            size_t pos = trimmed.find(':');
            if (pos != std::string::npos) {
                std::string key = Utility::trim(trimmed.substr(0, pos), " ");
                std::string value = Utility::trim(trimmed.substr(pos + 1), " ");
                
                // Remove the semicolon if present at the end
                if (value.back() == ';') 
                {
                    value.erase(value.size() - 1);
                }

                if (insideErrorPages) 
                {
                    std::stringstream ss(key);
                    int statusCode;
                    ss >> statusCode;
                    globalConfig.default_error_pages[statusCode] = value;
                } 
                else 
                {
                    if (key == "client_body_limit") 
                    {
                        std::stringstream ss(value);
                        ss >> globalConfig.client_body_limit;
                    }
                }
            }
        }
        
        if (trimmed == "servers {") 
            break;
    }
}

void Conf::parseServerSection(std::ifstream& file) 
{
    std::string line;
    t_serverConf currentServer;

    while (std::getline(file, line)) 
    {
        std::string trimmed = Utility::trim(line, " ");
        size_t pos = trimmed.find(':');

        if (pos != std::string::npos) 
        {
            std::string key = Utility::trim(trimmed.substr(0, pos), " ");
            std::string value = Utility::trim(trimmed.substr(pos + 1), " ");

            // Remove the semicolon if present at the end
            if (value[value.size() - 1] == ';') 
            {
                value.erase(value.size() - 1);
            }

            // Assign the parsed values to the currentServer structure
            if (key == "host") 
            {
                currentServer.host = value;
            } 
            else if (key == "port") 
            {
                std::istringstream iss(value);
                iss >> currentServer.port;
            } 
            else if (key == "server_name") 
            {
                currentServer.serverName = value;
            } 
            else if (key == "default") 
            {
                currentServer.isDefault = (value == "true");
            }
        }

        // std::cout << "Processing line in parseServerSection: [" << trimmed << "]" << std::endl;
        if (trimmed.find("location ") == 0) 
        {
            size_t startPath = trimmed.find(" ");
            size_t endPath = trimmed.find(" {");

            if (startPath != std::string::npos && endPath != std::string::npos && startPath < endPath)
            {
                std::string locationPath = trimmed.substr(startPath + 1, endPath - startPath - 1);
                currentRoute.path = locationPath;  // Assuming your t_routes structure has a path member
            }
    
            parseRouteSection(file);
            currentServer.routes.push_back(currentRoute);
            currentRoute = s_routes();
        }

        if (trimmed == "}") 
        {
            currentServer.routes.push_back(currentRoute);
            serverConfigs.push_back(currentServer);
            currentServer = s_serverConf();  // reset for next server
            currentRoute = s_routes();  // reset for next route
            return;
        }
    }
}

void Conf::parseRouteSection(std::ifstream& file) 
{
    std::string line;
    int depth = 1;

    while (std::getline(file, line) && depth > 0) 
    {
        std::string trimmed = Utility::trim(line, " ");
        std::cout << "Entered parseRouteSection with line: [" << trimmed << "]" << std::endl;

        if (trimmed.find("{") != std::string::npos) 
        {
            depth++;
        }
        if (trimmed.find("}") != std::string::npos) 
        {
            depth--;
        }
        if (depth == 0) 
        {
            break; // Exit the loop if we've closed the location block
        }

        size_t pos = trimmed.find(':');
        if (pos != std::string::npos) 
        {
            std::string key = Utility::trim(trimmed.substr(0, pos), " ");
            std::string value = Utility::trim(trimmed.substr(pos + 1), " ");
            
            if (!value.empty() && value[value.size() - 1] == ';') 
            {
                value.erase(value.size() - 1);
            }

            if (key == "path") 
            {
                currentRoute.path = value;
            } 
            else if (key == "root") 
            {
                currentRoute.root = value;
            } 
            else if (key == "redirect") 
            {
                currentRoute.redirection = value;
            } 
            else if (key == "directory_listing") 
            {
                currentRoute.dirListing = (value == "on");
            } 
            else if (key == "default_file") 
            {
                currentRoute.defaultFile = value;
            }
            // ... Add other route-specific configurations here...
        }

        if (trimmed == "cgi {") 
        {
            parseCgiSection(file);
        } 
        else if (trimmed == "upload {") 
        {
            // parseUploadSection(file);
        }
        // if (trimmed == "}") 
        // { // Exiting the location block
        //     break;
        // }

        std::cout << "Exiting parseRouteSection after processing line: [" << trimmed << "]" << std::endl;
    }

}

void Conf::parseCgiSection(std::ifstream& file) 
{
    std::string line;
    t_cgi cgiConfig; // assuming this is the structure to hold CGI configs

    while (std::getline(file, line)) {
        std::string trimmed = Utility::trim(line, " ");
        
        // Skip empty lines and comments
        if (trimmed.empty() || trimmed[0] == '#')
            continue;
        
        // Check if we've reached the end of the CGI section
        if (trimmed == "}") 
            break;
        
        size_t pos = trimmed.find(':');
        if (pos != std::string::npos) {
            std::string key = Utility::trim(trimmed.substr(0, pos), " ");
            std::string value = Utility::trim(trimmed.substr(pos + 1), " ");
            
            // Remove the semicolon if present at the end
            if (value.back() == ';') 
                value.erase(value.size() - 1);

            if (key == "extension") {
                cgiConfig.extension = value;
            } else if (key == "executable") {
                cgiConfig.executable = value;
            }
            // ... Add other CGI configurations if any
        }
    }
    currentRoute.cgi = cgiConfig;  // assuming currentRoute is a reference or pointer to the current route being parsed
}

void Conf::printGlobalConfig() const
{
    std::cout << "---- Global Configuration ----" << std::endl;
    
    std::cout << "client_body_limit: " << globalConfig.client_body_limit << std::endl;
    
    std::cout << "default_error_pages: " << std::endl;
    for (std::map<int, std::string>::const_iterator it = globalConfig.default_error_pages.begin(); it != globalConfig.default_error_pages.end(); ++it) 
    {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }
    
    std::cout << "-------------------------------" << std::endl;
}

void Conf::printServerConf() const 
{
    std::vector<t_serverConf>::const_iterator serverIt;
    for (serverIt = serverConfigs.begin(); serverIt != serverConfigs.end(); ++serverIt) 
    {
        std::cout << "Server Config:" << std::endl;
        std::cout << "----------------------" << std::endl;
        std::cout << "Host: " << serverIt->host << std::endl;
        std::cout << "Port: " << serverIt->port << std::endl;
        std::cout << "Server Name: " << serverIt->serverName << std::endl;
        std::cout << "Default: " << (serverIt->isDefault ? "True" : "False") << std::endl;
        
        std::cout << "--- Routes ---\n";
        std::vector<t_routes>::const_iterator routeIt;
        for (routeIt = serverIt->routes.begin(); routeIt != serverIt->routes.end(); ++routeIt) {
            std::cout << "    Route Path: " << routeIt->path << std::endl;
            std::cout << "    Root: " << routeIt->root << std::endl;
            std::cout << "    Redirect: " << routeIt->redirection << std::endl;
            std::cout << "    Directory Listing: " << (routeIt->dirListing ? "On" : "Off") << std::endl;
            std::cout << "    Default File: " << routeIt->defaultFile << std::endl;

            std::cout << "    CGI:" << std::endl;
            std::cout << "        Extension: " << routeIt->cgi.extension << std::endl;
            std::cout << "        Executable: " << routeIt->cgi.executable << std::endl;

            std::cout << "    Upload:" << std::endl;
            std::cout << "        Enabled: " << (routeIt->upload.enabled ? "True" : "False") << std::endl;
            std::cout << "        Save Path: " << routeIt->upload.savePath << std::endl;
        }
        std::cout << "----------------------" << std::endl << std::endl;
    }
}