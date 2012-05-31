#include <iostream>
#include <stdio.h>
#include "boost/asio.hpp"
#include <string>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "xml/rapidxml.hpp"
#include <fstream>
#include <ctime>
#include <sstream>
#ifndef __REQUEST_HPP_INCLUDED
#define __REQUEST_HPP_INCLUDED
#include "request.hpp"
#endif
#include "response.hpp"
#ifndef __UTIL_HPP_INCLUDED
#define __UTIL_HPP_INCLUDED
#include "Util.hpp"
#endif
using namespace std;
using boost::asio::ip::tcp;

//need to include content-length but it seems it is not sent by all servers
int main()
{
   
    if(!Util::parseXml()) { //fixes segmentation fault
    	cout << "Please make sure config.xml is located in the working directory and that it is readable." << endl;
    	return -1;
    }
    boost::asio::io_service is;
    tcp::acceptor acceptor(is, tcp::endpoint(tcp::v4(), Util::port)); //if port 80, must run as sudo
    while(true) {
		tcp::socket sock(is);
		acceptor.accept(sock);
		
		boost::system::error_code err;
		//works but cant convert to string easily
		//boost::array<char, 512> buffer;
		//size_t l = boost::asio::read(sock, boost::asio::buffer(buffer, 512), boost::asio::transfer_all(), err);
		char buffer [1024]; //is this liable to a buffer overflow exploit
	    size_t l = sock.read_some(boost::asio::buffer(buffer), err);
		string str = buffer;
		Request request(str);
		cout << Util::make_daytime_string() << "\t" << sock.remote_endpoint().address().to_string() << "\t" << request.getVerb() << " " << request.getUri() << endl;
		//static const boost::regex uriRegex("(?=/).*(?= HTTP)");	
		//Request req (host, uri);
		Response res = getResponse(request);
		boost::asio::write(sock, boost::asio::buffer(res.getPage()), boost::asio::transfer_all(), err);		
	}
}
