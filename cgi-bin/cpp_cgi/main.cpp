#include <iostream>
#include <string>
#include <cstdlib> // for getenv()

using namespace std;

int main()
{
    // Output the HTTP header
    std::cout << "Content-type: text/html\r\n\r\n";

    // Output the HTML content
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "<title>CGI C++ Program</title>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "<h1>Hello, world!</h1>\n";

    // Read and display environment variables
    // std::cout << "<p>QUERY_STRING = " << getenv("QUERY_STRING") << "</p>\n";
    // std::cout << "<p>REMOTE_ADDR = " << getenv("REMOTE_ADDR") << "</p>\n";

    std::cout << "</body>\n";
    std::cout << "</html>\n";

    return 0;
}
