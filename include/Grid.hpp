#pragma once

#include "Cell.hpp"

#include <cstdint>
#include <memory>
#include <random>
#include <vector>

class Grid
{
public:
    Grid(uint32_t rows, uint32_t columns)
        : mRows(rows), mColumns(columns)
    {
        prepareGrid();
        configureCells();
    }

    void prepareGrid()
    {
        mGrid.resize(mRows);

        for (uint32_t row = 0; row < mRows; ++row)
        {
            mGrid[row].resize(mColumns);

            for (uint32_t column = 0; column < mColumns; ++column)
            {
                mGrid[row][column] =
                    std::make_unique<Cell>(row, column);
            }
        }
    }

    void configureCells()
    {
        for (uint32_t row = 0; row < mRows; ++row)
        {
            for (uint32_t column = 0; column < mColumns; ++column)
            {
                Cell* cell = mGrid[row][column].get();

                cell->setNorth(
                    (row > 0) ? mGrid[row - 1][column].get() : nullptr);

                cell->setSouth(
                    (row + 1 < mRows) ? mGrid[row + 1][column].get() : nullptr);

                cell->setWest(
                    (column > 0) ? mGrid[row][column - 1].get() : nullptr);

                cell->setEast(
                    (column + 1 < mColumns) ? mGrid[row][column + 1].get() : nullptr);
            }
        }
    }

    size_t size() const
    {
        return static_cast<size_t>(mRows) * mColumns;
    }

    Cell* randomCell()
    {
        if (mRows == 0 || mColumns == 0)
            return nullptr;

        static std::random_device rd;
        static std::mt19937 generator(rd());

        std::uniform_int_distribution<uint32_t> rowDist(0, mRows - 1);
        std::uniform_int_distribution<uint32_t> columnDist(0, mColumns - 1);

        return mGrid[rowDist(generator)][columnDist(generator)].get();
    }

    template <typename F>
    void eachCell(F&& fn) {
        for (auto& row : mGrid) {
            for (auto& cell : row) {
                fn(*cell);
            }
        }
    }

    template <typename F>
    void eachRow(F&& fn) {
        for (auto& row : mGrid) {
            fn(row);
        }
    }

private:
    uint32_t mRows = 0;
    uint32_t mColumns = 0;

    std::vector<std::vector<std::unique_ptr<Cell>>> mGrid;
};