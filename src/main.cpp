/*!
\file
\brief Main file with some smoke tests

This file contain @main(), which call smoke test static class function.
*/

#include "tester.h"

int main()
{
	otus::TesterFriendClass::simple_test();

	return 0;
}
