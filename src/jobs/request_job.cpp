#include <utility>

#include "request_job.h"
#include "../server/session.h"

namespace otus {
  ParseCommandJob::ParseCommandJob(std::string &&command,
                                   std::shared_ptr<otus::server::ClientSession> session)
      : text_(std::move(
      command)), session_(std::move(session)) {}

  void ParseCommandJob::run() {
      std::vector<std::string> tokens = split(text_);

      if (tokens.size() == 4 && tokens[0] == INSERT_COMMAND_NAME) {
          // INSERT table id name
          process_insert_command(tokens);

      } else if (tokens.size() == 2 && tokens[0] == TRUNCATE_COMMAND_NAME) {
          // TRUNCATE table_name
          process_truncate_command(tokens);

      } else if (tokens.size() == 1) {
          if (tokens[0] == INTERSECT_COMMAND_NAME) {
              table::SimpleTable result_table = session_->get_storage()->intersection();
              send_table(result_table);
          } else if (tokens[0] == SYMDIFF_COMMAND_NAME) {
              table::SimpleTable result_table = session_->get_storage()->symmetric_difference();
              send_table(result_table);
          } else {
              session_->write("ERR unsupported command\n");
          }
      } else {
          session_->write("ERR wrong command format or unsupported command\n");
      }
  }

  void ParseCommandJob::process_insert_command(std::vector<std::string> &tokens) {
      // INSERT table id name
      const std::string &table_name = tokens[1];
      const std::string &id_text = tokens[2];
      std::string &name_text = tokens[3];

      size_t processed_chars;
      int id = std::stoi(id_text, &processed_chars);
      if (processed_chars != id_text.size()) {
          session_->write("ERR  invalid key\n");
          return;
      }

      bool success;
      try {
          success = session_->get_storage()->insertion(table_name, id, std::move(name_text));
      }
      catch (std::invalid_argument &e) {
          session_->write(std::string{"ERR "} + e.what() + "\n");
          return;
      }

      if (success) {
          session_->write(OK_mesg_);
      } else {
          session_->write(std::string("ERR duplicate ") + id_text + "\n");
      }
  }

  void ParseCommandJob::process_truncate_command(std::vector<std::string> &tokens) {
      // TRUNCATE table_name
      const std::string &table_name = tokens[1];

      try {
          session_->get_storage()->truncate(table_name);
          session_->write(OK_mesg_);
      }
      catch (std::invalid_argument &e) {
          session_->write(std::string{"ERR "} + e.what() + "\n");
      }
  }

  void ParseCommandJob::send_table(const table::SimpleTable &table) {
      std::stringstream ss;
      ss << table;

      session_->write(ss.str() + OK_mesg_);
  }

} // namesapce otus