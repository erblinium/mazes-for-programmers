#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <cstdint>
#include <vector>

class BitfieldGrid
{
public:
    /*
        cell bits = 01 → linked east
        cell bits = 10 → linked south
        cell bits = 11 → linked east AND south
        cell bits = 00 → isolated
    */
    enum Direction : std::uint8_t
    {
        East  = 0x01,
        South = 0x02,
        EastAndSouth = 0x03,
        Isolated = 0x00
    };
    
    BitfieldGrid(uint32_t rows, uint32_t columns)
        : mRows(rows), mColumns(columns)
    {
        prepareGrid_();
    }

    template <typename F>
    void eachCell(F&& fn)
    {
        for (std::size_t row = 0; row < mRows; row++)
        {
            for (std::size_t col = 0; col < mColumns; col++)
            {
                const auto cell = cell_(row, col);
                fn(cell, row, col);
            }
        }
    }

    template <typename F>
    void eachRow(F&& fn)
    {
        for (std::size_t row = 0; row < mRows; ++row)
        {
            std::vector<std::uint8_t> rowVec;
            rowVec.reserve(mColumns);

            for (std::size_t column = 0; column < mColumns; ++column)
            {
                rowVec.push_back(cell_(row, column));
            }

            fn(rowVec);
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

private:
    void prepareGrid_()
    {
        // Each cell only needs to know its south and east border as other cells will then contain the other remaining borders
        // Using a bitfield with uint64 fields we therefore can represent 64 bits / 2 (bits/cell) = 32 cells per word
        // The +31 is in the "worst case" scenario of a 1x1 grid. It is important that vector is initialized with all 0.
        mData = std::vector<uint64_t>(
            (mRows * mColumns + 31) / 32,
            std::uint64_t{0});
    }

    std::size_t cellIndex_(const std::size_t row, const std::size_t column) const
    {
        return row * mColumns + column;
    }   

    std::size_t wordIndex_(const std::size_t cellIndex) const
    {
        return cellIndex / 32;
    }

    std::size_t bitOffset_(std::size_t cellIndex) const
    {
        return (cellIndex % 32) * 2;
    }

    std::uint8_t cell_(std::size_t row, std::size_t column) const
    {
        const auto index = cellIndex_(row, column);
        const auto word = wordIndex_(index);
        const auto offset = bitOffset_(index);

        return (mData[word] >> offset) & 0b11;
    }

public:

    void setCell(
        std::size_t row,
        std::size_t column,
        std::uint8_t value)
    {
        const auto index = cellIndex_(row, column);
        const auto word = wordIndex_(index);
        const auto offset = bitOffset_(index);

        mData[word] &= ~(0b11ULL << offset);
        mData[word] |=
            (static_cast<std::uint64_t>(value) << offset);
    }

    void linkEast(std::size_t row, std::size_t column)
    {
        const auto index = cellIndex_(row, column);
        const auto word = wordIndex_(index);
        const auto offset = bitOffset_(index);

        mData[word] |=
            (static_cast<std::uint64_t>(Direction::East) << offset);
    }

    void linkSouth(std::size_t row, std::size_t column)
    {
        const auto index = cellIndex_(row, column);
        const auto word = wordIndex_(index);
        const auto offset = bitOffset_(index);

        mData[word] |=
            (static_cast<std::uint64_t>(Direction::South) << offset);
    }

    bool isLinkedEast(const std::uint8_t cell) const
    {
        return cell & Direction::East;
    }

    bool isLinkedSouth(const std::uint8_t cell) const
    {
        return cell & Direction::South;
    }

    bool hasSouthernNeighbour(const size_t row) const
    {
        if (row < mRows - 1)
        {
            return true;
        }

        return false;
    }

    bool hasEasternNeighbour(const size_t col) const
    {
        if (col + 1 < mColumns)
        {
            return true;
        }

        return false;
    }
private:
    uint32_t mRows = 0;
    uint32_t mColumns = 0;
    std::vector<std::uint64_t> mData;
};

std::ostream& operator<<(std::ostream& os, BitfieldGrid p)
{
    os << "+";
    for (uint32_t columns = 0; columns < p.getColumns(); columns += 1)
    {
        os << "---+";
    }
    os << '\n';

    size_t rowIndex = 0;
    p.eachRow([&os, &p, &rowIndex](std::vector<std::uint8_t>& row) 
    {
        os << "|";
        std::ostringstream bottom;
        bottom << "+";
        size_t colIndex = 0;
        for (auto& cell : row)
        {
            os << "   "; // THREE (3) spaces
            auto east = p.isLinkedEast(cell) && p.hasEasternNeighbour(colIndex);
            if (east)
            {
                os << " ";
            }
            else
            {
                os << "|";
            }

            auto south = p.isLinkedSouth(cell) && p.hasSouthernNeighbour(rowIndex);
            if (south)
            {
                bottom << "   " << "+";
            }
            else
            {
                bottom << "---" << "+";
            }

            colIndex += 1;
        }
        os << '\n';
        os << bottom.str() << '\n';
        rowIndex += 1;
    });

    return os;
}