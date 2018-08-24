#pragma once

#include <cstddef>

namespace utils {
  /**
   * Cause of:
   * using node_t = otus::fixed_size_list<std::_Rb_tree_node<
   *     std::pair<const long unsigned int, long unsigned int> >, 10>::Node;
   * std::cout << (std::is_standard_layout_v<node_t> ? "YES" : "NO ") << std::endl; // print "NO"
   *
   * // compile error
   * // std::cout << offsetof(node_t, value_) << std::endl; // non-standard-layout type
   *
   * Code from https://gist.github.com/graphitemaster/494f21190bb2c63c5516
   *
   * Usage:
   * struct vec2 { float x, y; };
   * struct vec3 : vec2 { float z; };
   * struct vec4 : vec3 { float w; };
   *
   * y_offset = offset_of(&vec4::y);
   */
  template<typename T1, typename T2>
  inline size_t offset_of(T1 T2::*member) {
      static T2 obj;
      return (size_t) (&(obj.*member)) - size_t(&obj);
  }
}