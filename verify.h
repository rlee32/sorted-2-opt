#pragma once

#include "Segment.h"
#include "constants.h"
#include "primitives.h"

#include <iostream>
#include <numeric> // accumulate
#include <vector>

namespace verify {

inline std::vector<primitives::point_id_t> reproduce_next(const Segment::SortedContainer& segments)
{
    std::vector<primitives::point_id_t> next(segments.size(), constants::invalid_point);
    for (const auto& s : segments)
    {
        next[s.a] = s.b;
    }
    return next;
}

inline std::vector<int> reproduce_visited(const std::vector<primitives::point_id_t>& next)
{
    std::vector<int> visited(next.size(), 0);
    constexpr primitives::point_id_t start_point{0};
    primitives::point_id_t current{start_point};
    do
    {
        // std::cout << current << std::endl;
        ++visited[current];
        current = next[current];
    } while (current != start_point);
    return visited;
}

inline bool valid_cycle(const Segment::SortedContainer& segments)
{
    for (const auto& s : segments)
    {
        if (s.a >= segments.size() or s.b >= segments.size())
        {
            std::cout << __func__ << ": ERROR: invalid tour; invalid segment! point ids: " << s.a << ", " << s.b << "\n";
            std::cout << __func__ << ": ERROR: invalid tour; invalid segment! segment count: " << segments.size() << "\n";
            return false;
        }
    }
    const auto next = reproduce_next(segments);
    for (const auto& n : next)
    {
        if (n >= segments.size() or n >= segments.size())
        {
            std::cout << __func__ << ": ERROR: invalid tour; invalid next!\n";
            return false;
        }
    }
    const auto visited = reproduce_visited(next);
    // check visited points.
    for (primitives::point_id_t i{0}; i < visited.size(); ++i)
    {
        if (visited[i] < 1)
        {
            std::cout << __func__ << ": ERROR: invalid tour: city " << i << " not visited.\n";
            return false;
        }
        else if (visited[i] > 1)
        {
            std::cout << __func__ << ": ERROR: invalid tour: city visited " << visited[i] << " times.\n";
            return false;
        }
    }
    return true;
}

inline primitives::length_t tour_length(const Segment::SortedContainer& segments)
{
    auto segment_length = [](primitives::length_t sum, const Segment& s) { return sum + s.length; };
    return std::accumulate(std::cbegin(segments), std::cend(segments), 0, segment_length);
}

} // namespace verify
