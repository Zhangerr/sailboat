#include <string>
using namespace std;

class Request {
	private:
		string host;
		string uri;		
	public:
		Request(string, string);
		string getHost();
		string getUri();
};

Request::Request (string h, string u) {
	host = h;
	uri = h + u;
}