#pragma once

#include "Grid.hpp"
#include "BitfieldGrid.hpp"

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

        static void on(BitfieldGrid& grid)
        {
            static std::mt19937 generator(std::random_device{}());

            grid.eachCell([&](std::uint8_t cell, const std::size_t row, const std::size_t column)
            {
                uint8_t neighbours = 0;

                const auto canLinkSouth = grid.hasSouthernNeighbour(row);
                const auto canLinkEast = grid.hasEasternNeighbour(column);

                if (!canLinkEast && !canLinkSouth)
                {
                    return;
                }

                if (canLinkEast && canLinkSouth)
                {
                    std::uniform_int_distribution<std::size_t> distribution(0, 1);

                    if(distribution(generator) == 0)
                    {
                        grid.linkEast(row, column);
                    }
                    else
                    {
                        grid.linkSouth(row, column);
                    }
                }
                else if(canLinkEast)
                {
                    grid.linkEast(row, column);
                }
                else if(canLinkSouth)
                {
                    grid.linkSouth(row, column);
                }
            });
        }
};