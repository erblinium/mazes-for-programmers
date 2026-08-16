#include "Grid.hpp"
#include "BinaryTree.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Grid grid(4, 4);
    BinaryTree::on(grid);
    std::cout << grid;
    return 0;
}