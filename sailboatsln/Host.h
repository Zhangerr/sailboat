#ifndef __HOST_HPP_INCLUDED
#define __HOST_HPP_INCLUDED
#include <string>
#include <map>
using namespace std;
class Host {
	private:
		string host;
		string root;
		string notFoundPage;
		bool _has404;
	public:
		string getHost();
		string getRoot();		
		string getNotFound();
		map<string,bool> moved;
		bool has404();
		Host(string a, string b) : host(a), root(b) {_has404 = false;}
		Host(string a, string b, string c) : host(a),root(b),notFoundPage(c){_has404 = true;}	
		Host(); //needed for map apparently
};
#endif