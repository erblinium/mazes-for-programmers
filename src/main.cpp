#include "BinaryTree.hpp"
#include "Sidewinder.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Grid binaryTreeGrid(5, 5);
    BinaryTree::on(binaryTreeGrid);
    binaryTreeGrid.savePng(10, "maze-binary-grid.png", 2);
    std::cout << "Binary Tree:\n";
    std::cout << binaryTreeGrid;

    std::cout << "Sidewinder:\n";
    Grid sidewinderGrid(5, 5);
    Sidewinder::on(sidewinderGrid);
    sidewinderGrid.savePng(10, "maze-sidewinder-grid.png", 2);
    std::cout << sidewinderGrid;

    BitfieldGrid binaryTreeBitfieldGrid(5, 5);
    BinaryTree::on(binaryTreeBitfieldGrid);
    //binaryTreeBitfieldGrid.savePng(10, "maze-binary-grid.png", 2);
    std::cout << "Binary Tree BitfieldGrid:\n";
    std::cout << binaryTreeBitfieldGrid;

    NodeGrid binaryTreeNodeGrid(5, 5);
    BinaryTree::on(binaryTreeNodeGrid);
    //binaryTreeNodeGrid.savePng(10, "maze-binary-grid.png", 2);
    std::cout << "Binary Tree NodeGrid:\n";
    std::cout << binaryTreeNodeGrid;

    NodeGrid sidewinderNodeGrid(5, 5);
    Sidewinder::on(sidewinderNodeGrid);
    //sidewinderNodeGrid.savePng(10, "maze-binary-grid.png", 2);
    std::cout << "SideWinder Tree NodeGrid:\n";
    std::cout << sidewinderNodeGrid;

    return 0;
}