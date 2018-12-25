#include <boost/log/trivial.hpp>

#include "server.h"
#include "session.h"

namespace otus::server {
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
                             std::bind(&JoinServer::handle_accept,
                                       shared_from_this(),
                                       clientSession,
                                       _1));

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

  server::JoinServer::~JoinServer() {
      requests_queue_->stop_wait_and_block_pushing();
      BOOST_LOG_TRIVIAL(info) << "### Server DTOR called";
  }

  void server::JoinServer::signal_handler(const boost::system::error_code &, int) {
      BOOST_LOG_TRIVIAL(info) << "Signal handler called";
      acceptor_.close();
      requests_queue_->stop_wait_and_block_pushing();
  }

} // namespace otus::server
