#include "Util.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "xml/rapidxml.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <ctime>

namespace Util {
	using namespace std;
	using namespace rapidxml;
	string docroot;
	string fileExtensions;
	map<string,Host> hosts;
	int port;
	int loglevel = 1;
	void log(string a) {
		log(a,0);
	}
	void log(string a, int lvl) {
		if(loglevel>=lvl){
			cout << a << endl;
		}
	}
	string make_daytime_string()
	{
	  time_t now = time(0);
	  string tm = ctime(&now);
	  
	  boost::algorithm::trim(tm);
	  return tm;	  
	}
	string getHost(string headers) {
			//Host: x is the host, used for virtual named hosts and such
			//GET / is page to load, regex isnt working so just subbing here with hardcode for now
			//alex perhaps come up with a better regex here, atm have to substr to cut "Host: " <- not anymore =D
		
			//( ) is used to create a capture group/sub expression
			boost::regex hostRegex("Host: (.*)");
			//important, otherwise . matches new line which we _don't_ want
			boost::match_flag_type flags = boost::match_default | boost::match_not_dot_newline; 
			/*
			refer to these documents --
			http://goo.gl/QpvrI
			http://goo.gl/8iMU0
			*/
			boost::sregex_token_iterator it(headers.begin(), headers.end(), hostRegex, 1, flags);
			
	   		boost::sregex_token_iterator end;
	  		for (; it != end; ++it) {
		  		string res = it->str();
		  		boost::algorithm::trim(res); //get rid of carriage return
	       		return res;
	   		}
	}
	
	bool exists(string name) {
		ifstream myfile(name.c_str());
		if (myfile.good()) {
			return true;
		}
		return false;
	}
	string getFile(string fn) {
		string result;
		string line;
		ifstream ind(fn.c_str());
		if(ind.is_open()) {
			while(ind.good()) {
				getline(ind,line);
				result += line + "\r\n";
			}
			return result;
		} else {
			return "";
		}
	}
	//http://www.ffuts.org/blog/quick-notes-on-how-to-use-rapidxml/
	bool parseXml() {
		if (!exists("config.xml")) {
			return false;
		}
		string xml = getFile("config.xml");
		xml_document<> doc;
		doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml[0]);
		xml_node<>* cur_node = doc.first_node("settings");
	    string dr = cur_node->first_node("DocumentRoot")->value();
	    Util::log("DocumentRoot:" + dr);
	    docroot = dr;
		string fileExt = cur_node->first_node("FileExtensions")->value();
		Util::log("FileExtensions:" + fileExt);
		fileExtensions = fileExt;	
		string portnum = cur_node->first_node("Port")->value();
		stringstream ss(portnum);
		ss >> port;	
		Util::log("listening on port " + portnum);
		ss.str("");
		ss.clear();
		string ll = cur_node->first_node("LogLevel")->value();
		ss << ll;
		ss >> loglevel;
		Util::log("Log level is set to " + ll);
		for(xml_node<>* i = cur_node->first_node("VirtualHost"); i; i = i->next_sibling("VirtualHost")) {
			Util::log("vhost found:");
			string name = i->first_node("Name")->value();
			if(port != 80) {
				name = name + ":" + portnum;
			}
			string docroot = i->first_node("DocumentRoot")->value();
			Util::log("Name: " + name);
			Util::log("Root: " + docroot);
			//hosts.insert(map<string,Host>::value_type(name,Host(name,docroot)));
			hosts[name]=Host(name,docroot);
			//cout << hosts[name].getHost() << endl;
			//=\
			//for(string::reverse_iterator j = name.rbegin(); j < name.rend();j++) {
				//cout << *j;
			//}
		//	hosts[name] = Host(name,docroot);
		}
	    //could use attribute instead, whitespace could be an issue using values
	    return true;
	}	
}
