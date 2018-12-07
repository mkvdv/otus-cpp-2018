#include "filters.h"

#include <cassert>
#include <fstream>
#include <algorithm>

int main() {
    using namespace otus;

    try {
        std::vector<ip_t> ip_pool;

        for (std::string line; getline(std::cin, line);) {
            assert(line != "\n" && line != "\t" && !line.empty());

            std::vector<std::string> v = split<std::string>(line, '\t', identity());
            ip_pool.push_back(split<uint8_t>(v.at(0), '.', [](const std::string &s) -> uint8_t {
                return static_cast<uint8_t>((stoul(s)));
            }));
        }

        // reverse lexicographically sort
        std::sort(ip_pool.begin(),
                  ip_pool.end(),
                  std::greater<>{});
        print(ip_pool);

        // filter by first uint8_t and output
        auto ips1 = filter(static_cast<uint8_t>(1), ip_pool);
        print(ips1);

        // filter by first and second uint8_ts and output
        auto ips2 =
            filter(static_cast<const uint8_t>(46), static_cast<const uint8_t>(70), ip_pool);
        print(ips2);

        // filter by any uint8_t and output
        auto ips_any = filter_any(static_cast<const uint8_t>(46), ip_pool);
        print(ips_any);

    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
