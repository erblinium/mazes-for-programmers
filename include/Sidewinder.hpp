#pragma once

#include "Grid.hpp"

class Sidewinder
{
    public:
        static void on(Grid& grid)
        {
            static std::mt19937 generator(std::random_device{}());
            static std::uniform_int_distribution<int> distribution(0, 1);
            
            grid.eachRow([&](std::vector<std::unique_ptr<Cell>>& row)
                {
                    std::vector<Cell*> cells;

                    for (auto& cell : row)
                    {
                        cells.push_back(cell.get());

                        bool atEasternBoundary = (cell->getEast() == nullptr);
                        bool atNorthernBoundary = (cell->getNorth() == nullptr);

                        bool shouldCloseOut = atEasternBoundary || (!atNorthernBoundary && distribution(generator) == 0);

                        if (shouldCloseOut)
                        {
                            std::uniform_int_distribution<size_t> cellsDistribution(0, cells.size() - 1);
                            auto index = cellsDistribution(generator);
                            auto member = cells[index];

                            auto memberNorth = member->getNorth();
                            if (memberNorth)
                            {
                                member->link(*memberNorth);
                            }
                            cells.clear();
                        }
                        else
                        {
                            cell->link(*cell->getEast());
                        }
                    }
                }
            );
        }
};