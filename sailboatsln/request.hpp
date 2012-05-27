using namespace std;

class Request {
	private:
		string host;
		string uri;
		string verb; //http verb e.g get, post, head, etc		
	public:
		Request(string, string);
		Request(string);
		string getHost();
		string getUri();
		string getVerb();
};

Request::Request (string h, string u) {
	host = h;
	uri = h + u;
}
string Request::getUri() {
	return uri;
}
Request::Request(string headers) {
	host = Util::getHost(headers);
	
	stringstream ss(headers);
	string init;
	getline(ss, init);
	if (init.length() < 512) {
		char req[512];
		char v[512];
		sscanf(init.c_str(), "%s %s",v,req);
		verb = v;
		if(string(req) == "/") {
			uri = "/index.html";
		} else {
			//read extension types from file?
			boost::regex uriRegex("/([^\\s]+(\\.(?i)(html|htm))$)");	
			bool match = false;
			match = boost::regex_match(req, uriRegex);
			cout << match << endl;
			uri = req;
		}		
		cout << verb << " " << uri << endl;
	} else {
		cout << "encountered huge request" << endl;
	}
}