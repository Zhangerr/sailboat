namespace Util {
	using namespace std;
	using namespace rapidxml;
	string docroot;
	string make_daytime_string()
	{
	  time_t now = time(0);
	  return ctime(&now);
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
	    cout << "DocumentRoot:" << dr << endl;
	    docroot = dr;
	    //could use attribute instead, whitespace could be an issue using values
	    return true;
	}
	
}
