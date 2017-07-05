#ifndef XTRAS_CSV_CONTEXT_HPP_
#define XTRAS_CSV_CONTEXT_HPP_

#include <string>
#include <string_view>

namespace xtras {
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
} // namespace xtras

#endif // XTRAS_CSV_CONTEXT_HPP_
