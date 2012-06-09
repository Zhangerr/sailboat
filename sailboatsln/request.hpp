#ifndef __REQUEST_HPP_INCLUDED
#define __REQUEST_HPP_INCLUDED
#include <string>
#include <boost/regex.hpp>
#include "Util.hpp"
using namespace std;
class Request {
	private:
		string host;
		string uri;
		string verb; //http verb e.g get, post, head, etc		
	public:
		int status;
		Request(string, string);
		Request(string);
		string getHost();
		string getUri();
		string getVerb();
};
#endif