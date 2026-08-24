#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Cell.hpp"

#include <cstdint>
#include <memory>
#include <random>
#include <vector>
#include <format>
#include <sstream>

#include "stb/stb_image_write.h"

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
    void eachCell(F&& fn)
    {
        for (auto& row : mGrid)
        {
            for (auto& cell : row)
            {
                fn(*cell);
            }
        }
    }

    template <typename F>
    void eachRow(F&& fn)
    {
        for (auto& row : mGrid)
        {
            fn(row);
        }
    }

    uint32_t getRows() const
    {
        return mRows;
    }

    uint32_t getColumns() const
    {
        return mColumns;
    }

    void savePng(const int cellSizeInPixels, const std::string& filename, const int lineBorderSizeInPixels = 2)
    {
        const int width = getColumns() * cellSizeInPixels + lineBorderSizeInPixels * (getColumns() + 1);
        const int height = getRows() * cellSizeInPixels + lineBorderSizeInPixels * (getRows() + 1);

        std::vector<unsigned char> pixels(width * height * 3, 255);

        auto setPixel = [&](int x, int y, unsigned char r,
                        unsigned char g, unsigned char b)
        {
            if (x < 0 || x >= width || y < 0 || y >= height)
                return;

            auto index = (y * width + x) * 3;

            pixels[index + 0] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
        };

        auto drawHorizontalLine = [&](int x1, int x2, int y)
        {
            for (int x = x1; x <= x2; ++x)
            {
                for (int thickness = 0;
                    thickness < lineBorderSizeInPixels;
                    ++thickness)
                {
                    setPixel(x, y + thickness, 0, 0, 0);
                }
            }
        };

        auto drawVerticalLine = [&](int y1, int y2, int x)
        {
            for (int y = y1; y <= y2; ++y)
            {
                for (int thickness = 0;
                    thickness < lineBorderSizeInPixels;
                    ++thickness)
                {
                    setPixel(x + thickness, y, 0, 0, 0);
                }
            }
        };

        const int cellSizePlusBorderThickness = cellSizeInPixels + lineBorderSizeInPixels;

        eachCell([&](Cell& cell)
        {
            const int x1 = cell.column() * cellSizePlusBorderThickness;
            const int y1 = cell.row() * cellSizePlusBorderThickness;

            const int x2 = x1 + cellSizePlusBorderThickness;
            const int y2 = y1 + cellSizePlusBorderThickness;

            auto north = cell.getNorth();
            auto south = cell.getSouth();
            auto east = cell.getEast();
            auto west = cell.getWest();

            // North wall
            if (!north || !cell.linked(*north))
            {
                drawHorizontalLine(x1, x2, y1);
            }

            // South wall
            if (!south || !cell.linked(*south))
            {
                drawHorizontalLine(x1, x2+1, y2);
            }

            // West wall
            if (!west || !cell.linked(*west))
            {
                drawVerticalLine(y1, y2, x1);
            }

            // East wall
            if (!east || !cell.linked(*east))
            {
                drawVerticalLine(y1, y2, x2);
            }
        });

        stbi_write_png(
            filename.c_str(),
            width,
            height,
            3,
            pixels.data(),
            width * 3
        );
    }

private:
    uint32_t mRows = 0;
    uint32_t mColumns = 0;

    std::vector<std::vector<std::unique_ptr<Cell>>> mGrid;
};

std::ostream& operator<<(std::ostream& os, Grid& p)
{
    os << "+";
    for (uint32_t columns = 0; columns < p.getColumns(); columns += 1)
    {
        os << "---+";
    }
    os << '\n';

    p.eachRow([&os](std::vector<std::unique_ptr<Cell>>& row) 
    {
        os << "|";
        std::ostringstream bottom;
        bottom << "+";

        for (auto& cell : row)
        {
            os << "   "; // THREE (3) spaces
            auto east = cell->getEast();
            if (east && cell->linked(*east))
            {
                os << " ";
            }
            else
            {
                os << "|";
            }

            auto south = cell->getSouth();
            if (south && cell->linked(*south))
            {
                bottom << "   " << "+";
            }
            else
            {
                bottom << "---" << "+";
            }
        }

        os << '\n';
        os << bottom.str() << '\n';
    });

    return os;
}

/* 

#include <print>
template<>
struct std::formatter<Grid> : std::formatter<std::string>
{
    auto format(const Grid& grid, format_context& ctx) const
    {
        std::string output = "+" + "---+" * grid.columns ? `\n`;
        return std::formatter<std::string>::format(
            output,
            ctx
        );
    }
} */