#include "exceptions/exceptions.h"
#include "finder.h"

#include <tuple>
#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

/**
 * This function throws otus::exception::terminate_exception
 * and boost::program_options::error
 */
static void check_arguments(const int argc, const char *const argv[],
                            std::vector<otus::fs::path> &dirs, std::vector<otus::fs::path> &ex_dirs,
                            bool &recursive, size_t &min_size, std::vector<std::string> &masks,
                            size_t &bsize, std::string &hash_name) {
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("dirs", po::value<std::vector<otus::fs::path>>(&dirs)->multitoken()->required(), "directories")
        ("ex-dirs", po::value<std::vector<otus::fs::path>>(&ex_dirs)->multitoken(),
         "excluded directories")
        ("rec", po::value<bool>(&recursive)->required(), "scanning level: true is recursive search, "
                                                         "false is non-recursive search without nested ones")
        ("minsize", po::value<size_t>(&min_size)->default_value(1), "minimum size of files to be "
                                                                    "checked ")
        ("masks", po::value<std::vector<std::string>>(&masks)->multitoken(), "masks for files")
        ("bsize", po::value<size_t>(&bsize)->default_value(BUFSIZ), "block_size for reading files")
        ("hash", po::value<std::string>(&hash_name), "name of hashing algorithm");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        throw otus::exception::terminate_exception("Help information printed");
    }

    po::notify(vm); // after help checking

    if (bsize == 0 || min_size == 0) {
        throw boost::program_options::error("Invalid integer argument value: must be > 0");
    }
}

int main(const int argc, const char *const argv[]) {
    std::vector<otus::fs::path> dirs{};
    std::vector<otus::fs::path> ex_dirs{};
    std::vector<std::string> masks{};
    std::string hash_name = "md5";
    size_t bsize;
    bool recursive = false;
    size_t min_size = 1;

    try {
        check_arguments(argc, argv,
                        dirs, ex_dirs, recursive, min_size,
                        masks, bsize, hash_name);
    } catch (const boost::program_options::error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const otus::exception::terminate_exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_SUCCESS;
    } catch (std::exception &ex) {
        std::cerr << "UNHANDLED EXCEPTION: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    namespace fs = boost::filesystem;
    std::vector<std::vector<fs::path>> res;
    try {
        res = otus::find_duplicates(std::move(dirs),
                                    std::move(ex_dirs),
                                    recursive,
                                    min_size,
                                    std::move(masks),
                                    bsize,
                                    hash_name);
    } catch (boost::filesystem::filesystem_error &e) {
        std::cerr << "Boost filesystem_error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (std::runtime_error &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // print groups of duplicates
    for (auto vec_it = res.begin(); vec_it != res.end(); ++vec_it) {
        if (vec_it != res.begin()) {
            std::cout << '\n';
        }
        for (fs::path &pth: *vec_it) {
            std::cout << pth.generic_string() << '\n';
        }
    }

    return EXIT_SUCCESS;
}
