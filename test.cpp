#include <iostream>
#include "boost/asio.hpp"
#include <string>
#include <ctime>
#include <fstream>
#include "request_handler.hpp"
#include <boost/regex.hpp>


using namespace std;
using boost::asio::ip::tcp;

string make_daytime_string()
{
  time_t now = time(0);
  return ctime(&now);
}

string getindex() {
	string result;
	string line;
	ifstream ind("www/index.html");
	if(ind.is_open()) {
		while(ind.good()) {
			getline(ind,line);
			result += line + "\r\n";
		}
		return result;
	} else {
		return "could not find index";
	}
}
//need to include content-length
int main()
{
   /* using namespace boost::lambda;
    typedef std::istream_iterator<int> in;
	cout << "enter numbers" << endl;
    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );*/
    boost::asio::io_service is;
    tcp::acceptor acceptor(is, tcp::endpoint(tcp::v4(), 8080)); //because port 80, must run as sudo
    
    while(true) {
		tcp::socket sock(is);
		acceptor.accept(sock);
		boost::system::error_code err;
		//works but cant convert to string easily
		//boost::array<char, 512> buffer;
		//size_t l = boost::asio::read(sock, boost::asio::buffer(buffer, 512), boost::asio::transfer_all(), err);
		char buffer [512];
	    size_t l = sock.read_some(boost::asio::buffer(buffer), err);
		string str = buffer;
		cout << str << endl;
		//Host: x is the host, used for virtual named hosts and such
		//GET / is page to load, regex isnt working so just subbing here with hardcode for now
		//alex perhaps come up with a better regex here, atm have to substr to cut "Host: "
		boost::cmatch hostMatches;
		boost::regex hostRegex(".");
		bool host = boost::regex_match(str.c_str(), hostMatches, hostRegex);
		cout << host << endl;
		if (host)
		{
			
		}
		//static const boost::regex uriRegex("(?=/).*(?= HTTP)");
		//bool uri = boost::regex_match(str, Matches, uriRegex);
		//out << uri << endl;
		
		//Request req (host, uri);
		Request req("a", "b");
		Response res = getResponse(req);
		boost::asio::write(sock, boost::asio::buffer("\
HTTP/1.1 200 OK\r\n\
Date: " + make_daytime_string() + " GMT\r\n\
Server: sailboat\r\n\
Connection: close\r\n\
Content-Type: text/html\r\n\r\n" 
+ res.getContent()), boost::asio::transfer_all(), err);
		
	}
}
