#pragma once

// Expresses connectivity and direction.

#include "primitives.h"

struct Connection
{
    Connection() = default;
    // for emplacement.
    Connection(primitives::point_id_t a, primitives::point_id_t b) : a(a), b(b) {}

    primitives::point_id_t a{0}; // first point.
    primitives::point_id_t b{0}; // second point (direction implied).
};

