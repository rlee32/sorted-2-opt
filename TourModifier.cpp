#include "TourModifier.h"

void TourModifier::initialize(const std::vector<primitives::point_id_t>& initial_tour)
{
    m_adjacents.resize(initial_tour.size());
    m_sequence_ids.resize(initial_tour.size());
    for (auto& a : m_adjacents)
    {
        a = {constants::invalid_point, constants::invalid_point};
    }
    auto prev = initial_tour.back();
    for (auto p : initial_tour)
    {
        create_adjacency(p, prev);
        prev = p;
    }
    m_next.resize(initial_tour.size());
    update_next();
}

std::vector<primitives::point_id_t> TourModifier::current_tour() const
{
    std::vector<primitives::point_id_t> points(m_next.size(), constants::invalid_point);
    constexpr primitives::point_id_t start_point{0};
    primitives::point_id_t i{start_point};
    int sequence{0};
    do
    {
        points[sequence] = i;
        i = m_next[i];
        ++sequence;
    } while (i != start_point);
    return points;
}

void TourModifier::move(const Move& move, aliases::SortedSegments& segments)
{
    reorder(move);
    update_next();
    segments.erase(move.old_segments[0]);
    segments.erase(move.old_segments[1]);
    segments.insert(move.new_segments[0]);
    segments.insert(move.new_segments[1]);
    // align after adding / removing segments.
    align_segments(segments);
}

void TourModifier::align_segments(aliases::SortedSegments& segments) const
{
    // std::sets only use const iterators, as element values determine order.
    std::vector<Segment> deleted;
    auto it = std::cbegin(segments);
    while (it != std::cend(segments))
    {
        if (it->a == m_next[it->b])
        {
            deleted.push_back(*it);
            std::swap(deleted.back().a, deleted.back().b);
            it = segments.erase(it);
        }
        else
        {
            ++it;
        }

    }
    for (const auto& d : deleted)
    {
        segments.insert(d);
    }
}

void TourModifier::update_next()
{
    primitives::point_id_t current{0};
    primitives::point_id_t sequence_id{0};
    m_sequence_ids[current] = sequence_id;
    m_next[current] = m_adjacents[current].front();
    do
    {
        auto prev = current;
        current = m_next[current];
        m_sequence_ids[current] = ++sequence_id;
        m_next[current] = get_other(current, prev);
    } while (current != 0); // tour cycle condition.
}

void TourModifier::reorder(const Move& move)
{
    // break old segments.
    break_adjacency(move.old_segments[0]);
    break_adjacency(move.old_segments[1]);
    // form new segments.
    create_adjacency(move.new_segments[0]);
    create_adjacency(move.new_segments[1]);
}

primitives::point_id_t TourModifier::get_other(primitives::point_id_t point, primitives::point_id_t adjacent) const
{
    const auto& a = m_adjacents[point];
    if (a.front() == adjacent)
    {
        return a.back();
    }
    else
    {
        return a.front();
    }
}

void TourModifier::create_adjacency(const Connection& c)
{
    create_adjacency(c.a, c.b);
}

void TourModifier::create_adjacency(primitives::point_id_t point1, primitives::point_id_t point2)
{
    fill_adjacent(point1, point2);
    fill_adjacent(point2, point1);
}

void TourModifier::fill_adjacent(primitives::point_id_t point, primitives::point_id_t new_adjacent)
{
    if (m_adjacents[point].front() == constants::invalid_point)
    {
        m_adjacents[point].front() = new_adjacent;
    }
    else if (m_adjacents[point].back() == constants::invalid_point)
    {
        m_adjacents[point].back() = new_adjacent;
    }
}

void TourModifier::break_adjacency(const Connection& c)
{
    break_adjacency(c.a, c.b);
}

void TourModifier::break_adjacency(primitives::point_id_t point1, primitives::point_id_t point2)
{
    vacate_adjacent_slot(point1, point2, 0);
    vacate_adjacent_slot(point1, point2, 1);
    vacate_adjacent_slot(point2, point1, 0);
    vacate_adjacent_slot(point2, point1, 1);
}

void TourModifier::vacate_adjacent_slot(primitives::point_id_t point, primitives::point_id_t adjacent, int slot)
{
    if (m_adjacents[point][slot] == adjacent)
    {
        m_adjacents[point][slot] = constants::invalid_point;
    }
}

