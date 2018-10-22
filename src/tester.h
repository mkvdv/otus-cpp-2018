/*!
\file
\brief File with tester class for the whole programm.

This class is friend for many other classes, so it can test private field of his friends.
*/

#pragma once

namespace otus {
	class TesterFriendClass {
	public:
		static void simple_test();
	};
} // namespace otus