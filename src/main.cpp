#include "Grid.hpp"
#include "BinaryTree.hpp"
#include "Sidewinder.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Grid binaryTreeGrid(5, 5);
    BinaryTree::on(binaryTreeGrid);
    std::cout << "Binary Tree:\n";
    std::cout << binaryTreeGrid;
    std::cout << "Sidewinder:\n";
    Grid sidewinderGrid(5, 5);
    Sidewinder::on(sidewinderGrid);
    std::cout << sidewinderGrid;
    return 0;
}