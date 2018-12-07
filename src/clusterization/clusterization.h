#pragma once

#include <dlib/clustering.h>
#include <dlib/rand.h>

#include <istream>

namespace otus {
  // Here we declare that our samples will be 2 dimensional column vectors.
  // (Note that if you don't know the dimensionality of your vectors at compile time
  // you can change the 2 to a 0 and then set the size at runtime)
  using sample_type = dlib::matrix<double, 2, 1>;

  // Now we are making a typedef for the kind of kernel we want to use.  I picked the
  // radial basis kernel because it only has one parameter and generally gives good
  // results without much fiddling.
  using kernel_type = dlib::radial_basis_kernel<sample_type>;

  std::vector<sample_type> read_from(std::istream &is);
  void clusterize(size_t k);

} // namespace otus
