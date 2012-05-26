#include <string>
#include <sstream>
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
		uri = req;
		}
		cout << uri << endl;
	} else {
		cout << "encountered huge request" << endl;
	}
}