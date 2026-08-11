#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>

class Cell
{
public:
    Cell(const uint32_t row, const uint32_t column)
        : mRow(row),
          mColumn(column),
          mNorth(nullptr),
          mEast(nullptr),
          mSouth(nullptr),
          mWest(nullptr)
    {
        mKey = (static_cast<uint64_t>(mRow) << 32) | mColumn;
        mNorth = nullptr;
    }

    uint64_t key() const
    {
        return mKey;
    }

    void link(Cell& cell, const bool bidirectional = true)
    {
        mLinks[cell.key()] = &cell;

        if (bidirectional)
        {
            cell.link(*this, false);
        }
    }

    void unlink(Cell& cell, const bool bidirectional = true)
    {
        mLinks.erase(cell.key());

        if (bidirectional)
        {
            cell.unlink(*this, false);
        }
    }

    uint32_t row() const
    {
        return mRow;
    }

    uint32_t column() const
    {
        return mColumn;
    }

    std::vector<Cell*> links() const
    {
        std::vector<Cell*> result;
        result.reserve(mLinks.size());

        for (const auto& [key, cell] : mLinks)
        {
            result.push_back(cell);
        }

        return result;
    }

    bool linked(const Cell& cell)
    {
        return mLinks.contains(cell.key());
    }

    std::vector<Cell*> neighbors() const
    {
        std::vector<Cell*> neighbors;
        if (mNorth)
        {
            neighbors.push_back(mNorth);
        }
        if (mSouth)
        {
            neighbors.push_back(mSouth);
        }
        if (mWest)
        {
            neighbors.push_back(mWest);
        }
        if (mEast)
        {
            neighbors.push_back(mEast);
        }

        return neighbors;
    }

    Cell* getNorth()
    {
        return mNorth;
    }

    Cell* getSouth()
    {
        return mSouth;
    }

    Cell* getWest()
    {
        return mWest;
    }

    Cell* getEast()
    {
        return mEast;
    }

    void setNorth(Cell* cell)
    {
        mNorth = cell;
    }

    void setSouth(Cell* cell)
    {
        mSouth = cell;
    }

    void setEast(Cell* cell)
    {
        mEast = cell;
    }

    void setWest(Cell* cell)
    {
        mWest = cell;
    }

private:
    Cell* mNorth;
    Cell* mEast;
    Cell* mSouth;
    Cell* mWest;

private:
    uint32_t mRow;
    uint32_t mColumn;
    uint64_t mKey;

    std::unordered_map<uint32_t, Cell*> mLinks;
};