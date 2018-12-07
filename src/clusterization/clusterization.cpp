#include "clusterization.h"
#include <boost/algorithm/string.hpp>

#include <boost/serialization/map.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace otus {
  namespace details {
    /**
     * Fil empty cells of vector of rows with average by column
     *
     * @param [in] skipped_coordinates  map for skipped cordinates: key is index of skipped column,
     * and value is vector of row ixes of skipped cells
     * @param [out] result vector of rows
     */
    void fill_spaces(std::map<column_ix_t, std::vector<row_ix_t >> skipped_coordinates,
                     samples_t &result) {
        /// count avg for columns with misses
        using pair_t = std::pair<const column_ix_t, std::vector<row_ix_t >>;

        for (const pair_t &p: skipped_coordinates) {
            const column_ix_t &column_ix = p.first;
            const std::vector<row_ix_t> &skipped_row_ixes = p.second;


            // count avg on known lines
            double sum = 0.0;
            size_t count = result.size() - skipped_row_ixes.size();

            // count summ af all cells in columns (skipped columns must contain zero value)
            for (size_t row_ix = 0; row_ix != result.size(); ++row_ix) {
                sum += result[row_ix](column_ix);
            }

            double avg = sum / count;

            // update this avg value for skipped cells
            for (row_ix_t row_ix : skipped_row_ixes) {
                result[row_ix](column_ix) = avg;
            }
        }
    }

  } // namespace details

  std::vector<sample_type> read_from(std::istream &is) {
      std::string tmp;

      std::vector<sample_type> res;
      std::map<column_ix_t, std::vector<row_ix_t >> skipped_coordinates;
      size_t skipped = 0;

      // read data from input to vector
      while (std::getline(is, tmp)) {
          if (tmp.empty()) {
              continue;
          }

          std::vector<std::string> splitted;
          splitted.reserve(COLUMNS);
          boost::split(splitted, tmp, boost::is_any_of(";"));

          // check format
          if (splitted.size() != COLUMNS) {
              throw std::logic_error(
                  std::string{"Wrong input format: "}
                      + std::to_string(splitted.size()) + " != "
                      + std::to_string(COLUMNS));
          }

          /**
           * Assumptions: there are no more misses in last two columns.
           * So we skip them cause there no sense to fill avg insted of them.
           */
          if (splitted[COLUMNS - 1].empty() || splitted[COLUMNS - 2].empty()) {
              skipped += 1;
              continue;
          }

          // write to res
          size_t row_ix = res.size();
          for (size_t i = 0; i != splitted.size(); ++i) {
              if (splitted[i].empty()) {
                  auto it = skipped_coordinates.find(i);
                  if (it == skipped_coordinates.end()) {
                      skipped_coordinates[i] = {std::vector<row_ix_t>{row_ix}};
                  } else {
                      it->second.push_back(row_ix);
                  }
              }
          }

          sample_type sample;
          for (size_t j = 0; j != COLUMNS - 1; ++j) {
              if (!splitted[j].empty()) {
                  sample(j) = std::stod(splitted[j]);
              } else {
                  sample(j) = 0.0;
              }
          }

          const unsigned long long floor = std::stoull(splitted[COLUMNS - 2]);
          const unsigned long long max_floor = std::stoull(splitted[COLUMNS - 1]);
          if (floor == 1 || floor == max_floor) {
              sample(COLUMNS - 2) = 0.0;
          } else {
              sample(COLUMNS - 2) = 1.0;
          }
          res.push_back(std::move(sample));
      }

      details::fill_spaces(skipped_coordinates, res);
      std::cout << "Skipped " << skipped << " rows cause of missing at last two columns\n";

      return res;
  }

  void create_and_serialize_model(size_t k,
                                  const std::string &model_filename,
                                  const std::string &clusters_filename) {
      const double gamma = 0.000001;
      dlib::kcentroid<kernel_type> kc(kernel_type(gamma), 0.01, 16);

      // Now we make an instance of the kkmeans object and tell it to use kcentroid objects
      // that are configured with the parameters from the kc object we defined above.
      dlib::kkmeans<kernel_type> test(kc);

      std::vector<sample_type> samples = read_from(std::cin);
      std::vector<sample_type> initial_centers;

      // tell the kkmeans object we made that we want to run k-means with k clusters
      test.set_number_of_centers(k);

      // You need to pick some initial centers for the k-means algorithm.  So here
      // we will use the dlib::pick_initial_centers() function which tries to find
      // n points that are far apart (basically).
      pick_initial_centers(static_cast<long>(k), initial_centers, samples, test.get_kernel());

      // now run the k-means algorithm on our set of samples.
      test.train(samples, initial_centers);

      // save model
      dlib::serialize(model_filename) << test;

      // save clusters
      std::map<unsigned long, samples_t> clusters;
      for (size_t i = 0; i != samples.size(); ++i) {
          unsigned long cluster = test(samples[i]);
          auto it = clusters.find(cluster);
          if (it != clusters.end()) {
              it->second.push_back(std::move(samples[i]));
          } else {
              samples_t new_samples;
              new_samples.push_back(std::move(samples[i]));
              clusters[cluster] = std::move(new_samples);
          }
      }

      dlib::serialize(clusters_filename) << clusters;
  }

} // namespace otus
