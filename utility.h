#pragma once

// Miscellaneous functions that I do not know where else to put.

#include <string>

namespace utility {

// Removes directories and extension from a file path.
inline std::string extract_filename(const char* file_path)
{
    std::string filename(file_path);
    auto last_slash = filename.rfind("/");
    if (last_slash != std::string::npos)
    {
        filename = filename.substr(last_slash + 1);
    }
    auto first_period = filename.rfind(".");
    if (first_period != std::string::npos)
    {
        filename = filename.substr(0, first_period);
    }
    return filename;
}

} // namespace utility
