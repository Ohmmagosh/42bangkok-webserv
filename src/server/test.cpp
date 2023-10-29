
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <ostream>
#include <vector>
#include "../stdlib.hpp"
#include "Response.hpp"

class Request {
	private:
		std::string							_method;
		std::string							_url;
		std::string							_version;
		std::string							_body;
		std::string							_boundary;
		std::vector<std::string>			_headers_no_c;
		std::map<std::string, std::string>	_headers;
		std::map<std::string, std::string>	_query_url;

	public:
		Request();
		Request(const std::string& raw_req);
		~Request();

		//getter
		std::string									getMethod() const;
		std::string									getBody() const;
		std::string									getUrl() const;
		std::string									getVersion() const;
		std::string									getBoundary() const;
		const std::vector<std::string>&				getHeaderNoC() const;
		std::string									getHeadersByValue(const std::string& key);
		const std::map<std::string, std::string>&	getHeaderC() const;
		const std::map<std::string, std::string>&	getQueryUrl() const;
		std::string									getQueryUrlByValue(const std::string& val);

		//helper
		bool										validateHeaderMethod(const std::string& line);
		void										validateHeadersCandNoC(const std::string& line);
		bool										validateBoundary();
		bool										validateMultiPart();
		bool										validateParams();
		bool										validateBoundaryBody() const;
		bool										validateBoundaryHeader() const;

		//setter
		void										setBoundaryFromContent(const std::string& content);
		void										setBoundary(const std::string& boundary);
		void										setHeaderMethod(const std::string& line);
		void										setMethod(const std::string& method);
		void										setBody(const std::string& body);
		void										setUrl(const std::string& method);
		void										setVersion(const std::string& method);
		void										setHeaderC(const std::string& line);
		void										setHeaderNoC(const std::string& line);
		void										setHeaderAndBody(const std::string& raw_req);
		void										setAllHeader(const std::string& header);
		void										setAllBody(const std::string& body);
		void										setQueryUrl();


};

std::ostream& operator<<(std::ostream& os, const Request& req);

Request::Request() {

}

Request::Request(const std::string& raw_req) {

	this->setHeaderAndBody(raw_req);
}

Request::~Request() {

}

//GETTER
std::string	Request::getMethod() const {
	return this->_method;
}


std::string	Request::getUrl() const {
	return this->_url;
}

std::string	Request::getVersion() const {
	return this->_version;
}

std::string	Request::getBoundary() const {
	return this->_boundary;
}

const std::map<std::string, std::string>&	Request::getHeaderC() const {
	return this->_headers;
}

const std::vector<std::string>&	Request::getHeaderNoC() const {
	return this->_headers_no_c;
}

std::string	Request::getHeadersByValue(const std::string& key) {
	return this->_headers[key];
}

std::string	Request::getBody() const {
	return this->_body;
}

const std::map<std::string, std::string>&	Request::getQueryUrl() const {
	return this->_query_url;
}

std::string	Request::getQueryUrlByValue(const std::string& val){
	return this->_query_url[val];
}

//SETTER
void	Request::setMethod(const std::string& method) {
	this->_method = method;
}

void	Request::setUrl(const std::string& url) {
	this->_url = url;
}

void	Request::setVersion(const std::string& version) {
	this->_version = version;
}


void	Request::setBoundary(const std::string& boundary) {
	this->_boundary = boundary;
}

void	Request::setBody(const std::string& body) {
	this->_body = body;
}

void	Request::setAllHeader(const std::string& header) {
	std::string	buff;
	std::stringstream	ss(header);

	while(std::getline(ss, buff)) {
		if (this->validateHeaderMethod(buff))
			this->setHeaderMethod(buff);
		else
			this->validateHeadersCandNoC(buff);
	}
	if (this->validateBoundary())
		this->setBoundaryFromContent(this->getHeadersByValue("Content-Type"));
	if (this->validateParams())
		this->setQueryUrl();

}

void	Request::setAllBody(const std::string& body) {
	this->setBody(body);
}

void	Request::setHeaderAndBody(const std::string& raw_req) {
	std::vector<std::string>	sp = Uti::splite(raw_req, "\r\n\r\n");

	this->setAllHeader(sp[0]);
	this->setAllBody(sp[1]);
}

