#ifndef RAFALW_CSV_SOURCEERROR_HPP_
#define RAFALW_CSV_SOURCEERROR_HPP_

#include <rafalw/csv/Context.hpp>
#include <rafalw/csv/Error.hpp>

inline namespace rafalw {
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
} // namespace rafalw

#endif // RAFALW_CSV_SOURCEERROR_HPP_
