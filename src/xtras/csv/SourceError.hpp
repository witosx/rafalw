#ifndef XTRAS_CSV_SOURCEERROR_HPP_
#define XTRAS_CSV_SOURCEERROR_HPP_

#include <xtras/csv/Context.hpp>
#include <xtras/csv/Error.hpp>

namespace xtras {
namespace csv {

class SourceError : public Error
{
public:
    template<typename... Args>
    SourceError(const Context& context, const Args&... args) :
        Error{ "csv source:", context.source, " row:", context.line, " ", args..., },
        m_context{ context }
    {}

    auto context() const -> const Context&
    {
        return m_context;
    }

private:
    Context m_context;
};

} // namespace csv
} // namespace xtras

#endif // XTRAS_CSV_SOURCEERROR_HPP_
