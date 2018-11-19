#include "server.h"

namespace otus {
  /**
   * ClientSession impl.
   */
  otus::server::ClientSession::ClientSession(ba::io_service &service, async::handle_t context)
	  : service_(service), sock_(service_), context_(context) {}

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
		  std::istream response_stream(&read_buffer_);
		  std::string command;
		  getline(response_stream, command);
		  command += '\n';
		  async::receive(context_, command.c_str(), bytes);
	  }

	  do_async_read();
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

  /**
   * Server impl.
   */
  server::BulkServer::BulkServer(unsigned short port, unsigned int bulk_size)
	  : context_(async::connect(bulk_size)),
		ep_(ba::ip::tcp::v4(), port),
		acceptor_(service_, ep_) {}

  void server::BulkServer::run() {
	  using namespace std::placeholders;

	  ba::signal_set sig(service_, SIGINT, SIGABRT);
	  sig.async_wait(std::bind(&BulkServer::signal_handler,
							   shared_from_this(),
							   std::placeholders::_1,
							   std::placeholders::_2));

	  auto clientSession = std::make_shared<ClientSession>(service_, context_);
	  acceptor_.async_accept(clientSession->sock(),
							 std::bind(&BulkServer::handle_accept,
									   shared_from_this(),
									   clientSession,
									   _1));
	  service_.run();
  }

  void server::BulkServer::handle_accept(const std::shared_ptr<server::ClientSession> &connected_client,
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
	  auto waited_client_session = std::make_shared<ClientSession>(service_, context_);
	  acceptor_.async_accept(waited_client_session->sock(),
							 std::bind(&BulkServer::handle_accept,
									   shared_from_this(),
									   waited_client_session,
									   _1));
  }

  // todo dtor NOT CALLED, memory leaks !!!
  server::BulkServer::~BulkServer() {
	  BOOST_LOG_TRIVIAL(info) << "### Server DTOR called";

	  if (context_) {
		  async::disconnect(context_);
		  context_ = nullptr;
	  }
  }

  void server::BulkServer::signal_handler(const boost::system::error_code &, int) {
	  BOOST_LOG_TRIVIAL(info) << "Signal handler called";

	  service_.stop();
	  if (context_) {
		  async::disconnect(context_);
		  context_ = nullptr;
	  }
  }
} // namespace otus
