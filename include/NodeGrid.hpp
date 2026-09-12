#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <utility>
#include <vector>

class NodeGrid
{
public:
    using NodeId = std::size_t;
    using Neighbors = std::vector<NodeId>;

    NodeGrid(std::uint32_t rows, std::uint32_t columns)
        : mRows(rows)
        , mColumns(columns)
        , mGraph(static_cast<std::size_t>(rows) * columns)
    {
    }

    std::uint32_t getRows() const noexcept
    {
        return mRows;
    }

    std::uint32_t getColumns() const noexcept
    {
        return mColumns;
    }

    template <typename F>
    void eachCell(F&& fn)
    {
        for (std::uint32_t row = 0; row < mRows; ++row)
        {
            for (std::uint32_t column = 0; column < mColumns; ++column)
            {
                const NodeId node = mapIndex(row, column);
                fn(node, row, column);
            }
        }
    }

    template <typename F>
    void eachCell(F&& fn) const
    {
        for (std::uint32_t row = 0; row < mRows; ++row)
        {
            for (std::uint32_t column = 0; column < mColumns; ++column)
            {
                const NodeId node = mapIndex(row, column);
                fn(node, row, column);
            }
        }
    }

    template <typename F>
    void eachRow(F&& fn)
    {
        for (std::uint32_t row = 0; row < mRows; ++row)
        {
            fn(row);
        }
    }

    void linkVertices(NodeId a, NodeId b)
    {
        mGraph[a].push_back(b);
        mGraph[b].push_back(a);
    }

    void linkEast(NodeId a)
    {
        const auto row = row_(a);
        const auto column = column_(a) + 1;

        const auto eastIndex = mapIndex(row, column);

        linkVertices(a, eastIndex);
    }

    void linkSouth(NodeId a)
    {
        const auto row = row_(a) + 1;
        const auto column = column_(a);

        const auto southIndex = mapIndex(row, column);

        linkVertices(a, southIndex); 
    }

    bool atEasternBoundary(NodeId node) const
    {
        return column_(node) == mColumns - 1;
    }

    bool atSouthernBoundary(NodeId node) const
    {
        return row_(node) == mRows - 1;
    }

    bool isLinkedEast(NodeId node) const
    {
        if (atEasternBoundary(node))
        {
            return false;
        }

        return isLinked_(node, east_(node));
    }

    bool isLinkedSouth(NodeId node) const
    {
        if (atSouthernBoundary(node))
        {
            return false;
        }

        return isLinked_(node, south_(node));
    }

    NodeId mapIndex(std::uint32_t row, std::uint32_t column) const noexcept
    {
        return static_cast<NodeId>(row) * mColumns + column;
    }

private:
    std::uint32_t row_(NodeId node) const noexcept
    {
        return static_cast<std::uint32_t>(node / mColumns);
    }

    std::uint32_t column_(NodeId node) const noexcept
    {
        return static_cast<std::uint32_t>(node % mColumns);
    }

    NodeId east_(NodeId node) const noexcept
    {
        return node + 1;
    }

    NodeId south_(NodeId node) const noexcept
    {
        return node + mColumns;
    }

    bool isLinked_(NodeId a, NodeId b) const
    {
        const auto& neighbors = mGraph[a];

        return std::find(
            neighbors.begin(),
            neighbors.end(),
            b
        ) != neighbors.end();
    }

private:
    std::uint32_t mRows = 0;
    std::uint32_t mColumns = 0;
    std::vector<Neighbors> mGraph;
};

inline std::ostream& operator<<(std::ostream& os, const NodeGrid& grid)
{
    // Top border.
    os << '+';

    for (std::uint32_t column = 0;
         column < grid.getColumns();
         ++column)
    {
        os << "---+";
    }

    os << '\n';

    for (std::uint32_t row = 0;
         row < grid.getRows();
         ++row)
    {
        // Vertical walls / horizontal connections.
        os << '|';

        for (std::uint32_t column = 0;
             column < grid.getColumns();
             ++column)
        {
            const std::size_t node =
                static_cast<std::size_t>(row) * grid.getColumns() + column;

            os << "   ";

            if (column + 1 < grid.getColumns() &&
                grid.isLinkedEast(node))
            {
                os << ' ';
            }
            else
            {
                os << '|';
            }
        }

        os << '\n';

        // Bottom walls / vertical connections.
        os << '+';

        for (std::uint32_t column = 0;
             column < grid.getColumns();
             ++column)
        {
            const std::size_t node =
                static_cast<std::size_t>(row) * grid.getColumns() + column;

            if (row + 1 < grid.getRows() &&
                grid.isLinkedSouth(node))
            {
                os << "   +";
            }
            else
            {
                os << "---+";
            }
        }

        os << '\n';
    }

    return os;
}