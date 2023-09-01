#include <iostream>
#include <string>
#include <cstdlib> // for getenv()

using namespace std;

int main()
{
    // Output the HTTP header
    cout << "Content-type: text/html\r\n\r\n";

    // Output the HTML content
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>CGI C++ Program</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<h1>Hello, world!</h1>\n";

    // Read and display environment variables
    // cout << "<p>QUERY_STRING = " << getenv("QUERY_STRING") << "</p>\n";
    // cout << "<p>REMOTE_ADDR = " << getenv("REMOTE_ADDR") << "</p>\n";

    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}