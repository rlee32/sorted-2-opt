#include "DistanceCalculator.h"
#include "TourModifier.h"
#include "aliases.h"
#include "debug.h"
#include "fileio/PointSet.h"
#include "fileio/fileio.h"
#include "primitives.h"
#include "solver.h"
#include "utility.h"

#include <iostream>

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cout << "Arguments: point_set_file_path optional_tour_file_path" << std::endl;
        return 0;
    }
    // Read input files.
    const fileio::PointSet point_set(argv[1]);
    const auto point_count = point_set.count();
    const auto initial_tour = fileio::initial_tour(argc, argv, point_count);
    // Initialize distance table.
    DistanceCalculator dt(point_set.x(), point_set.y());
    TourModifier tour_modifier(initial_tour);
    // Initialize sorted segments.
    Segment::SortedContainer segments;
    for (auto id : initial_tour)
    {
        segments.emplace(id, tour_modifier.next(id), dt);
    }
    // Initial tour stats.
    std::cout << "Largest segment length: " << std::crbegin(segments)->length << std::endl;
    std::cout << "Smallest segment length: " << std::cbegin(segments)->length << std::endl;
    const auto initial_tour_length = verify::tour_length(segments);
    std::cout << "Initial tour length: " << initial_tour_length << std::endl;
    std::cout << "Average segment length: " << initial_tour_length / static_cast<primitives::space_t>(segments.size()) << std::endl;
    // Optimization loop.
    auto filename = utility::extract_filename(argv[1]);
    solver::hill_climb(initial_tour, segments, dt, filename);
    return 0;
}
