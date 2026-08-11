#pragma once

#include "Grid.hpp"

class BinaryTree
{
    public:
        static void on(Grid& grid)
        {
            static std::mt19937 generator(std::random_device{}());

            grid.eachCell([](Cell& cell) 
            {
                std::vector<Cell*> neighbors;

                if (auto* north = cell.getNorth())
                {
                    neighbors.push_back(north);
                }

                if (auto* east = cell.getEast())
                {
                    neighbors.push_back(east);
                }

                if (neighbors.empty())
                {
                    return;
                }

                std::uniform_int_distribution<std::size_t> distribution(
                    0, neighbors.size() - 1
                );

                Cell* neighbor = neighbors[distribution(generator)];

                cell.link(*neighbor);
            });
        }
};