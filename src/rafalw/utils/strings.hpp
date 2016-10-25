#ifndef RAFALW_UTILS_STRINGS_HPP_
#define RAFALW_UTILS_STRINGS_HPP_

#include <rafalw/utils/static.hpp>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <ostream>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>

inline namespace rafalw {
namespace utils {

template<typename T>
auto str(const T& obj) -> std::string
{
    using std::to_string;

    return to_string(obj);
}

template<typename CharT, typename... Args>
auto basic_stringify(const Args&... args) -> std::basic_string<CharT>;

template<typename... Args>
auto stringify(const Args&... args) -> std::basic_string<char>;

template<typename... Args>
struct StreamArgs
{
    StreamArgs(Args... args) :
        args{ args... }
    {}

    std::tuple<Args...> args;
};

template<typename CharT, typename... Args>
auto operator <<(std::basic_ostream<CharT>& os, const StreamArgs<Args...>& o) -> std::basic_ostream<CharT>&
{
    static_foreach(o.args, [&os](auto& e){
        os << e;
    });
    return os;
}

template<typename... Args>
auto stream_args(const Args&... args) -> StreamArgs<const Args&...>
{
    return StreamArgs<const Args&...>{ args... };
}

template<typename CharT, typename... Args>
auto basic_stringify(const Args&... args) -> std::basic_string<CharT>
{
    auto ss = std::basic_ostringstream<CharT>{};
    ss << stream_args(args...);
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

template<typename Char>
auto replace(std::basic_string<Char> str, Char c1, Char c2) -> std::basic_string<Char>
{
    std::replace(str.begin(), str.end(), c1, c2);
    return str;
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_STRINGS_HPP_
