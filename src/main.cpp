#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/log/core/core.hpp>

#include <iostream>
#include <stdexcept>

#include "server.h"
#include "exceptions/exception.h"

namespace {
  std::tuple<unsigned short, unsigned int, bool> check_arguments(const int argc,
																 const char *const argv[]) {
	  namespace po = boost::program_options;

	  // Declare the supported options.
	  po::options_description desc("Allowed options");
	  desc.add_options()("help", "produce help message")
		  ("port", po::value<unsigned short>(), "set server port")
		  ("bulk_size", po::value<unsigned int>(), "set size of bulk")
		  ("logging", po::value<bool>(), "is logging enabled");

	  po::variables_map vm;
	  po::store(po::parse_command_line(argc, argv, desc), vm);
	  po::notify(vm);

	  if (vm.count("help")) {
		  std::cout << desc << "\n";
		  throw std::runtime_error("Help information printed");
	  }

	  if (!vm.count("port")) {
		  throw std::invalid_argument("Port was not set");
	  } else if (!vm.count("bulk_size")) {
		  throw std::invalid_argument("Size of bulk was not set.");
	  }

	  unsigned short port = vm["port"].as<unsigned short>();
	  unsigned int bulk_size = vm["bulk_size"].as<unsigned int>();
	  bool logging = false;
	  if (vm.count("logging")) {
		  logging = vm["logging"].as<bool>();
	  }

	  return std::make_tuple(port, bulk_size, logging);
  }
} // anonymous namespace

int main(const int argc, const char *const argv[]) {
	unsigned short port;
	unsigned int bulk_size;
	bool logging_enabled;

	try {
		std::tie(port, bulk_size, logging_enabled) = check_arguments(argc, argv);
	}
	catch (const std::invalid_argument &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const otus::exception::terminate_exception &) {
		return EXIT_FAILURE;
	}

	boost::log::core::get()->set_logging_enabled(logging_enabled);

	auto server = std::make_shared<otus::server::BulkServer>(port, bulk_size);
	server->run();

	BOOST_LOG_TRIVIAL(info) << "Returning from main now, "
							<< server.use_count() << " pointers to server alive";

	return 0;
}
