#pragma once

#include <boost/asio.hpp>

#include "../active/threadsafe_queue.h"
#include "../jobs/request_job.h"
#include "../model/two_tables_storage.h"

namespace otus::server {
  namespace ba = boost::asio;

  class JoinServer : public std::enable_shared_from_this<JoinServer> {
   public:
    explicit JoinServer(unsigned short port,
                        std::shared_ptr<ThreadsafeQueue<ParseCommandJob>> request_queue);
    ~JoinServer();

    void run();
    void handle_accept(const std::shared_ptr<ClientSession> &connected_client,
                       const boost::system::error_code &err);

   private:
    void signal_handler(const boost::system::error_code &, int);

   private:
    ba::io_service service_{};
    std::shared_ptr<ThreadsafeQueue<ParseCommandJob>>
        requests_queue_ = std::make_shared<ThreadsafeQueue<ParseCommandJob>>();
    std::shared_ptr<TwoTablesStorage> storage_ = std::make_shared<TwoTablesStorage>();

    ba::ip::tcp::endpoint ep_;
    ba::ip::tcp::acceptor acceptor_;
  };

} // namespace otus::server
