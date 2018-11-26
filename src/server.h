#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <set>

#include "active/threadsafe_queue.h"
#include "jobs/request_job.h"
#include "model/two_tables_storage.h"

// #define DEBUG

namespace otus::server {
  namespace ba = boost::asio;

  /**
   * ClientSession
   */
  class ClientSession : public std::enable_shared_from_this<ClientSession> {
   public:
	ClientSession(ba::io_service &service,
				  std::shared_ptr<ThreadsafeQueue<ParseCommandJob>> request_queue,
				  std::shared_ptr<TwoTablesStorage> storage);
	~ClientSession();

	ba::ip::tcp::socket &sock();
	void start();
	void stop();
	void write(const std::string &);
	std::shared_ptr<TwoTablesStorage> get_storage();

   private:
	using error_code = boost::system::error_code;

	void on_read(const error_code &err, size_t bytes);
	void do_async_read();

	void on_write(const error_code &err, size_t bytes);
	void do_async_write(const std::string &);

   private:
	ba::io_service &service_;
	ba::ip::tcp::socket sock_;
	bool stopped_ = true;
	ba::streambuf read_buffer_;

	std::shared_ptr<ThreadsafeQueue<ParseCommandJob>> request_queue_;
	std::shared_ptr<TwoTablesStorage> storage_;
  }; // class ClientSession

  /**
   * JoinServer
   */
  class JoinServer : public std::enable_shared_from_this<JoinServer> {
   public:
	explicit JoinServer(unsigned short port,
						std::shared_ptr<ThreadsafeQueue<ParseCommandJob>> request_queue);
	~JoinServer();

	void run();
	void handle_accept(const std::shared_ptr<ClientSession> &connected_client,
					   const boost::system::error_code &err);

   private:
	void signal_handler(const boost::system::error_code &, int);

   private:
	ba::io_service service_{};
	std::shared_ptr<ThreadsafeQueue<ParseCommandJob>>
		requests_queue_ = std::make_shared<ThreadsafeQueue<ParseCommandJob>>();
	std::shared_ptr<TwoTablesStorage> storage_ = std::make_shared<TwoTablesStorage>();

	ba::ip::tcp::endpoint ep_;
	ba::ip::tcp::acceptor acceptor_;

#ifdef DEBUG
	size_t DEBUG_CNT = 0;
#endif

  }; // class JoinServer

} // namespace otus
