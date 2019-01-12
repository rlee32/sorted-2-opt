#pragma once

#include "primitives.h"

namespace constants {

constexpr auto invalid_point{std::numeric_limits<primitives::point_id_t>::max()};

constexpr int save_period{1};

// if true, segments will be considered for replacement in order of length (largest first).
// if false, segments will be considered in random order.
constexpr bool sorted_segment_order{false};

} // namespace constants
