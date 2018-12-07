#include "reader.h"

namespace otus {
  Reader::Reader(std::istream &istream)
      : istream_(istream) {}

  void Reader::start_reading_cycle() {
      std::string tmp;
      while (std::getline(istream_, tmp)) {
          notify_all(tmp);
      }
  }
} // namespace otus
