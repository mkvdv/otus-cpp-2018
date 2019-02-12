#include "radix_tree/radix_tree.h"

#include <iostream>
#include <fstream>
#include <cassert>

int main() {

    otus::RadixTree tree;
    otus::read_from_input(tree, std::cin);

    tree.print_abbreviations(std::cout);
    std::cout << '\n';

    tree.print(std::cout);

    return 0;
}
