#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <boost/log/core/core.hpp>
#include <boost/log/trivial.hpp>

#include "exceptions/exception.h"
#include "model/simple_table.h"
#include "jobs/request_job.h"
#include "active/job_pool.h"
#include "server/server.h"

#include "utils/utils.h"

namespace {
  std::tuple<unsigned short, bool> check_arguments(const int argc, const char *const argv[]) {
      namespace po = boost::program_options;

      // Declare the supported options.
      po::options_description desc("Allowed options");
      desc.add_options()
          ("help", "produce help message")
          ("port", po::value<unsigned short>(), "set server port")
          ("logging", po::value<bool>(), "is logging enabled");

      po::variables_map vm;
      try {
          po::store(po::parse_command_line(argc, argv, desc), vm);
          po::notify(vm);
      }
      catch (boost::program_options::error &err) {
          throw std::invalid_argument(err.what());
      }

      if (vm.count("help")) {
          std::cout << desc << "\n";
          throw otus::exception::terminate_exception("Help information printed");
      }

      if (!vm.count("port")) {
          throw std::invalid_argument("Port was not set");
      }

      unsigned short port = vm["port"].as<unsigned short>();
      bool logging = false;
      if (vm.count("logging")) {
          logging = vm["logging"].as<bool>();
      }

      return std::make_tuple(port, logging);
  }
} // anonymous namespace

int main(const int argc, const char *const argv[]) {
    unsigned short port;
    bool logging_enabled;
    const size_t MAX_JOB_EXEC_THREADS = 1;

    try {
        std::tie(port, logging_enabled) = check_arguments(argc, argv);
    }
    catch (const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const otus::exception::terminate_exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_SUCCESS;
    }

    boost::log::core::get()->set_logging_enabled(logging_enabled);

    std::shared_ptr<otus::ThreadsafeQueue<otus::ParseCommandJob>>
        requests_queue = std::make_shared<otus::ThreadsafeQueue<otus::ParseCommandJob>>();

    std::shared_ptr<otus::server::JoinServer>
        server = std::make_shared<otus::server::JoinServer>(port, requests_queue);

    otus::JobPool<otus::ParseCommandJob, MAX_JOB_EXEC_THREADS> executor(requests_queue);

    server->run();
    BOOST_LOG_TRIVIAL(info) << "Will return EXIT_SUCCESS now";

    return EXIT_SUCCESS;
}
