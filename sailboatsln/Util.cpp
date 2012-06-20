#include "Util.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>
#include "xml/rapidxml.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <vector>

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
	   	return "";
	}
	/**
	 * Check if a file exists
	 */
	bool exists(string name) {
		ifstream myfile(name.c_str());
		if (myfile.good()) {
			myfile.close();
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
				result += line + "\n";
			}
			ind.close();
			return result;
		} else {
			return "";
		}
	}
	/**http://www.ffuts.org/blog/quick-notes-on-how-to-use-rapidxml/
	 * TODO -- actually check for NULL's in case of segfaults
	 * Current way of adding nodes is inefficient, perhaps consider making it use a for loop and a map instead that stores all the properties as keys
	 * A problem would be integer based properties like port and log level
	 *
	 * Actually, instead of just returning false and exiting if there's an error, a better method would be to just use a default
	 */
	bool parseXml() {
		if (!exists("config.xml")) {
			return false;
		}
		string xml = getFile("config.xml");
		xml_document<> doc;
		
		doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml[0]);
		xml_node<>* cur_node = doc.first_node("settings");
		if(cur_node == NULL) {
			Util::log("settings node not found");
			return false;
		}
		string dr;
		xml_node<>* drnode = cur_node->first_node("DocumentRoot");
		if(drnode == NULL) {
			Util::log("DocumentRoot node not found, using current working directory.");
			dr = ""; //current working directory
			//return false;
		} else {
			 dr = drnode->value();
		}
	    Util::log("DocumentRoot:" + dr);
	    docroot = dr;
		xml_node<>* fxNode = cur_node->first_node("FileExtensions");
		string fileExt;
		if(fxNode == NULL) {
			Util::log("FileExtensions node not found, using default");
			fileExt="html|htm|lua";
		} else {
			fileExt = fxNode->value();
		}
		
		Util::log("FileExtensions:" + fileExt);
		fileExtensions = fileExt;	
		xml_node<>* pN = cur_node->first_node("Port");
		string portnum = pN->value();
		if(pN == NULL) {
			Util::log("Port node not found");
			portnum = "8080";
		} else {
			portnum = pN->value();
		}		
		stringstream ss(portnum);
		ss >> port;	
		Util::log("listening on port " + portnum);
		ss.str("");
		ss.clear();
		xml_node<>* lN = cur_node->first_node("LogLevel");
		string ll;
		if(lN==NULL) {
			Util::log("LogLevel node not found");			
			ll = "0";
		} else {
			ll = lN->value();
		}
		
		ss << ll;
		ss >> loglevel;
		Util::log("Log level is set to " + ll);
	    return true;
	}
		
	bool parseVH()
	{
		string xml = getFile("hosts.xml");
		xml_document<> doc;
		doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml[0]);
		xml_node<>* node = doc.first_node("hosts");		
		for(xml_node<>* i = node->first_node("VirtualHost"); i != 0; i = i->next_sibling("VirtualHost")) {
			Util::log("vhost found:");
			string name = i->first_node("Name")->value();
			if(port != 80) {
				std::stringstream out;
				out << port;
				name = name + ":" + out.str();
			}
			string docroot = i->first_node("DocumentRoot")->value();
			Util::log("Name: " + name);
			Util::log("Root: " + docroot);
			string notFound;
			if (i->first_node("NotFound") != NULL)
			{
				notFound = i->first_node("NotFound")->value();
				Util::log("404 page: " + notFound);
				hosts[name]=Host(name,docroot, notFound);
			}
			else 
			{
				hosts[name]=Host(name,docroot);
			for (xml_node<>* a = i->first_node("ResourceMoved"); a; a = a->next_sibling("ResourceMoved")) {
				string res = a->value();
				Util::log("request for resource " + res + " will return a 410 status");
				hosts[name].moved[res] = true;
			}
			}
		}

		return true;
	}
	map<string,string> mimemap;
	bool isLuaFile(string name) {
		return boost::ends_with(name,".lua");
	}
	//http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/example/http/server3/request_handler.cpp
	bool url_decode(string in, string& out)
	{
	  out.clear();
	  out.reserve(in.size());
	  for (std::size_t i = 0; i < in.size(); ++i)
	  {
		if (in[i] == '%')
		{
		  if (i + 3 <= in.size())
		  {
			int value = 0;
			std::istringstream is(in.substr(i + 1, 2));
			if (is >> std::hex >> value)
			{
			  out += static_cast<char>(value);
			  i += 2;
			}
			else
			{
			  return false;
			}
		  }
		  else
		  {
			return false;
		  }
		}
		else if (in[i] == '+')
		{
		  out += ' ';
		}
		else
		{
		  out += in[i];
		}
	  }
	  return true;
	}

	bool loadMime() {
		//string s = getFile("/etc/mime.types"); //this could just be converted to a static map
		if(!exists("mime.types")) {
			return false;
		}
		//compress_on specifies that adjacent separators are combined into one, i.e \n\n\n becomes just one \n when parsing (that's what i take from output)
		string result;
		string line;
		ifstream ind("mime.types");
		if(ind.is_open()) {
			while(ind.good()) {
				getline(ind,line);
				if(boost::starts_with(line,"#")) { //comments
					continue;
				}			
				vector<string> t;
				vector<string> tt;									
				boost::split( t, line, boost::is_any_of("\t"), boost::token_compress_on );
				if(t.size() > 1) {
					boost::split(tt,t[1],boost::is_any_of(" "),boost::token_compress_on);
					for(vector<string>::size_type j = 0; j < tt.size();j++) {
					//	string temp1 = tt[j];
						string temp = tt[j];
						boost::algorithm::trim(temp);
					//	cout << temp << endl;
						mimemap[temp] = t[0];
					}
				}					
			}
			ind.close();
		}
		
	//for(map<string,string>::iterator i = mimemap.begin();i != mimemap.end(); i++) {
	//		cout << "ext:" << i->first << "<<>>" << i->second <<" :end" << endl;
	//	}
		return true;
	}
	template <typename U, typename K> bool keyExists(map<U,K> a, U look) { 
		if(a.find(look)!=a.end()) {
			return true;
		}
		return false;
	}
	string getMime(string filext) {
		vector<string> v;
		boost::split(v,filext,boost::is_any_of("."),boost::token_compress_on);
		if(v.size()>1) {
			string ext = v[v.size()-1];
			if(keyExists(mimemap,ext)) {
				return mimemap[ext];
			}
		}
		return "application/octet-stream";
	}
}
