#ifndef RAFALW_UTILS_STRINGWRAPPER_HPP_
#define RAFALW_UTILS_STRINGWRAPPER_HPP_

#include <string_view>
#include <ostream>

inline namespace rafalw {
namespace utils {

template<typename TagT, typename StringT>
class StringWrapper
{
public:
	using Tag = TagT;
    using String = StringT;
    using Char = typename String::value_type;

    explicit StringWrapper(const std::basic_string_view<Char>& str) :
        m_string{ str.data(), str.length() }
    {}

    friend auto to_string(const StringWrapper& o) -> const String&
    {
        return o.m_string;
    }

private:
    String m_string;
};

template<typename TagT, typename StringT>
auto operator ==(const StringWrapper<TagT, StringT>& o1, const StringWrapper<TagT, StringT>& o2) -> bool
{
    return to_string(o1) == to_string(o2);
}

template<typename TagT, typename StringT>
auto operator <(const StringWrapper<TagT, StringT>& o1, const StringWrapper<TagT, StringT>& o2) -> bool
{
    return to_string(o1) < to_string(o2);
}

template<typename TagT, typename StringT>
auto operator !=(const StringWrapper<TagT, StringT>& o1, const StringWrapper<TagT, StringT>& o2) -> bool
{
    return to_string(o1) != to_string(o2);
}

template<typename TagT, typename StringT>
auto operator >=(const StringWrapper<TagT, StringT>& o1, const StringWrapper<TagT, StringT>& o2) -> bool
{
    return to_string(o1) >= to_string(o2);
}

template<typename TagT, typename StringT>
auto operator <=(const StringWrapper<TagT, StringT>& o1, const StringWrapper<TagT, StringT>& o2) -> bool
{
    return to_string(o1) <= to_string(o2);
}

template<typename TagT, typename StringT>
auto operator >(const StringWrapper<TagT, StringT>& o1, const StringWrapper<TagT, StringT>& o2) -> bool
{
    return to_string(o1) > to_string(o2);
}

} // namespace utils
} // namespace rafalw

template<typename TagT, typename StringT>
auto operator <<(std::ostream& os, const rafalw::utils::StringWrapper<TagT, StringT>& sw) -> std::ostream&
{
	return os << to_string(sw);
}

namespace std {

    template<typename TagT, typename StringT>
    struct hash<::rafalw::utils::StringWrapper<TagT, StringT>>
    {
        auto operator()(const ::rafalw::utils::StringWrapper<TagT, StringT>& wrapper) const -> size_t
        {
            return hash<StringT>{}(to_string(wrapper));
        }
    };
}

#endif // RAFALW_UTILS_STRINGWRAPPER_HPP_
