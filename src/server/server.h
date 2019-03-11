#pragma once

#include <boost/asio.hpp>

#include "session.h"

namespace otus::server {
  namespace ba = boost::asio;

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
  }; // class BulkServer

} // namespace otus::server
