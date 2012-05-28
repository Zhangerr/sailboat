#include "response.hpp"
#include "request.cpp"
//namespace copied and edited from http://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/http/server/reply.cpp
namespace status_strings {

const std::string ok =
  "HTTP/1.1 200 OK\r\n";
const std::string created =
  "HTTP/1.1 201 Created\r\n";
const std::string accepted =
  "HTTP/1.1 202 Accepted\r\n";
const std::string no_content =
  "HTTP/1.1 204 No Content\r\n";
const std::string multiple_choices =
  "HTTP/1.1 300 Multiple Choices\r\n";
const std::string moved_permanently =
  "HTTP/1.1 301 Moved Permanently\r\n";
const std::string moved_temporarily =
  "HTTP/1.1 302 Moved Temporarily\r\n";
const std::string not_modified =
  "HTTP/1.1 304 Not Modified\r\n";
const std::string bad_request =
  "HTTP/1.1 400 Bad Request\r\n";
const std::string unauthorized =
  "HTTP/1.1 401 Unauthorized\r\n";
const std::string forbidden =
  "HTTP/1.1 403 Forbidden\r\n";
const std::string not_found =
  "HTTP/1.1 404 Not Found\r\n";
const std::string internal_server_error =
  "HTTP/1.1 500 Internal Server Error\r\n";
const std::string not_implemented =
  "HTTP/1.1 501 Not Implemented\r\n";
const std::string bad_gateway =
  "HTTP/1.1 502 Bad Gateway\r\n";
const std::string service_unavailable =
  "HTTP/1.1 503 Service Unavailable\r\n";

string getStatus(Response::status_type status)
{
//	cout << status << endl;
//	cout << Response::ok << endl;
  switch (status)
  {
  case Response::ok:
    return ok;
  case Response::created:
    return created;
  case Response::accepted:
    return accepted;
  case Response::no_content:
    return no_content;
  case Response::multiple_choices:
    return multiple_choices;
  case Response::moved_permanently:
    return moved_permanently;
  case Response::moved_temporarily:
    return moved_temporarily;
  case Response::not_modified:
    return not_modified;
  case Response::bad_request:
    return bad_request;
  case Response::unauthorized:
    return unauthorized;
  case Response::forbidden:
    return forbidden;
  case Response::not_found:
    return not_found;
  case Response::internal_server_error:
    return internal_server_error;
  case Response::not_implemented:
    return not_implemented;
  case Response::bad_gateway:
    return bad_gateway;
  case Response::service_unavailable:
    return service_unavailable;
  default:
    return internal_server_error;
  }
}
}
Response getResponse(Request req)
{	
	return Response(req.getUri(), req.status);
}
string extensions[] = {".html", ".htm"};

Response::Response (string page, status_type status)
{
	string reqpage = Util::docroot + page;
	if(Util::exists(reqpage)) {
		content = Util::getFile(reqpage);
	} else {
		content = "404 Page Not Found :(";
	}
	headers = generateHeaders(status);
}
//stub, actually construct the header
string Response::generateHeaders(status_type status) {
	//cout << status_strings::getStatus(status) << endl;
return status_strings::getStatus(status) + "Date: "
+ Util::make_daytime_string() + " GMT\r\n\
Server: sailboat\r\n\
Connection: close\r\n\
Content-Type: text/html\r\n\r\n"; 
}
string Response::getHeaders()
{
	return headers;
}
string Response::getContent()
{
	return content;
}
string Response::getPage()
{
	return headers + content;
}