void	Request::setQueryUrl() {
	std::string	url = this->getUrl();
	if (url.empty())
		return ;
	if (url.find("?") != std::string::npos) {
		std::vector<std::string>	sp = Uti::splite(url, "?");
		if (sp[1].empty())
			return ;
		std::vector<std::string>	sa = Uti::splite(sp[1], "&");
		for (size_t i = 0; i < sa.size(); i++) {
			std::vector<std::string> se = Uti::splite(sa[i], "=");
			this->_query_url[se[0]] = se[1];
		}
	}
	return ;
}

void	Request::setHeaderC(const std::string& line) {
	std::vector<std::string>	sp;
	std::stringstream			ss;
	sp = Uti::spliteHeader(line, ":");
	this->_headers[sp[0]] = sp[1];
}

void	Request::setHeaderNoC(const std::string& line) {
	this->_headers_no_c.push_back(line);
}

void	Request::setHeaderMethod(const std::string& line) {
	std::string	method;
	std::string	url;
	std::string	version;
	std::stringstream	ss(line);

	ss >> method >> url >> version;
	this->setMethod(method);
	this->setUrl(url);
	this->setVersion(version);
	return ;
}

void	Request::setBoundaryFromContent(const std::string& content) {
	std::vector<std::string>	sp = Uti::splite(Uti::trim(content, " "), " ");
	std::vector<std::string>	bp = Uti::splite(sp[1], "=");
	this->setBoundary(bp[1]);
}

// HELPER
bool	Request::validateBoundary() {
	std::string	content_type = this->getHeadersByValue("Content-Type");
	if (content_type.find("boundary") != std::string::npos)
		return true;
	return false;
}

bool	Request::validateMultiPart() {
	std::string	content_type = this->getHeadersByValue("Content-Type");
	if (content_type.find("multipart/form-data") != std::string::npos)
		return true;
	return false;
}


void	Request::validateHeadersCandNoC(const std::string& line) {
	std::vector<std::string>	sp;
	if (line.find(":") != std::string::npos) {
		sp = Uti::splite(line, ":");
		this->setHeaderC(line);
	}else {
		this->setHeaderNoC(line);
	}
}

bool	Request::validateHeaderMethod(const std::string& line) {
	if (line.find("GET") != std::string::npos)
		return true;
	else if (line.find("POST") != std::string::npos)
		return true;
	else if (line.find("DELETE") != std::string::npos)
		return true;
	return false;
}

bool	Request::validateBoundaryBody() const {
	std::string		body = this->getBody();
	std::string			boundary = this->getBoundary();
	if (body.empty() || boundary.empty())
		return false;
	std::stringstream	ss(body);
	std::string			buff;
	while(std::getline(ss, buff)) {
		if (Uti::trim(buff, "-\r\n").compare(Uti::trim(boundary, "-\r\n")) == 0)
			return true;
	}
	return false;
}

bool	Request::validateParams() {
	std::string	url = this->getUrl();
	if (url.empty())
		return false;
	if (url.find("?"))
		return true;
	return false;
}






std::ostream&	operator<<(std::ostream& os, const Request& req) {
	std::map<std::string, std::string>::const_iterator	it = req.getHeaderC().begin();
	std::map<std::string, std::string>					query = req.getQueryUrl();
	os << "----------------------ostream---------------------" << std::endl;
	os << "---------------------[HEADER]---------------------" << std::endl;
	os << "Method : " << req.getMethod() << " Url : " << req.getUrl() << " Version : " << req.getVersion() << std::endl;
	for (size_t i = 0; i < req.getHeaderNoC().size(); i++) {
		os << req.getHeaderNoC()[i] << std::endl;
	}
	while (it != req.getHeaderC().end()) {
		os << it->first << ":" << it->second << std::endl;
		it++;
	}
	os << "boundary : " << req.getBoundary() << std::endl;
	if (!query.empty()) {
		std::map<std::string, std::string>::const_iterator	qit = query.begin();
		while (qit != query.end()) {
			os << "[QUERY] : " << qit->first << " = " << qit->second << std::endl;
			qit++;
		}
	}
	else
		os << "[NO QUERY PARAMS]" << std::endl;
	os << "-------------------[END HEADER]-------------------" << std::endl;
	os << "---------------------[BODY]---------------------" << std::endl;
	if (req.getBody().empty())
		os << "BODY IS EMPTY" << std::endl;
	else
		os << req.getBody() << std::endl;
	os << "---------------------[END]---------------------" << std::endl;
	os << "--------------------end ostream-------------------" << std::endl;
	return os;
}


