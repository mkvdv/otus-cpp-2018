#include "../clusterization/dlib_types.h"
#include "classification.h"

#include <boost/algorithm/string.hpp>

namespace otus {
  namespace details {
    double euclid_distance(const sample_type &lhs, const sample_type &rhs) {
        return std::sqrt(std::pow(rhs(0) - lhs(0), 2) + std::pow(rhs(1) - rhs(1), 2));
    }

    void print(const sample_type &sample, std::ostream &os) {
        for (long i = 0; i != sample.size(); ++i) {
            if (i != 0) {
                os << ';';
            }
            os << sample(i);
        }
        os << '\n';
    }
  } // namespace details

  void print_neighbours(const sample_type &sample, samples_t &neighbours) {
      auto greater_dist = [&sample](const sample_type &lhs, const sample_type &rhs) {
          return details::euclid_distance(sample, lhs)
              < details::euclid_distance(sample, rhs);
      };

      // sort in ascending (by distance o sample) order
      std::sort(neighbours.begin(), neighbours.end(), greater_dist);

      for (sample_type &row: neighbours) {
          details::print(row, std::cout);
      }
  }

  void check_input(const std::vector<std::string> &row) {
      if (row.size() != COLUMNS - 1) {
          throw std::runtime_error(std::string{"Invalid input format: there must be "} +
              std::to_string(COLUMNS - 1) + " columns, but actually there are" +
              std::to_string(row.size()) + " columns");
      }

      for (const auto &s : row) {
          if (s.empty()) {
              throw std::runtime_error("Invalid input format: there must not be any skipped cells");
          }
      }
  }

  void load_model_and_classify(const std::string &model_filename, const std::string &clusters_filename) {
      dlib::kcentroid<kernel_type> kc;
      dlib::kkmeans<kernel_type> test(kc);
      std::map<unsigned long, samples_t> clusters;

      dlib::deserialize(model_filename) >> test; // load real classificator from file
      dlib::deserialize(clusters_filename) >> clusters; // load real classificator from file

      // react on input and make predictions
      std::string tmp;
      while (std::getline(std::cin, tmp)) {
          if (tmp.empty()) {
              continue;
          }

          std::vector<std::string> splitted;
          splitted.reserve(COLUMNS);
          boost::split(splitted, tmp, boost::is_any_of(";"));

          check_input(splitted);

          // convert to sample_type
          sample_type sample;
          for (size_t j = 0; j != COLUMNS - 1; ++j) {
              sample(j) = std::stod(splitted[j]);
          }

          unsigned long cluster_number = test(sample);
          print_neighbours(sample, clusters[cluster_number]);
      }

      std::cout << clusters.size() << std::endl;
  }

} // namespace otus
