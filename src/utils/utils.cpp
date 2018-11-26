#include "utils.h"

namespace otus {
  std::vector<std::string> split(const std::string &text, char delim) {
	  std::vector<std::string> res;

	  size_t begin = 0;

	  while (begin < text.length()) {
		  size_t end = text.find_first_of(delim, begin);
		  if (end != std::string::npos) {
			  res.push_back(text.substr(begin, end - begin));
			  begin = end + 1;
		  } else {
			  res.push_back(text.substr(begin, text.length() - begin));
			  break;
		  }
	  }

	  return res;
  }

} // namespace otus
