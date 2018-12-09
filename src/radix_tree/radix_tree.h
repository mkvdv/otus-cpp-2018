#pragma once

#include <istream>
#include <map>
#include <memory>

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
      Node(std::string &&label, size_t chars_depth, bool is_end);

      Node(Node &&) = default;
      Node &operator=(Node &&) = default;

      std::map<char, Node> childs_{};

      std::string label_ = ""; // string on the link this node connected to parent
      size_t chars_depth_ = 0; // how many characters was in chain before this node
      bool is_end_ = false;
    };

    void print_node(const std::string &indent, const RadixTree::Node &node, std::ostream &os);
    void print_abbrevation_node(const RadixTree::Node &node, const std::string &acc, std::ostream &os);
    Node &find_last(const std::string &s);

   private:
    Node root_;
  }; // class RadixTree

  void read_from_input(RadixTree &, std::istream &);

} // namespace otus
