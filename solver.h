#pragma once

#include "DistanceTable.h"
#include "Move.h"
#include "PointSequence.h"
#include "Segment.h"
#include "Solution.h"
#include "TourModifier.h"
#include "aliases.h"
#include "fileio/fileio.h"
#include "primitives.h"
#include "verify.h"

#include <iterator>
#include <vector>

namespace solver {

inline Move first_improvement(const aliases::SortedSegments& segments, const DistanceTable& dt)
{
    for (auto s1 = std::crbegin(segments); s1 != std::prev(std::crend(segments)); ++s1)
    {
        for (auto s2 = std::next(s1); s2 != std::crend(segments); ++s2)
        {
            if (not s1->compatible(*s2))
            {
                continue;
            }
            const auto current_length = s1->length + s2->length;
            Segment new_segment_1(s1->a, s2->a, dt);
            auto new_length = new_segment_1.length;
            if (new_length >= current_length)
            {
                continue;
            }
            Segment new_segment_2(s1->b, s2->b, dt);
            new_length += new_segment_2.length;
            if (new_length >= current_length)
            {
                continue;
            }
            const auto improvement = current_length - new_length;
            const auto old_segment_1 = *s1;
            const auto old_segment_2 = *s2;
            return {improvement, {old_segment_1, old_segment_2}, {new_segment_1, new_segment_2}};
        }
    }
    return {};
}

inline Solution hill_climb(const std::vector<primitives::point_id_t>& ordered_points
    , aliases::SortedSegments& segments
    , const DistanceTable& dt
    , const std::string save_file_prefix)
{
    constexpr int save_period{500};
    TourModifier tour_modifier(ordered_points);
    auto move = first_improvement(segments, dt);
    int iteration{1};
    while (move.improvement > 0)
    {
        tour_modifier.move(move, segments);
        const bool save = iteration % save_period == 0;
        if (save)
        {
            if (not verify::valid_cycle(segments))
            {
                std::cout << __func__ << ": ERROR: tour has become invalid!" << std::endl;
                break;
            }
            auto length = verify::tour_length(segments);
            std::cout << "Iteration " << iteration << " tour length: " << length << " (step improvement: " << move.improvement << ")\n";
            fileio::write_ordered_points(tour_modifier.current_tour()
                , "saves/" + save_file_prefix + "_" + std::to_string(length) + ".txt");
        }
        move = first_improvement(segments, dt);
        ++iteration;
    }
    return {tour_modifier.current_tour(), verify::tour_length(segments)};
}

} // namespace solver
