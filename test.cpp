#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;
int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;
	cout << "enter numbers" << endl;
    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
}
