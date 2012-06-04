#include <string>
#include "request.hpp"
#include "Util.hpp"
using namespace std;

class Response {
	private:	
		string content;
		string headers;
	public:
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
		string getContent();
		Response(string, status_type);
		string generateHeaders(status_type);
		string getHeaders();
		string getPage();
};
Response getResponse(Request);
