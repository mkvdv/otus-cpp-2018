#pragma once

#include <string>

#include <dlib/clustering.h>
#include "dlib_types.h"

namespace otus {
  /**
   * Converts line from format:
   * 	86.116781; 55.335492; 2; 4326901.00; 54.00; 7.00; 5; 5\n
   *  to sample_type, also convert last to parameters to one
   *  using this rule: !(first == 1 || first == second)
   *
   */
  std::vector<sample_type> read_from(std::istream &is);

  void create_and_serialize_model(size_t k,
                                  const std::string &model_filename,
                                  const std::string &clusters_filename);

} // namespace otus
