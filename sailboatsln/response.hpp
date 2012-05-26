#include <string>
#include "Util.cpp"

using namespace std;

class Response {
	private:
	//copied from http://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/http/server/reply.hpp
		enum status_type
		  {
			ok = 200,
			created = 201,
			accepted = 202,
			no_content = 204,
			multiple_choices = 300,
			moved_permanently = 301,
			moved_temporarily = 302,
			not_modified = 304,
			bad_request = 400,
			unauthorized = 401,
			forbidden = 403,
			not_found = 404,
			internal_server_error = 500,
			not_implemented = 501,
			bad_gateway = 502,
			service_unavailable = 503
		  } status;
		string content;
	public:
		string getContent();
		Response(string);
		string getHeaders();
};

Response::Response (string page)
{
	content = Util::getFile(Util::docroot + page);
}
//stub, actually construct the header
string Response::getHeaders() {
return "\
HTTP/1.1 200 OK\r\n\
Date: " + Util::make_daytime_string() + " GMT\r\n\
Server: sailboat\r\n\
Connection: close\r\n\
Content-Type: text/html\r\n\r\n"; 
}

string Response::getContent()
{
	return content;
}
