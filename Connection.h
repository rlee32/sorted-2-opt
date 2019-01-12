#pragma once

// Expresses connectivity and direction.

#include "primitives.h"

#include <algorithm> // min, max
#include <cstdint>
#include <unordered_set>

struct Connection
{
    Connection() = default;
    // for emplacement.
    Connection(primitives::point_id_t a, primitives::point_id_t b) : a(a), b(b) {}

    primitives::point_id_t a{0}; // first point.
    primitives::point_id_t b{0}; // second point (direction implied).

    primitives::point_id_t min() const
    {
        return std::min(a, b);
    }

    primitives::point_id_t max() const
    {
        return std::max(a, b);
    }

    bool compatible(const Connection& other) const
    {
        return other.a != a and other.b != a and other.a != b and other.b != b;
    }

    // For use in unordered_set.
    struct Hash
    {
        using hash_t = uint64_t;
        hash_t operator()(const Connection& c) const
        {
            static_assert(sizeof(hash_t) == sizeof(c.a) + sizeof(c.b));
            constexpr auto ShiftBits = 4 * sizeof(hash_t);
            static_assert(ShiftBits / 8 == sizeof(c.b));
            return (static_cast<hash_t>(c.min()) << ShiftBits) + static_cast<hash_t>(c.max());
        }
    };

    using Container = std::unordered_set<Connection, Hash>;
};

// For use in unordered_set.
inline bool operator==(const Connection& lhs, const Connection& rhs)
{
    return lhs.min() == rhs.min() and lhs.max() == rhs.max();
}

