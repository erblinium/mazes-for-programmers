#include "Grid.hpp"
#include "BinaryTree.hpp"

int main(int argc, char* argv[])
{
    Grid grid(4, 4);
    BinaryTree::on(grid);
    return 0;
}