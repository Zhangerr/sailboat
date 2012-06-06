#include "request.hpp"
#include "response.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>
Request::Request (string h, string u) {
	host = h;
	uri = h + u;
}
string Request::getUri() {
	return uri;
}
string Request::getVerb() {
	return verb;
}
string Request::getHost() {
	return host;
}
Request::Request(string headers) {
	status = Response::ok;
	host = Util::getHost(headers);
	
	stringstream ss(headers);
	string init;
	getline(ss, init);
	if (init.length() < 512) {
		char creq[512];
		char v[512];
		sscanf(init.c_str(), "%s %s",v,creq);
		string req(creq);
		verb = v;
		if(string(req) == "/") {
			req = "/index";
		}				
		uri = req;
		if(Util::exists(Util::docroot + string(req))) { 				
				status = Response::ok;
		} else {
		//read extension types from file?
		boost::regex uriRegex("/([^\\s]+(\\.(?i)(" + Util::fileExtensions + "))$)");	
		bool match = false;
		match = boost::regex_match(req, uriRegex);
		if (!match)
		{
			boost::regex extRegex("(\\w*)");
			boost::match_flag_type flags = boost::match_default;
			boost::sregex_token_iterator it(Util::fileExtensions.begin(), Util::fileExtensions.end(), extRegex, 1, flags);
			boost::sregex_token_iterator end;
			bool pageExists = false;
			for (; it != end; ++it)
			{
				string res = it->str();
				if (res != "")
				{
					string path = Util::docroot + string(req) + "." + res;
					if (Util::exists(path))
					{
						uri = string(req) + "." + res;
						status = Response::ok;
						pageExists = true;
						break;
					}
				}
			}
			if (!pageExists)
			{
				status = Response::not_found;
			}
		}
		else
		{
			//uri = req;
			//if(Util::exists(Util::docroot + string(req))) { 				
			//	status = Response::ok;
			//} else {
				status = Response::not_found;
			//}
		}	
		}		
	//	cout << verb << " " << uri << endl;
	} else {
		cout << "encountered huge request" << endl;
	}
}
