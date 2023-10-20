#ifndef CONF_HPP
# define CONF_HPP

# include "../stdlib.hpp"

typedef struct  s_upload
{
    bool                        enabled;
    std::string                 savePath;

    s_upload()
    {
        enabled  = false;
        savePath = "";
    }
}               t_upload;

typedef struct  s_cgi
{
    std::string                 extension;
    std::string                 executable;

    s_cgi()
    {
        extension   =   "";
        executable  =   "";
    }
}               t_cgi;

typedef struct  s_routes
{
    std::string                 path;
    std::string                 root;
    std::vector<std::string>    method;
    std::string                 redirection;
    bool                        dirListing;
    std::string                 defaultFile;
    t_cgi                       cgi;
    t_upload                    upload;

    s_routes()
    {
        path        =   "";
        root        =   "";
        method.clear();
        redirection =   "";
        dirListing  =   false;
        defaultFile =   "";
        cgi         =   s_cgi();
        upload      =   s_upload();
    }
}               t_routes;

typedef struct  s_serverConf
{
    std::string                 host;
    int                         port;
    std::string                 serverName;
    bool                        isDefault;
    std::vector<t_routes>       routes;

    s_serverConf()
    {
        host        =   "";
        port        =   0;
        serverName  =   "";
        isDefault   =   false;
        routes.clear();
    }
}               t_serverConf;

typedef struct  s_globalConf
{
    std::map<int, std::string>  default_error_pages;
    int                         client_body_limit;

    s_globalConf()
    {
        default_error_pages.clear();
        client_body_limit = 0;
    }
}               t_globalConf;

class Conf
{
	public:
		Conf();
		~Conf();

    private:
        t_globalConf                globalConfig;
        std::vector<t_serverConf>   serverConfigs;

        t_upload                     parseUploadSection(std::ifstream& configFile);
        t_cgi                        parseCgiSection(std::ifstream& configFile);
        bool                         parseConfigFile(const std::string& filePath, t_globalConf& globalConfig, std::vector<t_serverConf>& serverConfigs);
        std::vector<t_routes>        parseRoutesSection(std::ifstream& configFile);



};

#endif