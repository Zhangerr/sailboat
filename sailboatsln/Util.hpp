#ifndef __UTIL_HPP_INCLUDED
#define __UTIL_HPP_INCLUDED
#include <string>
namespace Util {
	extern std::string docroot;
	extern std::string fileExtensions;
	extern int port;
	std::string make_daytime_string();
	std::string getHost(std::string);
	bool exists(std::string);
	std::string getFile(std::string);
	bool parseXml();
}
#endif
