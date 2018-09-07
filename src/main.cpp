/*!
\file
\brief Main file with some smoke tests

This file contain @main(), which call smoke test functions
*/

#include "print_ip.h"
#include "utils.h"

#include <vector>
#include <list>

namespace {
	void test_integral() {
		otus::print_ip(char(-1));
		std::cout << std::endl;

		otus::print_ip(short(0));
		std::cout << std::endl;

		otus::print_ip(int(2130706433));
		std::cout << std::endl;

		otus::print_ip(long(8875824491850138409));
		std::cout << std::endl;

		// otus::print_ip(float(1.0)); // compile error, only integral types
	}

	void test_string() {
		otus::print_ip(std::string{"192.168.2.108"});
		std::cout << std::endl;
	}

	void test_containers() {
		otus::print_ip(std::vector<char>{char(192), char(168), char(0), char(1)});
		std::cout << std::endl;

		otus::print_ip(std::list<char>{char(192), char(255), char(0), char(-1)});
		std::cout << std::endl;

	}

	void test_tuple() {
		otus::print_ip(std::make_tuple(char(0), char(0), char(0), char(-1)));
		std::cout << std::endl;

		otus::print_ip(std::make_tuple());
		std::cout << std::endl;

		otus::print_ip(std::make_tuple(char(1)));
		std::cout << std::endl;

		otus::print_ip(std::make_tuple(char(1), char(2)));
		std::cout << std::endl;

		// compile error, different types
		// otus::print_ip(std::make_tuple(char(1), int(2)));
		// otus::print_ip(std::make_tuple(char(1), char(2), int(3)));
	}
} // anonymous namespace

int main() {
	test_integral();
	test_string();
	test_containers();
	test_tuple();

	return 0;
}
