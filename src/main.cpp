#include "exceptions/exceptions.h"
#include "prefix/prefix_counter.h"
#include "map_reduce.h"
#include "prefix/find_prefix_function.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <boost/log/core/core.hpp>

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <algorithm>

namespace {
  std::tuple<std::string, size_t, size_t, bool> check_arguments(const int argc, const char *const argv[]) {
	  namespace po = boost::program_options;

	  // Declare the supported options.
	  po::options_description desc("Allowed options");
	  desc.add_options()
		  ("help", "produce help message")
		  ("src", po::value<std::string>(), "path to file with data")
		  ("mnum", po::value<size_t>(), "number of threads to work with map function")
		  ("rnum", po::value<size_t>(), "number of threads to work with reduce function")
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

	  if (!vm.count("src")) {
		  throw std::invalid_argument("Source path was not set");
	  } else if (!vm.count("mnum")) {
		  throw std::invalid_argument("Number of threads for map was not set");
	  } else if (!vm.count("rnum")) {
		  throw std::invalid_argument("Number of threads for reduce was not set");
	  }

	  bool logging = false;
	  if (vm.count("logging")) {
		  logging = vm["logging"].as<bool>();
	  }

	  std::string src = vm["src"].as<std::string>();
	  size_t mnum = vm["mnum"].as<size_t>();
	  size_t rnum = vm["rnum"].as<size_t>();

	  return std::make_tuple(std::move(src), mnum, rnum, logging);
  }

  std::vector<std::string> read_file(std::ifstream &&fs) {
	  std::vector<std::string> lines;
	  std::string tmp;
	  while (fs) {
		  fs >> tmp;
		  if (!tmp.empty()) {
			  BOOST_LOG_TRIVIAL(info) << "read from file: " << tmp;
			  lines.push_back(std::move(tmp));
		  }
	  }
	  fs.close(); // todo can fail, check it (exception or other)
	  return lines;
  }
} // anonymous namespace

int main(const int argc, const char *const argv[]) {
	std::string src_path;
	size_t mnum;
	size_t rnum;
	bool logging_enabled;

	try {
		std::tie(src_path, mnum, rnum, logging_enabled) = check_arguments(argc, argv);
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

	std::ifstream fs(src_path);
	if (!fs) {
		BOOST_LOG_TRIVIAL(error) << "Can't open file " << src_path;
		return EXIT_FAILURE;
	}
	std::vector<std::string> input_lines = read_file(std::move(fs));

	/* Types:
	 *  -  map :: std::vector<std::string>&& -> std::vector<otus::PrefixCounter>
	 *  -  reduce :: std::vector<otus::PrefixCounter> -> otus::PrefixCounter
	 *
	 *  or
	 *
	 *  - map     :: vec<T1> -> vec<T2>
	 *  - reduce  :: vec<T2> -> T3
	 *  - extract :: T3 -> Res
	 */
	auto map_reducer = otus::MapReducer<std::string,            // T1
										otus::PrefixCounter,    // T2
										otus::PrefixCounter,    // T3
										otus::PrefixCounter     // T4
	>(mnum, rnum);

	otus::PrefixCounter res = map_reducer.run(std::move(input_lines),
											  otus::mapper,
											  otus::reducer,
											  otus::reducer);
	size_t min_ident_prefix_size = (res.count() > 1 ? res.strlen() : 0) + 1;

	BOOST_LOG_TRIVIAL(info) << "Res prefixCounter: " << res;
	std::cout << "Minimal identifying prefix size: " << min_ident_prefix_size << std::endl;

	return EXIT_SUCCESS;
}
