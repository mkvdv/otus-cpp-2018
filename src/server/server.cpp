#include <boost/log/trivial.hpp>

#include "server.h"

namespace otus::server {
  BulkServer::BulkServer(unsigned short port, unsigned int bulk_size)
      : context_(async::connect(bulk_size)),
        ep_(ba::ip::tcp::v4(), port),
        acceptor_(service_, ep_) {
      BOOST_LOG_TRIVIAL(info) << this << " server CTOR called";
  }

  void BulkServer::run() {
      using namespace std::placeholders;

      ba::signal_set sig(service_, SIGINT, SIGABRT);
      sig.async_wait(std::bind(&BulkServer::signal_handler,
                               shared_from_this(),
                               std::placeholders::_1,
                               std::placeholders::_2));

      auto clientSession = std::make_shared<ClientSession>(service_, context_);
      BOOST_LOG_TRIVIAL(info) << clientSession << " session will async_accept";
      acceptor_.async_accept(clientSession->sock(),
                             std::bind(&BulkServer::handle_accept,
                                       shared_from_this(),
                                       clientSession,
                                       _1));
      service_.run();
  }

  void BulkServer::handle_accept(const std::shared_ptr<ClientSession> &connected_client,
                                 const boost::system::error_code &err) {
      if (err) {
          BOOST_LOG_TRIVIAL(error) << "in handle_accept: " << err.message() << std::endl;
          return;
      }

      using namespace std::placeholders;
      BOOST_LOG_TRIVIAL(info) << connected_client << " connected";

      connected_client->start();

#ifdef DEBUG
      ++DEBUG_CNT;
      if (DEBUG_CNT >= 2) {
          BOOST_LOG_TRIVIAL(info) << "No more new connection here, acceptor will be closed";
          acceptor_.close();
          return;
      }
#endif

      // async wait new connection
      auto waited_client_session = std::make_shared<ClientSession>(service_, context_);
      BOOST_LOG_TRIVIAL(info) << waited_client_session << " session will async_accept";

      acceptor_.async_accept(waited_client_session->sock(),
                             std::bind(&BulkServer::handle_accept,
                                       shared_from_this(),
                                       waited_client_session,
                                       _1));
  }

  BulkServer::~BulkServer() {
      BOOST_LOG_TRIVIAL(info) << "### Server DTOR called";

      if (context_) {
          async::disconnect(context_);
          context_ = nullptr;
      }
  }

  void BulkServer::signal_handler(const boost::system::error_code &, int) {
      BOOST_LOG_TRIVIAL(info) << "Signal handler called";

      acceptor_.close();
      if (context_) {
          async::disconnect(context_);
          context_ = nullptr;
      }
  }
} // namespace otus::serer
