using namespace std;

class Request {
	private:
		string host;
		string uri;
		string verb; //http verb e.g get, post, head, etc		
	public:
		Response::status_type status;
		Request(string, string);
		Request(string);
		string getHost();
		string getUri();
		string getVerb();
};

