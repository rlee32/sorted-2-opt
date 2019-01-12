#pragma once

#include "primitives.h"

#include <vector>

struct Solution
{
    std::vector<primitives::point_id_t> ordered_points; // in order of traversal.
    primitives::length_t length{0};
};

