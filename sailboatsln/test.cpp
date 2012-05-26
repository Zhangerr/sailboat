#include <iostream>
#include "boost/asio.hpp"
#include <string>
#include <ctime>
#include <fstream>
#include "request_handler.cpp"
#include <boost/regex.hpp>
#include <stdio.h>

using namespace std;
using boost::asio::ip::tcp;

string make_daytime_string()
{
  time_t now = time(0);
  return ctime(&now);
}

//use XML to set document root

//need to include content-length but it seems it is not sent by all servers
int main()
{
    boost::asio::io_service is;
    tcp::acceptor acceptor(is, tcp::endpoint(tcp::v4(), 8080)); //because port 80, must run as sudo
    
    while(true) {
		tcp::socket sock(is);
		acceptor.accept(sock);
		boost::system::error_code err;
		//works but cant convert to string easily
		//boost::array<char, 512> buffer;
		//size_t l = boost::asio::read(sock, boost::asio::buffer(buffer, 512), boost::asio::transfer_all(), err);
		char buffer [512]; //is this liable to a buffer overflow exploit
	    size_t l = sock.read_some(boost::asio::buffer(buffer), err);
		string str = buffer;
		char hostt[512];
		sscanf(str.c_str(), "%*s %s", hostt);
		cout << hostt << endl;
		//cout << str << endl;
		//Host: x is the host, used for virtual named hosts and such
		//GET / is page to load, regex isnt working so just subbing here with hardcode for now
		//alex perhaps come up with a better regex here, atm have to substr to cut "Host: "
		boost::cmatch hostMatches;
		boost::regex hostRegex(".");
		bool host = boost::regex_search(str.c_str(), hostMatches, hostRegex);
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
