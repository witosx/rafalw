#ifndef RAFALW_UTILS_STRINGWRAPPER_HPP_
#define RAFALW_UTILS_STRINGWRAPPER_HPP_

#include <boost/utility/string_view.hpp>

inline namespace rafalw {
namespace utils {

template<typename StringT>
class StringWrapper
{
public:
    using String = StringT;

    explicit StringWrapper(const boost::string_view& str) :
        m_string{ str.data(), str.length() }
    {}

    friend auto to_string(const StringWrapper& f) -> const String&
    {
        return f.m_string;
    }

private:
    String m_string;
};

template<typename T>
auto operator ==(const StringWrapper<T>& f1, const StringWrapper<T>& f2) -> bool
{
    return to_string(f1) == to_string(f2);
}

template<typename T>
auto operator <(const StringWrapper<T>& f1, const StringWrapper<T>& f2) -> bool
{
    return to_string(f1) < to_string(f2);
}

template<typename T>
auto operator !=(const StringWrapper<T>& f1, const StringWrapper<T>& f2) -> bool
{
    return to_string(f1) != to_string(f2);
}

template<typename T>
auto operator >=(const StringWrapper<T>& f1, const StringWrapper<T>& f2) -> bool
{
    return to_string(f1) >= to_string(f2);
}

template<typename T>
auto operator <=(const StringWrapper<T>& f1, const StringWrapper<T>& f2) -> bool
{
    return to_string(f1) <= to_string(f2);
}

template<typename T>
auto operator >(const StringWrapper<T>& f1, const StringWrapper<T>& f2) -> bool
{
    return to_string(f1) > to_string(f2);
}

} // namespace utils
} // namespace rafalw

namespace std {

    template<typename StringT>
    struct hash<rafalw::utils::StringWrapper<StringT>>
    {
        auto operator()(const rafalw::utils::StringWrapper<StringT>& wrapper) const -> std::size_t
        {
            return std::hash<StringT>{}(to_string(wrapper));
        }
    };
}

#endif // RAFALW_UTILS_STRINGWRAPPER_HPP_
