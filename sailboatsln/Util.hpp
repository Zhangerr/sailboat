#ifndef __UTIL_HPP_INCLUDED
#define __UTIL_HPP_INCLUDED
#include <string>
#include <map>
#include "Host.h"
namespace Util {
	extern std::string docroot;
	extern std::string fileExtensions;
	extern int port;
	std::string make_daytime_string();
	std::string getHost(std::string);
	bool exists(std::string);
	std::string getFile(std::string);
	bool parseXml();
	bool parseVH();
	extern map<string,Host> hosts;
	void log(std::string);
	void log(std::string, int);
	extern int loglevel;
	std::string getMime(std::string);
	bool loadMime();
	extern map<string,string> mimemap;
}
#endif
