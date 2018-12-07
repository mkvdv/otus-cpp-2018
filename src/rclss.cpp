#include "exceptions/exceptions.h"
#include "classification/classification.h"

#include <tuple>
#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace {
  std::tuple<std::string, std::string> check_arguments(const int argc, const char *const argv[]) {
      namespace po = boost::program_options;

      const char *HELP_PARAM = "help";
      const char *MODEL_FILENAME_PARAM = "model_filename";
      const char *CLUSTERS_FILENAME_PARAM = "clusters_filename";

      // Declare the supported options.
      po::options_description desc("Allowed options");
      desc.add_options()
          (HELP_PARAM, "produce help message")
          (MODEL_FILENAME_PARAM, po::value<std::string>(), "name of output model file")
          (CLUSTERS_FILENAME_PARAM, po::value<std::string>(), "name of output clusters data file");

      po::positional_options_description p;
      p.add(MODEL_FILENAME_PARAM, 1).add(CLUSTERS_FILENAME_PARAM, 1);

      po::variables_map vm;
      try {
          po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
          po::notify(vm);
      } catch (boost::program_options::error &err) {
          throw std::invalid_argument(err.what());
      }

      if (vm.count(HELP_PARAM)) {
          std::cout << desc << "\n";
          throw otus::exception::terminate_exception("Help information printed");
      }

      if (!vm.count(MODEL_FILENAME_PARAM)) {
          throw std::invalid_argument("Name of output model file was not set");
      } else if (!vm.count(CLUSTERS_FILENAME_PARAM)) {
          throw std::invalid_argument("Name of output clusters data file was not set");
      }

      std::string model_file_name = vm[MODEL_FILENAME_PARAM].as<std::string>();
      std::string clusters_file_name = vm[CLUSTERS_FILENAME_PARAM].as<std::string>();

      return std::make_tuple(std::move(model_file_name), std::move(clusters_file_name));
  }
} // anonymous namespace

/**
 * Read csv from stdin. Process last two parameters from csv string.
 * Train classification model.
 * Save to file.
 */
int main(const int argc, const char *const argv[]) {
    std::string model_filename;
    std::string clusters_filename;

    try {
        std::tie(model_filename, clusters_filename) = check_arguments(argc, argv);
        model_filename += ".XXX";
        clusters_filename += ".XXX";

        otus::load_model_and_classify(model_filename, clusters_filename);
    } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const otus::exception::terminate_exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_SUCCESS;
    } catch (std::exception &ex) {
        std::cerr << "UNHANDLED EXCEPTION: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
