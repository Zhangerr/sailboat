#ifndef __HOST_HPP_INCLUDED
#define __HOST_HPP_INCLUDED
#include <string>
using namespace std;
class Host {
	private:
		string host;
		string root;
	public:
		string getHost();
		string getRoot();		
		Host(string a, string b) : host(a),root(b){}	
		Host(); //needed for map apparently
};
#endif