class Store {
	private:

	public:
		Store();
		~Store();
}
//Host

class HttpRequestHandle {
	private:

	public:
		HttpRequestHandle();
		~HttpRequestHandle();

		// Helper
		void	validateMethod(const Request& req, const Store& st);
		const std::string&	RequestHandler();

		void				getMethod(const Request& req);
		void				postMethod(const Request& req);
		void				delMethod(const Request& req);
		void				setResponse();
		const std::string&	response();

};

HttpRequestHandle::HttpRequestHandle() {

}
HttpRequestHandle::~HttpRequestHandle() {

}

const std::string&	HttpRequestHandle::RequestHandler() {

}


void	HttpRequestHandle::validateMethod(const Request& req, const Store& st) {
	if (req.getMethod() == "GET") {
		this->getMethod(req);
	}
	else if (req.getMethod() == "POST") {
		this->getMethod(req);
	}
	else if (req.getMethod() == "DELETE") {
		this->getMethod(req);
	}
	else {
		this->setResponse();
	}
	return ;
}

void	HttpRequestHandle::getMethod(const Request& req) {
	if (req.getUrl() == "")
		this->setResponse();
	return ;
}

void	HttpRequestHandle::postMethod(const Request& req) {
	if (req.getUrl() == "")
		this->setResponse();
	return ;
}

void	HttpRequestHandle::delMethod(const Request& req) {
	if (req.getUrl() == "")
		this->setResponse();
	return ;
}

void	HttpRequestHandle::setResponse() {

}

// argv make index null
// env header[0-n], query
//execve()




