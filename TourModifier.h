#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <utility> // std::swap
#include <vector>

#include "Move.h"
#include "Connection.h"
#include "Segment.h"
#include "aliases.h"
#include "constants.h"
#include "primitives.h"


class TourModifier
{
    using Adjacents = std::array<primitives::point_id_t, 2>;
public:
    TourModifier(const std::vector<primitives::point_id_t>& initial_tour) { initialize(initial_tour); }

    void initialize(const std::vector<primitives::point_id_t>& initial_tour);
    void move(const Move&, aliases::SortedSegments& segments);
    std::vector<primitives::point_id_t> current_tour() const;
    primitives::point_id_t next(primitives::point_id_t i) const { return m_next[i]; }

private:
    std::vector<Adjacents> m_adjacents;
    std::vector<primitives::point_id_t> m_next;
    std::vector<primitives::point_id_t> m_sequence_ids; // indexed by first point id in a segment.

    void update_next();
    void reorder(const Move&);
    void align_segments(aliases::SortedSegments&) const;

    primitives::point_id_t get_other(primitives::point_id_t point, primitives::point_id_t adjacent) const;
    void create_adjacency(const Connection& c);
    void create_adjacency(primitives::point_id_t point1, primitives::point_id_t point2);
    void fill_adjacent(primitives::point_id_t point, primitives::point_id_t new_adjacent);
    void break_adjacency(const Connection& c);
    void break_adjacency(primitives::point_id_t point1, primitives::point_id_t point2);
    void vacate_adjacent_slot(primitives::point_id_t point, primitives::point_id_t adjacent, int slot);
};
