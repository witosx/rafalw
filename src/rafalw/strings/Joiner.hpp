#ifndef RAFALW_STRINGS_JOINER_HPP_
#define RAFALW_STRINGS_JOINER_HPP_

#include <string>

inline namespace rafalw {
namespace strings {

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
auto join(Strs&&... strs) -> std::string
{
    return joiner("")(std::move(strs)...);
}

} // namespace strings
} // namespace rafalw

#endif // RAFALW_STRINGS_JOINER_HPP_
