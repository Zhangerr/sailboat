#include <string>
#include <boost/regex.hpp>
#ifndef __UTIL_HPP_INCLUDED
#define __UTIL_HPP_INCLUDED
#include "Util.hpp"
#endif
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
