#include <boost/log/trivial.hpp>

#include "session.h"

namespace otus::server {
  ClientSession::ClientSession(ba::io_service &service,
                               std::shared_ptr<ThreadsafeQueue<ParseCommandJob>> request_queue,
                               std::shared_ptr<TwoTablesStorage> storage)
      : service_(service), sock_(service_), request_queue_(std::move(request_queue)),
        storage_(std::move(storage)) {}

  ClientSession::~ClientSession() {
      BOOST_LOG_TRIVIAL(info) << this << " Session DTOR called";
  }

  boost::asio::ip::tcp::socket &ClientSession::sock() {
      return sock_;
  }

  void ClientSession::start() {
      stopped_ = false;
      do_async_read();
  }

  void ClientSession::stop() {
      if (stopped_) {
          return;
      }
      stopped_ = true;
      sock_.close();
      BOOST_LOG_TRIVIAL(info) << this << " Stopped, sock closed !";
  }

  void ClientSession::on_read(const ClientSession::error_code &err, size_t bytes) {
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

  void ClientSession::do_async_read() {
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

  void ClientSession::write(const std::string &s) {
      BOOST_LOG_TRIVIAL(info) << this << " will write " << s;
      do_async_write(s);
  }

  void ClientSession::on_write(const ClientSession::error_code &err,
                               [[maybe_unused]] size_t bytes) {
      if (err) {
          stop();
      }
      if (stopped_) {
          return;
      }

      do_async_read();
  }

  void ClientSession::do_async_write(const std::string &s) {
      if (stopped_) {
          return;
      }
      using namespace std::placeholders;

      // BOOST_LOG_TRIVIAL(info) << this << " Will async_write_until now";
      ba::async_write(sock_,
                      ba::buffer(s),
                      std::bind(&ClientSession::on_write, shared_from_this(), _1, _2));
  }

  std::shared_ptr<TwoTablesStorage> ClientSession::get_storage() {
      return storage_;
  }
} // namespace otus::server