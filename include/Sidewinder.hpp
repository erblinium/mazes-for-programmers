#pragma once

#include "Grid.hpp"
#include "BitfieldGrid.hpp"
#include "NodeGrid.hpp"

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

        static void on(BitfieldGrid& grid)
        {
            static std::mt19937 generator(std::random_device{}());
            static std::uniform_int_distribution<int> distribution(0, 1);

            uint32_t rowIndex = 0;
            grid.eachRow([&](std::vector<std::uint8_t>& row)
                {
                    std::vector<uint32_t> cells;
                    uint32_t columnIndex = 0;
                    for (auto& cell : row)
                    {
                        cells.push_back(columnIndex);
                        const bool atEasternBoundary = !grid.hasEasternNeighbour(columnIndex);
                        const bool atSouthernBoundary = !grid.hasSouthernNeighbour(rowIndex);

                        bool shouldCloseOut = atEasternBoundary || (!atSouthernBoundary && distribution(generator) == 0);

                        if (shouldCloseOut)
                        {
                            auto memberNorth = grid.hasSouthernNeighbour(rowIndex);
                            if (memberNorth)
                            {
                                std::uniform_int_distribution<size_t> cellsDistribution(0, cells.size() - 1);
                                auto index = cellsDistribution(generator);
                                auto memberCol = cells[index];
                                grid.linkSouth(rowIndex, memberCol);
                            }
                            cells.clear();
                        }
                        else
                        {
                            grid.linkEast(rowIndex, columnIndex);
                        }

                        columnIndex += 1;
                    }
                    rowIndex += 1;
                }
            );
        }

        static void on(NodeGrid& grid)
        {
            static std::mt19937 generator(std::random_device{}());
            static std::uniform_int_distribution<int> distribution(0, 1);

            grid.eachRow([&](std::uint32_t row)
            {
                std::vector<std::size_t> cells;

                for (std::uint32_t column = 0;
                    column < grid.getColumns();
                    ++column)
                {
                    const auto mapIndex = grid.mapIndex(row, column);
                    cells.push_back(mapIndex);

                    const bool atEasternBoundary =
                        grid.atEasternBoundary(mapIndex);

                    const bool atSouthernBoundary =
                        grid.atSouthernBoundary(mapIndex);

                    const bool shouldCloseOut =
                        atEasternBoundary ||
                        (!atSouthernBoundary && distribution(generator) == 0);

                    if (shouldCloseOut)
                    {
                        std::uniform_int_distribution<std::size_t>
                            cellsDistribution(0, cells.size() - 1);

                        const auto index = cellsDistribution(generator);
                        const auto member = cells[index];

                        if (!grid.atSouthernBoundary(member))
                        {
                            grid.linkSouth(member);
                        }

                        cells.clear();
                    }
                    else
                    {
                        grid.linkEast(mapIndex);
                    }
                }
            });
        }
};