int main() {
	Store	st;
	std::string raw_req_post = "POST /upload-file HTTP/1.1\r\n"
							  "Host: localhost:8080\r\n"
							  "Connection: keep-alive\r\n"
							  "Content-Length: 4202\r\n"
							  "sec-ch-ua: \"Chromium\";v=\"118\", \"Google Chrome\";v=\"118\", \"Not=A?Brand\";v=\"99\"\r\n"
							  "sec-ch-ua-platform: \"macOS\"\r\n"
							  "sec-ch-ua-mobile: ?0\r\n"
							  "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36\r\n"
							  "Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryyt9qQAQQ1iOckn1M\r\n"
							  "Accept: */*\r\n"
							  "Origin: http://localhost:8080\r\n"
							  "Sec-Fetch-Site: same-origin\r\n"
							  "Sec-Fetch-Mode: cors\r\n"
							  "Sec-Fetch-Dest: empty\r\n"
							  "Referer: http://localhost:8080/\r\n"
							  "Accept-Encoding: gzip, deflate, br\r\n"
							  "Accept-Language: en-US,en;q=0.9,th;q=0.8\r\n"
							  "\r\n";

	std::cout << "-------------------------START---------------------------" << std::endl;
	std::cout << raw_req_post << std::endl;
	Request		req(raw_req_post);
	std::cout << req << std::endl;
	std::cout << "--------------------------END----------------------------" << std::endl;

	std::string	raw_req_post2 = "------WebKitFormBoundaryyt9qQAQQ1iOckn1M\r\n"
								"Content-Disposition: form-data; name=\"fileToUpload\"; filename=\"test.txt\"\r\n"
								"Content-Type: text/plain\r\n"
								"\r\n"
								"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse convallis ipsum at massa faucibus, vitae varius nunc tristique. Vivamus lacinia, tellus in congue semper, magna mauris fringilla sapien, eu tincidunt risus purus sit amet nisl. Suspendisse ac nisi imperdiet, aliquam nulla et, maximus lacus. Nulla aliquam bibendum convallis. Sed aliquet bibendum ante. Suspendisse consequat orci non blandit viverra. Donec at nibh sed est fringilla maximus. Suspendisse potenti. Morbi erat nibh, aliquet et massa ac, eleifend sollicitudin erat. Sed viverra, lectus in commodo rhoncus, urna velit laoreet quam, sed porta velit dui ut ligula.\n"

								"Integer luctus semper est. Fusce id bibendum nibh. Quisque suscipit, sem non ornare sagittis, mi lorem porttitor eros, non suscipit felis odio sed libero. Praesent lobortis ullamcorper urna nec vulputate. Curabitur lacinia tempor suscipit. Proin fermentum, tortor ac accumsan gravida, nulla libero egestas justo, nec bibendum sapien lorem in purus. Etiam eleifend sollicitudin lectus non mattis. Curabitur laoreet orci ultrices, cursus ipsum ac, euismod magna.\n"

								"Aliquam id neque non augue fermentum faucibus. Vestibulum fermentum nibh eu arcu sagittis, nec tincidunt dolor feugiat. Mauris sit amet suscipit magna. Nam sodales, quam a accumsan rhoncus, felis tortor elementum nisl, nec lobortis nisi metus non purus. Quisque hendrerit, turpis a efficitur tempor, mi ante laoreet urna, ac feugiat sem ante vel eros. Proin tristique finibus ipsum in elementum. Suspendisse justo mauris, mattis ut nisl in, fermentum bibendum tortor. Donec consectetur dolor ante, sed dictum felis vulputate sit amet. Vestibulum at neque a mauris tempor vestibulum a et leo. Suspendisse potenti.\n"

								"Vestibulum sed purus non ipsum cursus mattis eget vitae massa. Pellentesque odio ipsum, aliquet id placerat quis, malesuada in justo. Nulla volutpat egestas erat, et ullamcorper nibh cursus eget. Suspendisse ut quam porttitor, rutrum dui in, pulvinar urna. Phasellus sit amet bibendum eros, in facilisis sem. Nunc luctus eget augue ut varius. Maecenas sed convallis ligula. Curabitur molestie pulvinar nulla, eu rutrum enim convallis nec. Curabitur sit amet turpis vitae urna dictum ultrices. Sed lacinia dictum nulla vel auctor. Fusce ac commodo tortor. In posuere augue vel tincidunt lobortis.\n"

								"Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Phasellus eu nisi felis. Fusce lobortis id eros commodo aliquam. Praesent scelerisque malesuada magna, vitae aliquam massa dignissim sit amet. Morbi libero diam, porttitor ac ullamcorper non, elementum dictum purus. Maecenas vulputate quam in ligula semper, eu lobortis erat sodales. Duis sit amet tortor vel eros tincidunt tempor. Quisque ac tempor felis, a placerat metus. Nulla rutrum malesuada magna, ut malesuada justo semper quis. Etiam tincidunt hendrerit neque. Aliquam laoreet ligula ex, id malesuada ex scelerisque at.\n"

								"Quisque consequat blandit venenatis. Fusce tempor lorem et urna semper imperdiet. Sed vestibulum purus ac enim cursus, luctus tempor elit imperdiet. Quisque ut tellus quis est commodo fringilla. Mauris malesuada iaculis mi, ac volutpat felis venenatis vel. Sed ac molestie diam. Aliquam erat volutpat. Pellentesque sed lectus metus. Nulla pharetra sem eu magna volutpat, ac euismod purus sagittis. Mauris ullamcorper pellentesque dolor. Proin id sapien non nisl interdum mattis. Mauris tristique porta dolor, eu mollis erat pulvinar et. Integer id convallis tortor.\n"

								"Donec bibendum mattis lobortis. Fusce tellus ligula, bibendum et elit a, tincidunt luctus diam. Duis nec iaculis ligula. Donec augue dui, viverra quis justo quis, consequat elementum orci. Nullam sed lobortis elit, id dapibus lacus. Nullam rhoncus justo in lacus suscipit tempus. Fusce tincidunt lobortis libero. Fusce egestas, mauris in ornare facilisis, elit risus vestibulum ante, suscipit lacinia odio purus vel ligula. Nullam in luctus felis, eget sollicitudin diam. Interdum et malesuada fames ac ante ut.\n"
								"------WebKitFormBoundaryyt9qQAQQ1iOckn1M--\r\n";

	std::cout << "-------------------------START---------------------------" << std::endl;
	Request		req_post2(raw_req_post2);
	std::cout << req_post2 << std::endl;
	std::cout << "--------------------------END----------------------------" << std::endl;

	std::string	raw_req_get =	"GET /favicon.ico HTTP/1.1:\r\n"
								"Host: localhost:8080\r\n"
								"Connection: keep-alive\r\n"
								"sec-ch-ua: \"Chromium\";v=\"118\", \"Google Chrome\";v=\"118\", \"Not=A?Brand\";v=\"99\"\r\n"
								"sec-ch-ua-mobile: ?0\r\n"
								"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36\r\n"
								"sec-ch-ua-platform: \"macOS\"\r\n"
								"Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\n"
								"Sec-Fetch-Site: same-origin\r\n"
								"Sec-Fetch-Mode: no-cors\r\n"
								"Sec-Fetch-Dest: image\r\n"
								"Referer: http://localhost:8080/\r\n"
								"Accept-Encoding: gzip, deflate, br\r\n"
								"Accept-Language: en-US,en;q=0.9,th;q=0.8\r\n"
								"\r\n";

	std::cout << "-------------------------START---------------------------" << std::endl;
	Request		req3(raw_req_get);
	std::cout << req3 << std::endl;
	std::cout << "--------------------------END----------------------------" << std::endl;


	std::cout << "jkfhdasjklflkhdsa" << std::endl;
	std::string	raw_req_post3 = "POST /submit-data HTTP/1.1"
								"Host: localhost:8080"
								"Connection: keep-alive"
								"Content-Length: 15"
								"Cache-Control: max-age=0"
								"sec-ch-ua: \"Chromium\";v=\"118\", \"Google Chrome\";v=\"118\", \"Not=A?Brand\";v=\"99\""
								"sec-ch-ua-mobile: ?0\r\n"
								"sec-ch-ua-platform: \"macOS\"\r\n"
								"Upgrade-Insecure-Requests: 1\r\n"
								"Origin: http://localhost:8080\r\n"
								"Content-Type: application/x-www-form-urlencoded\r\n"
								"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36\r\n"
								"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
								"Sec-Fetch-Site: same-origin\r\n"
								"Sec-Fetch-Mode: navigate\r\n"
								"Sec-Fetch-User: ?1\r\n"
								"Sec-Fetch-Dest: document\r\n"
								"Referer: http://localhost:8080/\r\n"
								"Accept-Encoding: gzip, deflate, br\r\n"
								"Accept-Language: en-US,en;q=0.9,th;q=0.8\r\n"
								"\r\n"
								"hello=dfasfdsaf\r\n";

	std::cout << "-------------------------START---------------------------" << std::endl;
	Request		req_post3(raw_req_post3);
	std::cout << req_post3 << std::endl;
	std::cout << "--------------------------END----------------------------" << std::endl;

	std::string	raw_req_del = 	"DELETE /delete-data?hello=world&wow=hello HTTP/1.1\r\n"
								"Host: localhost:8080\r\n"
								"Connection: keep-alive\r\n"
								"sec-ch-ua: \"Chromium\";v=\"118\", \"Google Chrome\";v=\"118\", \"Not=A?Brand\";v=\"99\"\r\n"
								"sec-ch-ua-mobile: ?0\r\n"
								"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36\r\n"
								"sec-ch-ua-platform: \"macOS\"\r\n"
								"Accept: */*\r\n"
								"Origin: http://localhost:8080\r\n"
								"Sec-Fetch-Site: same-origin\r\n"
								"Sec-Fetch-Mode: cors\r\n"
								"Sec-Fetch-Dest: empty\r\n"
								"Referer: http://localhost:8080/\r\n"
								"Accept-Encoding: gzip, deflate, br\r\n"
								"Accept-Language: en-US,en;q=0.9,th;q=0.8\r\n"
								"\r\n";
	std::cout << "-------------------------START---------------------------" << std::endl;
	Request		req_del(raw_req_del);
	std::cout << req_del << std::endl;
	std::cout << "--------------------------END----------------------------" << std::endl;
	std::cout << "-------------------------START---------------------------" << std::endl;
	std::string test = "dfsfds";
	std::cout << test.empty() << std::endl;
	std::cout << "--------------------------END----------------------------" << std::endl;
	std::cout << "-------------------------START---------------------------" << std::endl;
	std::string	test = "multipart/form-data; boundary=----WebKitFormBoundaryyt9qQAQQ1iOckn1M";
	std::vector<std::string>	sp = Uti::splite(test, " ");
	std::cout << "sp[0] : " << sp[0] << std::endl;
	std::cout << "sp[1] : " << sp[1] << std::endl;
	std::vector<std::string>	bp = Uti::splite(sp[1], "=");
	std::cout << "bp[0] : " << bp[0] << std::endl;
	std::cout << "bp[1] : " << bp[1] << std::endl;


	std::cout << "--------------------------END----------------------------" << std::endl;
	std::cout << "-------------------------START---------------------------" << std::endl;

	std::string	hello = "-----------hello\r\n";
	std::string	test = "fjdsalkfjlkdsja;lfj;lkdjsaf;ljasl;kfjlkdajsf;lk\r\n"
						"fkjdsalkfjlkjsdalk;fjlkdjsaflkjkldsajfkljsadflkj\r\n"
						"------hello------\r\n";
	std::stringstream	ss(test);
	std::string			buff;
	std::string			tb = Uti::trim(hello, "-\n\r");
	while (std::getline(ss, buff)) {
		// std::cout << " trim : " << Uti::trim(buff, "-\n\r") << std::endl;
		if (tb.compare(Uti::trim(buff, "-\n\r")) == 0)
			std::cout << "It Find" << std::endl;
		else
			std::cout << "NO can not find" << std::endl;
	}

	std::cout << "--------------------------END----------------------------" << std::endl;
	return 0;
}

