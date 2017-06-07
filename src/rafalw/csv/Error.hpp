#ifndef RAFALW_CSV_ERROR_HPP_
#define RAFALW_CSV_ERROR_HPP_

#include <rafalw/csv/Context.hpp>
#include <rafalw/utils/Error.hpp>

inline namespace rafalw {
namespace csv {

class Error : public utils::Error
{
public:
    template<typename... Args>
    Error(const Context& context, const Args&... args) :
        utils::Error{ "csv ", context.source, " line ", context.line, " ", args..., },
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

#endif // RAFALW_CSV_ERROR_HPP_
