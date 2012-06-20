#ifndef __REQUEST_HPP_INCLUDED
#define __REQUEST_HPP_INCLUDED
#include <string>
#include <boost/regex.hpp>
#include "Util.hpp"
#include <vector>
#include <map>
using namespace std;
class Request {
	private:
		string host;
		string uri;
		string verb; //http verb e.g get, post, head, etc		
	public:
		int status;
		map<string,string> GetParams;
		Request(string, string);
		Request(string);
		string getHost();
		string getUri();
		string getVerb();
};
#endif