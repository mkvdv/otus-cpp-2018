#pragma once

#include <string>

#include "../model/two_tables_storage.h"
#include "../active/threadsafe_queue.h"
#include "../utils/utils.h"

// from "../server.h"
namespace otus {
  namespace server {
	class ClientSession; // forward decl
  }

  /**
   * Job for worker thread -- parse command and do job, and then send result to asio async function.
   */
  class ParseCommandJob {
   public:
	ParseCommandJob(std::string &&command, std::shared_ptr<server::ClientSession> session);
	void run();

   private:
	void process_insert_command(std::vector<std::string> &tokens);
	void process_truncate_command(std::vector<std::string> &tokens);
	void send_table(const table::SimpleTable &);

   private:
	std::string text_;
	std::shared_ptr<server::ClientSession> session_;

	const char *OK_mesg_ = "OK\n";
	const char *INSERT_COMMAND_NAME = "INSERT";
	const char *TRUNCATE_COMMAND_NAME = "TRUNCATE";
	const char *INTERSECT_COMMAND_NAME = "INTERSECTION";
	const char *SYMDIFF_COMMAND_NAME = "SYMMETRIC_DIFFERENCE";
  };
} // namespace otus
