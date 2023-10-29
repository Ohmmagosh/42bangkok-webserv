#ifndef STRUCT_HPP
# define STRUCT_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>


typedef struct	s_upload
{
	bool                        enabled;
	std::string                 savePath;

	s_upload()
	{
		enabled  = false;
		savePath = "";
	}
}				t_upload;

typedef struct	s_cgi
{
	std::string				extension;
	std::string				executable;

	s_cgi()
	{
		extension   = "";
		executable  = "";
	}
}				t_cgi;

typedef struct	s_location
{
	std::string					path;
	std::string					root;
	std::vector<std::string>	method;
	std::string					redirection;
	bool						dirListing;
	std::string					defaultFile;
	t_cgi						cgi;
	t_upload					upload;

	s_location()
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
}				t_location;

typedef struct	s_serverConf
{
	std::string					host;
	int							port;
	std::string					serverName;
	bool						isDefault;
	std::string					serverroot;
	std::vector<t_location>		location;

	s_serverConf()
	{
		host        = "";
		port        = 0;
		serverName  = "";
		serverroot		= "";
		isDefault   =   false;
		location.clear();
	}
}				t_serverConf;

typedef struct	s_globalConf
{
	std::map<int, std::string>	default_error_pages;
	int									client_body_limit;

	s_globalConf()
	{
		default_error_pages.clear();
		client_body_limit = 0;
	}
}				t_globalConf;

typedef struct	s_con
{
	t_globalConf				global;
	std::vector<t_serverConf>	server;
}				t_con;

typedef struct	s_detail {
	bool						status;
	std::string					detail;
	t_serverConf				server;
	std::vector<t_location>		locations;
	t_location					location;
}				t_detail;

#endif
