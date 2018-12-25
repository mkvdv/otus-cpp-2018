#include <boost/log/trivial.hpp>

#include "session.h"

namespace otus::server {
  ClientSession::ClientSession(ba::io_service &service, async::handle_t context)
      : service_(service), sock_(service_), context_(context) {
      BOOST_LOG_TRIVIAL(info) << this << " session CTOR called";
  }

  ClientSession::~ClientSession() {
      BOOST_LOG_TRIVIAL(info) << this << " session DTOR called";
  }

  boost::asio::ip::tcp::socket &ClientSession::sock() {
      return sock_;
  }

  void ClientSession::start() {
      BOOST_LOG_TRIVIAL(info) << this << " session start() called";
      stopped_ = false;
      do_async_read();
  }

  void ClientSession::stop() {
      if (stopped_) {
          return;
      }
      stopped_ = true;
      sock_.close();
      BOOST_LOG_TRIVIAL(info) << this << " session stopped, sock closed !";
  }

  void ClientSession::on_read(const ClientSession::error_code &err, size_t bytes) {
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

  void ClientSession::do_async_read() {
      if (stopped_) {
          return;
      }
      using namespace std::placeholders;

      const char delimiter = '\n';
      ba::async_read_until(sock_,
                           read_buffer_,
                           delimiter,
                           std::bind(&ClientSession::on_read, shared_from_this(), _1, _2));
      BOOST_LOG_TRIVIAL(info) << this << " session async_read_until now";
  }
} // namespace otus 
