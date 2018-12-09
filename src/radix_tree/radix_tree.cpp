#include "radix_tree.h"

#include <memory>
#include <cassert>
#include <string>
#include <iostream>
#include <algorithm>

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

  RadixTree::Node::Node(std::string &&label, size_t chars_depth, bool is_end)
      : label_(std::move(label)), chars_depth_(chars_depth), is_end_(is_end) {}

  /**
   * Retrun last node in trie branch, that matches string s (as most as it can)
   */
  RadixTree::Node &RadixTree::find_last(const std::string &s) {
      Node *cur = &root_;

      size_t i = 0;
      while (i != s.size()) {
          auto next_node_it = cur->childs_.find(s[i]);
          if (next_node_it != cur->childs_.end()) {
              cur = &(next_node_it->second); // re-set

              // check that this node's label and next string portion are equal before going next
              if (std::string_view{s.c_str() + i, std::min(cur->label_.size(), s.size() - i)} !=
                  std::string_view{cur->label_}) {
                  break; // no need to go to next
              }
              i += std::min(cur->label_.size(), s.size() - i);
          } else {
              break;
          }
      }

      return *cur;
  }

  void RadixTree::add(const std::string &s) {
      Node &last = find_last(s); // find last existed node, that we way on the string

      // check that this string exist in tree in some way
      if (last.label_ == s) {
          if (last.is_end_) {
              return; // this string is already exists
          } else {
              last.is_end_ = true; // exist prefix, but not the whole string
              return;
          }
      }

      /* s_suffix -- the remaining part of string (not found as child of last node)
       * next code will push it inside tree
       *
       * 2 cases
       *  - 1 -- last.label_ is full prefix of suffix of s (so, len last.label < len of s
       *  - 2 -- last.label_ is NOT full prefix,of s, so we need split this node and create 2
       *    links to 2 nodes suffix of s and suffix of last.label_, and last.label_ will contain
       *    max common prefix of both s and last.label_
       */

      std::string s_suffix = s.substr(last.chars_depth_);
      size_t common_prefix_len = equal_prefix_size(s_suffix, last.label_);

      if (common_prefix_len == last.label_.size()) {
          // case 1 - add new link
          last.label_ = last.label_.erase(common_prefix_len);
          std::string suffix_of_suffix = s_suffix.substr(common_prefix_len);
          char key = suffix_of_suffix[0];
          last.childs_[key] = Node(std::move(suffix_of_suffix),
                                   last.chars_depth_ + last.label_.size(),
                                   true);
      } else if (common_prefix_len == s.size()) {
          // case 1.2 -- add new link, but shrink last
          std::string last_suffix = last.label_.substr(common_prefix_len);
          last.label_ = last.label_.erase(common_prefix_len);
          char key = last_suffix[0];
          last.childs_[key] = Node(std::move(last_suffix),
                                   last.chars_depth_ + last.label_.size(),
                                   true);
      } else {
          // case 2 - split last node, add 2 childs
          std::string last_suffix = last.label_.substr(common_prefix_len);

          // insert last node's suffix node if there is something to insert
          if (!last_suffix.empty()) {
              last.label_ = last.label_.erase(common_prefix_len);
              char key = last_suffix[0];

              Node last_suffix_node = Node(std::move(last_suffix),
                                           last.chars_depth_ + last.label_.size(),
                                           last.is_end_);
              last_suffix_node.childs_ = std::move(last.childs_); // move tail of last node whith it's
              // connection to next node
              last.childs_ = std::map<char, Node>{};

              last.childs_[key] = std::move(last_suffix_node);
          }
          if (last.is_end_) {
              last.is_end_ = false;
          }

          // and new node
          std::string suffix_of_suffix = s_suffix.substr(common_prefix_len);
          char key = suffix_of_suffix[0];
          last.childs_[key] = Node(std::move(suffix_of_suffix),
                                   last.chars_depth_ + last.label_.size(),
                                   true);
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