// int	main() {
// 	pid_t pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }

//     if (pid == 0) {  // Child process
//         char *argv[] = {"/path/to/program", NULL};
//         char *envp[] = {NULL};
//         if (execve(argv[0], argv, envp) == -1) {
//             perror("execve");
//             exit(EXIT_FAILURE);  // Exit with failure status if execve fails
//         }
//         // No need to exit on success; execve replaces the process
//     } else {  // Parent process
//         int status;
//         wait(&status);  // Wait for child process to finish

//         if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
//             std::cout << "Child process succeeded" << std::endl;
//         } else {
//             std::cout << "Child process failed" << std::endl;
//         }
//     }

//     return 0;
// }

// if (pid == 0) {  // Child process
//     close(pipefd[1]);  // Close unused write end

//     // Save the original stdout file descriptor
//     int stdout_save = dup(STDOUT_FILENO);

//     // Redirect stdout to write to the pipe
//     dup2(pipefd[1], STDOUT_FILENO);

//     // Write the string to stdout (which is now the pipe)
//     std::cout << "hello" << std::endl;

//     // Restore stdout to its original file descriptor
//     dup2(stdout_save, STDOUT_FILENO);
//     close(stdout_save);  // Close the saved stdout file descriptor

//     // Redirect stdin to read from the pipe
//     dup2(pipefd[0], STDIN_FILENO);
//     close(pipefd[0]);  // Close original read end

//     char *argv[] = {"./read_stdin", NULL};  // Path to the read_stdin program
//     char *envp[] = {NULL};
//     execve(argv[0], argv, envp);  // Execute the read_stdin program

//     // execve only returns on error
//     perror("execve");
//     exit(EXIT_FAILURE);
// }

// int	main() {
// 	int		pipe_fd[2];

// 	if (pipe(pipe_fd) == -1) {
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}
// 	pid_t	pid = fork();
// 	if (pid == 0) { //child
// 		if (execve({NULL}))
// 	} else { //parent
// 		close(pipe_fd[1]);
// 		std::string	res;
// 		char	buff[4096];
// 		size_t	byteRead;

// 		while((byteRead = read(pipe_fd[0], buff, sizeof(buff))) > 0) {
// 			res.append(buff, byteRead);
// 		}
// 		close(pipe_fd[0]);
// 		waitpid(pid, NULL, 0);
// 		std::cout << res << std::endl;
// 	}
// }
