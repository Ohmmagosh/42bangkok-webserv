#include "src/server/Server.hpp"
#include "src/stdlib.hpp"


int main()
{
	{
		std::fstream	file("config.conf");
		if (!file.is_open()) {
			std::cerr << "Unable to open file\n";
			return 1;
		}

		std::string line;
		std::stringstream	ss;
		std::string		ret;
		while (std::getline(file, line)) { // Read lines until end of file
			ss << line << std::endl; // Print each line
			// You can add more code here to process each line as needed
		}

		ret = ss.str();
		std::cout << ret;
		file.close(); // Close the file
		return 0;
	}
	// Server server(8080);
	// server.start();
	// return 0;
}
