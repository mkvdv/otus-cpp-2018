#include "server.h"

#include <boost/log/trivial.hpp>

namespace otus {
  /**
   * ClientSession impl.
   */
  server::ClientSession::ClientSession(ba::io_service &service,
									   std::shared_ptr<ThreadsafeQueue<ParseCommandJob>> request_queue,
									   std::shared_ptr<TwoTablesStorage> storage)
	  : service_(service), sock_(service_), request_queue_(std::move(request_queue)),
		storage_(std::move(storage)) {}

  server::ClientSession::~ClientSession() {
	  BOOST_LOG_TRIVIAL(info) << this << " Session DTOR called";
  }

  boost::asio::ip::tcp::socket &server::ClientSession::sock() {
	  return sock_;
  }

  void server::ClientSession::start() {
	  stopped_ = false;
	  do_async_read();
  }

  void server::ClientSession::stop() {
	  if (stopped_) {
		  return;
	  }
	  stopped_ = true;
	  sock_.close();
	  BOOST_LOG_TRIVIAL(info) << this << " Stopped, sock closed !";
  }

  void server::ClientSession::on_read(const server::ClientSession::error_code &err, size_t bytes) {
	  if (err) {
		  stop();
	  }
	  if (stopped_) {
		  return;
	  }

	  if (bytes) {
		  // read protocol command
		  std::istream response_stream(&read_buffer_);
		  std::string command;
		  getline(response_stream, command);

		  ParseCommandJob job(std::move(command), shared_from_this());
		  request_queue_->push(std::move(job));
	  }
  }

  void server::ClientSession::do_async_read() {
	  if (stopped_) {
		  return;
	  }
	  using namespace std::placeholders;

	  const char delimiter = '\n';
	  // BOOST_LOG_TRIVIAL(info) << this << " Will async_read_until now";
	  ba::async_read_until(sock_,
						   read_buffer_,
						   delimiter,
						   std::bind(&ClientSession::on_read, shared_from_this(), _1, _2));
  }

  void server::ClientSession::write(const std::string &s) {
	  BOOST_LOG_TRIVIAL(info) << this << " will write " << s;
	  do_async_write(s);
  }

  void server::ClientSession::on_write(const server::ClientSession::error_code &err,
									   [[maybe_unused]] size_t bytes) {
	  if (err) {
		  stop();
	  }
	  if (stopped_) {
		  return;
	  }

	  do_async_read();
  }

  void server::ClientSession::do_async_write(const std::string &s) {
	  if (stopped_) {
		  return;
	  }
	  using namespace std::placeholders;

	  // BOOST_LOG_TRIVIAL(info) << this << " Will async_write_until now";
	  ba::async_write(sock_,
					  ba::buffer(s),
					  std::bind(&ClientSession::on_write, shared_from_this(), _1, _2));
  }

  std::shared_ptr<TwoTablesStorage> server::ClientSession::get_storage() {
	  return storage_;
  }

  /**
   * Server impl.
   */
  server::JoinServer::JoinServer(unsigned short port,
								 std::shared_ptr<ThreadsafeQueue<ParseCommandJob>> request_queue)
	  : requests_queue_(std::move(request_queue)), ep_(ba::ip::tcp::v4(), port),
		acceptor_(service_, ep_) {}

  void server::JoinServer::run() {
	  using namespace std::placeholders;

	  ba::signal_set sig(service_, SIGINT, SIGABRT);
	  sig.async_wait(std::bind(&JoinServer::signal_handler,
							   shared_from_this(),
							   std::placeholders::_1,
							   std::placeholders::_2));

	  auto clientSession = std::make_shared<ClientSession>(service_, requests_queue_, storage_);
	  acceptor_.async_accept(clientSession->sock(),
							 std::bind(&JoinServer::handle_accept, shared_from_this(), clientSession, _1));

	  service_.run();
  }

  void server::JoinServer::handle_accept(const std::shared_ptr<server::ClientSession> &connected_client,
										 const boost::system::error_code &err) {
	  if (err) {
		  throw std::runtime_error("TODO REMOVE THIS EXCEPTION LATER");
	  }

	  using namespace std::placeholders;
	  BOOST_LOG_TRIVIAL(info) << connected_client << " connected";

	  connected_client->start();

#ifdef DEBUG
	  ++DEBUG_CNT;
	  if (DEBUG_CNT >= 3) {
		  BOOST_LOG_TRIVIAL(info) << "No more new connection here";
		  return;
	  }
#endif

	  // async wait new connection
	  auto waited_client_session = std::make_shared<ClientSession>(service_, requests_queue_, storage_);
	  acceptor_.async_accept(waited_client_session->sock(),
							 std::bind(&JoinServer::handle_accept,
									   shared_from_this(),
									   waited_client_session,
									   _1));
  }

// todo dtor NOT CALLED, memory leaks !!!
  server::JoinServer::~JoinServer() {
	  requests_queue_->stop_wait_and_block_pushing();
	  BOOST_LOG_TRIVIAL(info) << "### Server DTOR called";
  }

  void server::JoinServer::signal_handler(const boost::system::error_code &, int) {
	  BOOST_LOG_TRIVIAL(info) << "Signal handler called";
	  service_.stop();
	  requests_queue_->stop_wait_and_block_pushing();
  }

} // namespace otus
