#include <iostream>
#include "boost/asio.hpp"
#include <string>
#include <ctime>
#include <fstream>
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
	ifstream ind("index.html");
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
    tcp::acceptor acceptor(is, tcp::endpoint(tcp::v4(), 80)); //because port 80, must run as sudo
    
    while(true) {
		tcp::socket sock(is);
		acceptor.accept(sock);
		boost::system::error_code err;
		//cout << make_daytime_string() << endl;
		boost::asio::write(sock, boost::asio::buffer("\
HTTP/1.1 200 OK\r\n\
Date: " + make_daytime_string() + " GMT\r\n\
Server: sailboat\r\n\
Connection: close\r\n\
Content-Type: text/html\r\n\r\n" 
+ getindex()), boost::asio::transfer_all(), err);
		
	}
}
