#pragma once

// Information needed for a 2-opt exchange.

#include "Segment.h"
#include "primitives.h"

#include <array>

struct Move
{
    primitives::length_t improvement{0};
    std::array<Segment, 2> old_segments;
    std::array<Segment, 2> new_segments;
};
