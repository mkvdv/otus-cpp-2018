#include "reader.h"

namespace otus {
	Reader::Reader(std::istream &istream)
		: istream_(istream) {}

	void Reader::start_reading_cycle() {
		while (istream_) {
			std::string tmp;
			std::getline(istream_, tmp);
			if (!tmp.empty()) {
				notify_all(tmp);
			}
		}
	}
} // namespace otus
