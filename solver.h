#pragma once

#include "DistanceTable.h"
#include "Move.h"
#include "Segment.h"
#include "Solution.h"
#include "TourModifier.h"
#include "aliases.h"
#include "fileio/fileio.h"
#include "primitives.h"
#include "verify.h"

#include <algorithm> // random_shuffle
#include <iterator>
#include <vector>

namespace solver {

inline Move first_improvement_sorted(const aliases::SortedSegments& segments, const DistanceTable& dt)
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

inline Move first_improvement_random(const aliases::SortedSegments& segments, const DistanceTable& dt)
{
    std::vector<Segment> random_access_segments;
    random_access_segments.assign(std::cbegin(segments), std::cend(segments));
    std::random_shuffle(std::begin(random_access_segments), std::end(random_access_segments));
    for (auto s1 = std::cbegin(random_access_segments); s1 != std::prev(std::cend(random_access_segments)); ++s1)
    {
        for (auto s2 = std::next(s1); s2 != std::cend(random_access_segments); ++s2)
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
    constexpr bool sorted_segment_order{false};
    auto move = (sorted_segment_order) ? first_improvement_sorted(segments, dt) : first_improvement_random(segments, dt);
    int iteration{1};
    while (move.improvement > 0)
    {
        tour_modifier.move(move, segments);
        const bool save = iteration % save_period == 0;
        if (save)
        {
            if (segments.size() != ordered_points.size())
            {
                std::cout << __func__ << ": ERROR: tour has become invalid: invalid segment count; actual, expected: "
                    << segments.size() << ", " << ordered_points.size() << std::endl;
                break;
            }
            if (not verify::valid_cycle(segments))
            {
                std::cout << __func__ << ": ERROR: tour has become invalid: invalid cycle.";
                break;
            }
            auto length = verify::tour_length(segments);
            std::cout << "Iteration " << iteration << " tour length: " << length << " (step improvement: " << move.improvement << ")\n";
            fileio::write_ordered_points(tour_modifier.current_tour()
                , "saves/" + save_file_prefix + "_" + std::to_string(length) + ".txt");
        }
        move = (sorted_segment_order) ? first_improvement_sorted(segments, dt) : first_improvement_random(segments, dt);
        ++iteration;
    }
    return {tour_modifier.current_tour(), verify::tour_length(segments)};
}

} // namespace solver
