#include "logger.h"

#include <fstream>

namespace otus {
  namespace {
    std::string concat_results(const std::vector<std::string> &text_results) {
        std::string ss;
        for (auto it = text_results.begin(); it != text_results.end(); ++it) {
            if (it != text_results.begin()) {
                ss.append(", ");
            }
            ss.append(*it);
        }
        return ss;
    }
  } // anonymous namespace

  void FileLogger::write_to_file(const FileLogger::time_point_t &time_point,
                                 const std::vector<std::string> &text_results) {
      std::string path = std::to_string(time_point.time_since_epoch().count()) + ".log";

      std::ofstream fs(path);
      std::string concatted = concat_results(text_results);
      fs << concatted << '\n';
  }

  /*-------------------------------------------------------------------------*/
  void EmptyFileLogger::write_to_file(const IFileLogger::time_point_t &,
                                      const std::vector<std::string> &) {
      /**
       * Do nothing.
       */
  }

  /*-------------------------------------------------------------------------*/
  BulkLogger::BulkLogger(std::ostream &ostream)
      : ostream_(ostream) {}

  void BulkLogger::log_output(const std::vector<std::string> &text_results) {
      ostream_ << "bulk: " << concat_results(text_results) << '\n';
  }
} // namespace otus
