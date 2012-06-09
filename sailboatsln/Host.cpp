#include "Host.h"



Host::Host() {

}

string Host::getHost() {
return host;
}
string Host::getRoot() {
return root;
}
string Host::getNotFound() {
return notFoundPage;
}
bool Host::has404() {
	return _has404;
}