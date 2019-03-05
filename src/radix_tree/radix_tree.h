#pragma once

#include <istream>
#include <map>
#include <string>

namespace otus {
  class RadixTree {
   public:
    RadixTree() = default;

    void add(const std::string &);

    void print_abbreviations(std::ostream &);
    void print(std::ostream &);

   private:
    struct Node {
      Node() = default;
      Node(std::string &&label, bool is_end);

      Node(Node &&) noexcept = default;
      Node &operator=(Node &&) noexcept = default;
      void swap(Node &node) noexcept;

      std::map<char, Node> childs_{};

      std::string label_ = ""; // string on the link this node connected to parent
      bool is_end_ = false;
    };

    void print_node(const std::string &indent, const RadixTree::Node &node, std::ostream &os);
    void print_abbrevation_node(const RadixTree::Node &node, const std::string &acc, std::ostream &os);

    using chars_depth_t = size_t;
    std::pair<Node &, chars_depth_t> find_last(const std::string &str);

   private:
    Node root_;
  }; // class RadixTree

  void read_from_input(RadixTree &, std::istream &);

} // namespace otus
