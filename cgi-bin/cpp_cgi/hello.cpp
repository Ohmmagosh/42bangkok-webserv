#include <cstdlib>
#include <iostream>


int main() {
	const char *name = getenv("NAME");
	std::string Name;
	if (!name)
		Name = "World";
	else
		Name = name;
	std::cout << "<!DOCTYPE html>\n";
	std::cout << "<html lang=\"en\">\n";
	std::cout << "<head>\n";
	std::cout << "    <meta charset=\"UTF-8\">\n";
	std::cout << "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
	std::cout << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	std::cout << "    <title>Hello, "<< name <<"/title>\n";
	std::cout << "</head>\n";
	std::cout << "<body>\n";
	std::cout << "    <h1>Hello, "<< name <<"</h1>\n";
	std::cout << "</body>\n";
	std::cout << "</html>\n";

	return 0;
}
