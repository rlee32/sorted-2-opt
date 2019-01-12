#pragma once

// This represents a straight line path between two adjacent cities.
// A tour is made of a number of segments equal to the number of cities.

#include "Connection.h"
#include "DistanceTable.h"
#include "primitives.h"

#include <ostream>

struct Segment : Connection
{
public:
    Segment() = default;
    Segment(primitives::point_id_t a, primitives::point_id_t b, primitives::length_t length, const DistanceTable& dt)
        : Connection(a, b), length(length)
        , xm(0.5 * (dt.x(a) + dt.x(b)))
        , ym(0.5 * (dt.y(a) + dt.y(b))) {}
    Segment(primitives::point_id_t a, primitives::point_id_t b, const DistanceTable& dt)
        : Segment(a, b, dt.compute_length(a, b), dt) {}
    Segment(const Connection& c, const DistanceTable& dt) : Segment(c.a, c.b, dt) {}
    // temporary fix.
    Segment(const Connection& c) : Connection(c) {}

    primitives::length_t length{0};
    primitives::space_t xm{0}; // x-coordinate of segment midpoint.
    primitives::space_t ym{0}; // y-coordinate of segment midpoint.

    // For use in set.
    struct LengthComparator
    {
        bool operator()(const Segment& lhs, const Segment& rhs) const
        {
            return (lhs.length == rhs.length)
                ? Hash()(lhs) < Hash()(rhs)
                : lhs.length < rhs.length;
        }
    };
};

inline std::ostream& operator<<(std::ostream& out, const Segment& s)
{
    return out << "Point " << s.a << " to point " << s.b << " with length " << s.length;
}

