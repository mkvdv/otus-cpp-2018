// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
    This is an example illustrating the use of the kkmeans object
    and spectral_cluster() routine from the dlib C++ Library.

    The kkmeans object is an implementation of a kernelized k-means clustering
    algorithm.  It is implemented by using the kcentroid object to represent
    each center found by the usual k-means clustering algorithm.

    So this object allows you to perform non-linear clustering in the same way
    a svm classifier finds non-linear decision surfaces.

    This example will make points from 3 classes and perform kernelized k-means
    clustering on those points.  It will also do the same thing using spectral
    clustering.

    The classes are as follows:
        - points very close to the origin
        - points on the circle of radius 10 around the origin
        - points that are on a circle of radius 4 but not around the origin at all
*/
#include "exceptions/exceptions.h"
#include "classification/classificaction.h"

#include <tuple>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace {
  std::tuple<size_t> check_arguments(const int argc, const char *const argv[]) {
	  namespace po = boost::program_options;

	  // Declare the supported options.
	  po::options_description desc("Allowed options");
	  desc.add_options()
		  ("help", "produce help message")
		  ("k", po::value<size_t>(), "number of clusters for k-means algorithm");

	  po::variables_map vm;
	  try {
		  po::store(po::parse_command_line(argc, argv, desc), vm);
		  po::notify(vm);
	  } catch (boost::program_options::error &err) {
		  throw std::invalid_argument(err.what());
	  }

	  if (vm.count("help")) {
		  std::cout << desc << "\n";
		  throw otus::exception::terminate_exception("Help information printed");
	  }

	  if (!vm.count("k")) {
		  throw std::invalid_argument("Number of clusters was not set");
	  }

	  size_t k = vm["k"].as<size_t>();

	  return std::make_tuple(k);
  }
} // anonymous namespace

int main(const int argc, const char *const argv[]) {
	size_t k;

	try {
		std::tie(k) = check_arguments(argc, argv);
	}
	catch (const std::invalid_argument &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const otus::exception::terminate_exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_SUCCESS;
	} catch (std::exception &ex) {
		std::cerr << "UNHANDLED EXCEPTION: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	otus::classify(k);

	return EXIT_SUCCESS;
}
