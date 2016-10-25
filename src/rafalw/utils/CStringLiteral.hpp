#ifndef RAFALW_UTILS_CSTRINGLITERAL_HPP_
#define RAFALW_UTILS_CSTRINGLITERAL_HPP_

#include <cstdlib>

inline namespace rafalw {
namespace utils {

template<typename _Char>
class BasicCStringLiteral
{
public:
    using Char = _Char;
    using Length = std::size_t;

private:
    const Char* m_data;
    Length m_length;

public:
    constexpr BasicCStringLiteral() :
        BasicCStringLiteral{ "" }
    {}

    template<Length _SIZE>
    constexpr BasicCStringLiteral(const Char (&data)[_SIZE]) :
        m_data{ data },
        m_length{ _SIZE - 1 }
    {}

    constexpr auto data() const -> const Char*
    {
        return m_data;
    }

    constexpr auto length() const -> Length
    {
        return m_length;
    }

    constexpr auto empty() const -> bool
    {
        return m_length == 0;
    }
};

using CStringLiteral = BasicCStringLiteral<char>;

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_CSTRINGLITERAL_HPP_
