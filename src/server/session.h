#pragma once

#include "../async/async.h"

#include <boost/asio.hpp>
#include <memory>

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
  };
} // namespace otus::server
