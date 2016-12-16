#ifndef RAFALW_STRINGS_HPP_
#define RAFALW_STRINGS_HPP_

#include <rafalw/utils/static.hpp>
#include <rafalw/streams.hpp>
#include <boost/tokenizer.hpp>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include <tuple>
#include <vector>

inline namespace rafalw {
namespace strings {

template<typename T>
auto str(const T& obj) -> std::string
{
    using std::to_string;
    return to_string(obj);
}

inline auto str(const std::string& obj) -> const std::string&
{
    return obj;
}

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
auto join(Strs&&... strs) -> std::string
{
    return joiner("")(std::move(strs)...);
}

template<typename Char>
auto split(const std::basic_string<Char>& s, Char c) -> std::vector<std::basic_string<Char>>
{
    auto res = std::vector<std::basic_string<Char>>{};
    auto idx1 = typename std::basic_string<Char>::size_type{ 0 };

    while (true)
    {
        const auto idx2 = s.find(c, idx1);

        if (idx2 == std::basic_string<Char>::npos)
        {
            res.push_back(s.substr(idx1));
            break;
        }

        res.push_back(s.substr(idx1, idx2 - idx1));
        idx1 = idx2 + 1;
    }

    return res;
}


template<typename CharT, typename... Args>
auto basic_stringify(const Args&... args) -> std::basic_string<CharT>;

template<typename... Args>
auto stringify(const Args&... args) -> std::basic_string<char>;


template<typename CharT, typename... Args>
auto basic_stringify(const Args&... args) -> std::basic_string<CharT>
{
    auto ss = std::basic_ostringstream<CharT>{};
    ss << streams::stream_args(args...);
    return ss.str();
}

template<typename... Args>
auto stringify(const Args&... args) -> std::basic_string<char>
{
    return basic_stringify<char>(args...);
}

template<typename CharT>
auto join(const std::vector<std::basic_string<CharT>>& strings, const std::basic_string<CharT>& sep) -> std::basic_string<CharT>
{
    if (strings.empty())
        return {};

    auto total_size = sep.size() * (strings.size() - 1);
    for (auto& e: strings)
        total_size += e.size();

    auto result = std::basic_string<CharT>{};
    result.reserve(total_size);

    auto first = true;
    for (auto& e: strings)
    {
        if (first)
            first = false;
        else
            result += sep;

        result += e;
    }

    return result;
}

template<typename CharT>
auto join(const std::vector<std::basic_string<CharT>>& strings, CharT sep) -> std::basic_string<CharT>
{
    return join(strings, std::basic_string<CharT>{ sep });
}

template<typename Char>
auto tokenize(const std::basic_string<Char>& str, Char separator) -> std::vector<std::basic_string<Char>>
{
    auto boost_separator = boost::escaped_list_separator<Char>{ '\\', separator };
    auto boost_tokenizer = boost::tokenizer<decltype(boost_separator), typename std::basic_string<Char>::const_iterator, std::basic_string<Char>>{ str, boost_separator };
    return std::vector<std::basic_string<Char>>{ boost_tokenizer.begin(), boost_tokenizer.end() };
}

} // namespace strings
} // namespace rafalw

#endif // RAFALW_STRINGS_HPP_
