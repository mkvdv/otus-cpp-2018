#pragma once

#include <dlib/clustering.h>

namespace otus {
  // Here we declare that our samples will be 2 dimensional column vectors.
  // (Note that if you don't know the dimensionality of your vectors at compile time
  // you can change the 2 to a 0 and then set the size at runtime)
  const size_t COLUMNS = 8;
  using sample_type = dlib::matrix<double, COLUMNS - 1, 1>;

  using row_t = std::vector<std::string>;
  using samples_t = std::vector<sample_type>;
  using rows_t = std::vector<row_t>;

  // Now we are making a typedef for the kind of kernel we want to use.  I picked the
  // radial basis kernel because it only has one parameter and generally gives good
  // results without much fiddling.
  using kernel_type = dlib::radial_basis_kernel<sample_type>;

  using column_ix_t = size_t;
  using row_ix_t = size_t;
} // namespace otus
