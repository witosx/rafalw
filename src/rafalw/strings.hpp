#ifndef RAFALW_STRINGS_HPP_
#define RAFALW_STRINGS_HPP_

#include <rafalw/utils/static.hpp>
#include <algorithm>

inline namespace rafalw {
namespace strings {

template<typename Char>
auto replace(std::basic_string<Char> str, Char c1, Char c2) -> std::basic_string<Char>
{
    std::replace(str.begin(), str.end(), c1, c2);
    return str;
}

template<typename Char>
auto remove(std::basic_string<Char> str, Char c) -> std::basic_string<Char>
{
    auto it = std::remove(str.begin(), str.end(), c);
    str.erase(it, str.end());
    return str;
}

template<typename Char>
class Joiner
{
public:
    Joiner(const Char* s) :
        m_separator{ s }
    {}

    template<typename... Strs>
    auto operator ()(std::basic_string<Char> str0, const Strs&... strs) const -> std::basic_string<Char>
    {
        utils::static_foreach(std::forward_as_tuple(strs...), [this,&str0](const auto& s){
            str0 += m_separator;
            str0 += s;
        });

        return str0;
    }

    template<typename... Strs>
    auto operator ()(const Char* str0, const Strs&... strs) const -> std::basic_string<Char>
    {
        return operator ()(std::basic_string<Char>{ str0 }, strs...);
    }

    template<typename... Strs>
    auto operator ()(Char c, const Strs&... strs) const -> std::basic_string<Char>
    {
        return operator ()(std::basic_string<Char>(1, c), strs...);
    }

private:
    const Char* m_separator;
};

template<typename Char>
auto joiner(const Char* s) -> Joiner<Char>
{
    return Joiner<Char>{ s };
}

template<typename... Strs>
auto join(const Strs&... strs) -> std::string
{
    return joiner("")(strs...);
}

} // namespace strings
} // namespace rafalw

#endif // RAFALW_STRINGS_HPP_
