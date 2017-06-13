#ifndef RAFALW_CSV_CONTEXT_HPP_
#define RAFALW_CSV_CONTEXT_HPP_

#include <string>
#include <string_view>

inline namespace rafalw {
namespace csv {

struct Context
{
    Context(std::string_view s) :
        source{ s }
    {}

    std::string source = "unknown";
    int line = 1;
};

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_CONTEXT_HPP_
