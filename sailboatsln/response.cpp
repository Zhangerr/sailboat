#include "response.hpp"
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
bool returnContent = true;
Response getResponse(Request req)
{		
	if (req.getVerb() == "HEAD")
		returnContent = false;
	return Response(req.getUri(), (Response::status_type)req.status,req.getHost());
}

Response::Response (string page, status_type status, string host)
{
	string root = Util::docroot;
	bool moved = false;
	if(Util::hosts.find(host) != Util::hosts.end()) {
		Util::log("vhost " + Util::hosts[host].getHost() + " found for " + host,2);
		root = Util::hosts[host].getRoot();
		moved = Util::hosts[host].moved[page];
	} else {
		Util::log("vhost not found for " + host,2);
	}
	string reqpage = root + page;
	if (!moved)
	{
		if(Util::exists(reqpage)) {
			content = Util::getFile(reqpage);
		} else {
			Util::log("Resource " + reqpage + " not found",2);
			if (Util::hosts[host].has404())
			{
				reqpage = root + "/" + Util::hosts[host].getNotFound();
				Util::log("Custom 404 available for " + host + ": " + reqpage, 2);
				if (Util::exists(reqpage))
				{
					content = Util::getFile(reqpage);
				}
				else
				{
					content = "404 Page Not Found. \r\nAdditionally a 404 Not Found error occured while getting the 404 Not Found Page.";
					Util::log("The custom 404 for " + host + " was not found.");
				}
			}
			else
				content = "404 Page Not Found";
		}
	}
	else
	{
		status = Response::moved_permanently;
		content = "410 Moved Permanently";
	}
	headers = generateHeaders(status);
}

//stub, actually construct the header
string Response::generateHeaders(status_type status) {
	//cout << status_strings::getStatus(status) << endl;
	string res = status_strings::getStatus(status) + "Date: "
	+ Util::make_daytime_string() + " GMT\r\n\
	Server: sailboat\r\n\
	Connection: close\r\n\
	";
	if (returnContent)
		res = res + "Content-Type: text/html\r\n\r\n"; 
	return res;
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
	return headers + (returnContent?content:"");
}