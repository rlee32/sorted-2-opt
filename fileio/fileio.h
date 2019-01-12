#pragma once

#include "Tour.h"
#include "primitives.h"

#include <fstream>
#include <vector>

namespace fileio {

inline void write_ordered_points(const std::vector<primitives::point_id_t>& ordered_points
    , const std::string output_filename)
{
    std::ofstream output_file;
    output_file.open(output_filename);
    output_file << "DIMENSION: " << ordered_points.size() << "\n";
    output_file << "TOUR_SECTION\n";
    for (auto p : ordered_points)
    {
        output_file << p + 1 << "\n";
    }
}

inline std::vector<primitives::point_id_t> read_ordered_points(const char* file_path)
{
    return Tour(file_path).point_ids();
}

inline std::vector<primitives::point_id_t> default_tour(primitives::point_id_t point_count)
{
    std::vector<primitives::point_id_t> tour;
    for (primitives::point_id_t i{0}; i < point_count; ++i)
    {
        tour.push_back(i);
    }
    return tour;
}

inline std::vector<primitives::point_id_t> initial_tour(int argc, const char** argv, primitives::point_id_t point_count)
{
    std::vector<primitives::point_id_t> tour;
    if (argc > 2)
    {
        tour = read_ordered_points(argv[2]);
    }
    else
    {
        tour = default_tour(point_count);
    }
    return tour;
}

} // namespace fileio
