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
#include "request.hpp"
#include "response.hpp"
#include "Util.hpp"
#include <vector>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/regex.hpp>
#define foreach         BOOST_FOREACH
using namespace std;
using boost::asio::ip::tcp;

/**need to include content-length but it seems it is not sent by all servers
  *possible todos -- gets posts cookies
  *lua design -- go php style & just have globals with post/get params? or pass those as arguments (varargs or table)
  *perhaps generate doxygen documentation?, use asserts
  */
int main()
{
	
	if(!Util::loadMime()) {
		cerr << "Please make sure you have mimes.type in the working directory" << endl;
		return -1;
	}
    if(!Util::parseXml()) { //missing xml nodes usually cause segmentation faults because the pointer to the node = 0 and we attempt to dereference that
    	cerr << "Please make sure config.xml is located in the working directory and that it is readable." << endl;
    	return -1;
    }
	if(Util::exists("hosts.xml")) { //vhosts should only be parsed if they exist, otherwise the server should be able to function still
		Util::parseVH();
	} else {
		cout << "Could not find vhosts file" << endl;
	}
  /* if(!Util::parseVH()) {
    	cerr << "Could not find vhosts file! make sure hosts.xml is in the working directory" << endl;
    //	return -1;    	
    }*/
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
		string str(buffer,l);		
		vector<string> temp;
		std::list<string> f;
		vector< string > result;
		boost::algorithm::split_regex( result, str, boost::regex( "\r\n\r\n" ) ) ;
		//copy( result.begin(), result.end(), ostream_iterator<string>( cout, "\n" ) ) ;
	//	foreach(string t, result) {
		//	cout << "hm:" <<  t << endl;  
		//} 
		cout << result[1].length() << endl;
	//	cout << boost::contains(str,"\r\n") << endl;
		//boost::split(temp,str, //need split by \r\n http://stackoverflow.com/questions/7436968/boostsplit-using-whole-string-as-delimiter
		Request request(str);
		Util::log(Util::make_daytime_string() + "\t" + sock.remote_endpoint().address().to_string() + "\t" + request.getVerb() + " " + request.getUri());
		//static const boost::regex uriRegex("(?=/).*(?= HTTP)");	
		//Request req (host, uri);
		Response res = getResponse(request,request.GetParams);
		
		boost::asio::write(sock, boost::asio::buffer(res.getPage()), boost::asio::transfer_all(), err);		
	}
}
