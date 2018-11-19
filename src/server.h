#pragma once

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include <iostream>
#include <thread>
#include <set>

#include "async/async.h"

// #define DEBUG

namespace otus::server {
  namespace ba = boost::asio;

  class ClientSession : public std::enable_shared_from_this<ClientSession> {
	public:
	  using error_code = boost::system::error_code;

	  ClientSession(ba::io_service &service, async::handle_t context);
	  ~ClientSession();

	  ba::ip::tcp::socket &sock();
	  void start();
	  void stop();

	private:
	  void on_read(const error_code &err, size_t bytes);
	  void do_async_read();

	private:
	  ba::io_service &service_;
	  ba::ip::tcp::socket sock_;
	  async::handle_t context_;
	  bool stopped_ = true;
	  ba::streambuf read_buffer_;
  }; // class ClientSession

  class BulkServer : public std::enable_shared_from_this<BulkServer> {
	public:
	  BulkServer(unsigned short port, unsigned int bulk_size);
	  ~BulkServer();

	  void run();
	  void handle_accept(const std::shared_ptr<ClientSession> &connected_client,
						 const boost::system::error_code &err);

	private:
	  void signal_handler(const boost::system::error_code &, int);

	private:
	  ba::io_service service_{};
	  void *context_;
	  ba::ip::tcp::endpoint ep_;
	  ba::ip::tcp::acceptor acceptor_;

#ifdef DEBUG
	  size_t DEBUG_CNT = 0;
#endif
  }; // class BulkServer

} // namespace otus
