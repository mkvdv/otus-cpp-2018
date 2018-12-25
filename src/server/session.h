#pragma once

#include "../active/threadsafe_queue.h"
#include "../jobs/request_job.h"
#include "../model/two_tables_storage.h"

#include <boost/asio.hpp>
#include <memory>

namespace otus::server {
  namespace ba = boost::asio;

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
  };
} // namespace otus::server
