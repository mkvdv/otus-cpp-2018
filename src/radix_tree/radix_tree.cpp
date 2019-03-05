#include "radix_tree.h"

#include <algorithm>
#include <string_view>
#include <utility>

namespace otus {
  /**
   * Return size of longest common prefix for both words.
   */
  size_t equal_prefix_size(const std::string &lhs, const std::string &rhs) {
      size_t prefix_size = 0;
      auto l_it = lhs.begin();
      auto r_it = rhs.begin();

      while (l_it != lhs.end() && r_it != rhs.end() && *l_it == *r_it) {
          ++prefix_size;
          ++l_it;
          ++r_it;
      }
      return prefix_size;
  }

  RadixTree::Node::Node(std::string &&label, bool is_end)
      : label_(std::move(label)), is_end_(is_end) {}

  void RadixTree::Node::swap(RadixTree::Node &node) noexcept {
      std::swap(label_, node.label_);
      std::swap(is_end_, node.is_end_);
  }

  /**
   * Retrun last node in trie branch, that matches string s (as most as it can)
   */
  std::pair<RadixTree::Node &, RadixTree::chars_depth_t> RadixTree::find_last(const std::string &str) {
      Node *cur = &root_;

      size_t i = 0;
      size_t chars_before_last_node = 0; // how many characters was in chain before last (returned) node
      while (i != str.size()) {
          auto next_node_it = cur->childs_.find(str[i]);
          if (next_node_it != cur->childs_.end()) {
              chars_before_last_node += cur->label_.size();
              cur = &(next_node_it->second); // re-set

              // check that this node's label and next string portion are equal before going next
              if (std::string_view{str.c_str() + i, std::min(cur->label_.size(), str.size() - i)}
                  != cur->label_) {
                  break; // no need to go to next
              }
              i += std::min(cur->label_.size(), str.size() - i);
          } else {
              break;
          }
      }

      return std::pair<RadixTree::Node &, RadixTree::chars_depth_t>{*cur, chars_before_last_node};
  }

  void RadixTree::add(const std::string &s) {
      auto[last, chars_before_last_node] = find_last(s); // find last existed node, that we way on the string

      // check that this string exist in tree in some way
      if (last.label_ == s) {
          if (last.is_end_) {
              return; // this string is already exists
          } else {
              last.is_end_ = true; // exist prefix, but not the whole string
              return;
          }
      }

      // part of s, that match beggining of last.label_
      std::string s_suffix = s.substr(chars_before_last_node);
      size_t max_common_prefix_len = equal_prefix_size(s_suffix, last.label_); // number of matched
      // chars in s_suffix and label

      /**
       * ## There are 5 Cases:
       * * Not matched at all, so (probably, last is root)
       *    - append s_suffix as new node
       * * Mathed all label, so
       *    - if label < s_suffix, append suffix of suffix
       *    - if label == s_suffix, make label final
       *    otherwise - case can't exist
       * * Matched not all label, so
       *    - if max_common_prefix_len == s_suffix.len() => split this node to 2 sequnced nodes (split
       *    label, make first node final)
       *    - if max_common_prefix_len < s_suffix.len() => split last to 3 nodes -- for max common
       *    prefix, label's suffix and s_suffixes suffix
       */


      if (max_common_prefix_len == 0) { // append s_suffix as new node
          Node new_node(std::move(s_suffix), true);
          last.childs_[new_node.label_[0]] = std::move(new_node);
      } else if (max_common_prefix_len == last.label_.length()) {
          if (last.label_.length() < s_suffix.length()) { // append suffix of suffix
              std::string suffix_of_suffix = s_suffix.substr(last.label_.length());
              char c = suffix_of_suffix[0];
              last.childs_[c] = Node(std::move(suffix_of_suffix), true);
          } else { // last.label_.length() == s_suffix.length()
              last.is_end_ = true;
          }
      } else {
          if (max_common_prefix_len == s_suffix.length()) {
              // split this node to 2 sequnced nodes (split label, make first node final)
              std::string suffix_of_label = last.label_.substr(max_common_prefix_len);
              last.label_ = last.label_.substr(0, max_common_prefix_len);

              Node next_node(std::move(suffix_of_label), last.is_end_);
              next_node.childs_ = std::move(last.childs_);
              last.childs_.clear();
              last.childs_[next_node.label_[0]] = std::move(next_node);
              last.is_end_ = true; // cause suffix ends here

          } else { // max_common_prefix_len < s_suffix.len()
              std::string suffix_of_label = last.label_.substr(max_common_prefix_len);
              std::string suffix_of_s_suffix = s_suffix.substr(max_common_prefix_len);
              last.label_ = last.label_.substr(0, max_common_prefix_len);

              Node s_suffix_node(std::move(suffix_of_s_suffix), true); // cause it is new word
              Node label_suffix_node(std::move(suffix_of_label), last.is_end_); // copy from lasy
              label_suffix_node.childs_ = std::move(last.childs_);

              last.childs_.clear();
              char label_suffix_c = label_suffix_node.label_[0];
              last.childs_[label_suffix_c] = std::move(label_suffix_node);
              char s_suffix_c = s_suffix_node.label_[0];
              last.childs_[s_suffix_c] = std::move(s_suffix_node);

              last.is_end_ = false; // cause node was splitted, it's true gone to label_suffix_node
          }
      }
  }

  void RadixTree::print_node(const std::string &indent, const RadixTree::Node &node, std::ostream &os) {
      std::string additional_indent;

      if (!node.label_.empty()) {
          os << indent << "├─" << node.label_;

          if (node.is_end_) {
              os << "$\n";
          } else {
              os << "\n";
          }

          additional_indent += "| ";
          std::fill_n(std::back_inserter(additional_indent), node.label_.size() - 1, ' ');
      }

      for (auto &child : node.childs_) {
          print_node(indent + additional_indent, child.second, os);
      }
  }

  void RadixTree::print(std::ostream &os) {
      print_node("", root_, os);
  }

  void RadixTree::print_abbrevation_node(const RadixTree::Node &node, const std::string &acc,
                                         std::ostream &os) {
      if (!node.label_.empty() && node.is_end_) {
          os << acc + node.label_ << ' ' << acc + node.label_[0] << '\n';
      }
      for (auto &child : node.childs_) {
          print_abbrevation_node(child.second, acc + node.label_, os);
      }
  }

  void RadixTree::print_abbreviations(std::ostream &os) {
      print_abbrevation_node(root_, root_.label_, os);
  }

  /**
   * Free functions.
   */
  void read_from_input(RadixTree &tree, std::istream &is) {
      std::string tmp;
      while (std::getline(is, tmp)) {
          tree.add(tmp);
      }
  }
} // namespace otus
