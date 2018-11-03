#include "async/async.h"
#include <iostream>
#include <thread>

// c-style
int main() {
	const std::size_t bulk = 5;
	namespace as = otus::async;

	auto h1 = as::connect(bulk);
	auto h2 = as::connect(bulk);
	auto h3 = as::connect(bulk + 1);


	as::receive(h1, "1", 1);
	as::receive(h2, "1\n", 2);
	as::receive(h3, "0\n0\n0\n0\n0\n0\n", 12); // <-- other Context, output can be mixed


	as::receive(h1, "\n2\n3\n4\n5\n6\n{\na\n", 15);
	as::receive(h1, "b\nc\nd\n}\n89\n", 11);

	as::disconnect(h1);
	as::disconnect(h2);

	std::cout << std::endl;


	as::receive(h3, "1\n2\n3\n4\n5\n6\n", 12);
	as::disconnect(h3);

	return 0;
}
