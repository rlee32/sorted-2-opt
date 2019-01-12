#pragma once

#include "Connection.h"
#include "primitives.h"

#include <set>
#include <vector>

namespace debug {

inline void check_connections(const std::vector<Connection>& connections)
{
    std::set<primitives::point_id_t> seen;
    for (const auto& c : connections)
    {
        if (seen.find(c.a) != seen.end() or seen.find(c.b) != seen.end())
        {
            std::cout << "ERROR: " << __func__ << std::endl;
        }
        seen.insert(c.a);
        seen.insert(c.b);
    }
    std::cout << "check complete.\n";
}

}
