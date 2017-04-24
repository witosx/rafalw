#ifndef RAFALW_STREAMS_MISC_HPP_
#define RAFALW_STREAMS_MISC_HPP_

#include <boost/iterator/iterator_facade.hpp>
#include <rafalw/utils/static.hpp>
#include <rafalw/utils/assert.hpp>
#include <istream>
#include <ostream>
#include <sstream>

inline namespace rafalw {
namespace streams {

template<typename T>
auto get(std::istream& is) -> T
{
    auto e = T{};
    is >> e;

    if (!is)
        throw std::runtime_error{ "stream read error" };

    return e;
}

template<typename T>
auto convert(const char* cstr, std::size_t n) -> T
{
    auto ss = std::stringstream{};
    ss.write(cstr, n);
    return get<T>(ss);
}

template<typename T>
auto convert(const std::string& str) -> T
{
    return convert<T>(str.c_str(), str.length());
}


template<typename... Args>
class StreamJoined
{
private:
    const std::string& m_separator;
    std::tuple<Args...> m_fields;

public:
    template<typename... Args2>
    StreamJoined(const std::string& sep, Args2&&... args) :
        m_separator{ sep },
        m_fields{ std::forward<Args2>(args)... }
    {}

    friend auto operator <<(std::ostream& os, const StreamJoined& obj) -> std::ostream&
    {
        auto init = true;

        static_foreach(obj.m_fields, [&](auto& e) {
            if (init)
                init = false;
            else
                os << obj.m_separator;
            os << e;
        });

        return os;
    }
};

template<typename T>
using RemoveRvalueReference = std::conditional_t<std::is_rvalue_reference<T>::value, std::remove_reference<T>, T>;

template<typename... Args>
auto stream_joined(const std::string& sep, Args&&... args) -> StreamJoined<RemoveRvalueReference<std::decay_t<Args>>...>
{
    return StreamJoined<RemoveRvalueReference<std::decay_t<Args>>...>{ sep, std::forward<Args>(args)... };
}

template<typename T>
class StreamRange
{
public:
    StreamRange(T range) :
        m_range{ static_cast<T>(range) }
    {}

    friend auto operator <<(std::ostream& os, const StreamRange& obj) -> std::ostream&
    {
        auto sep = "";

        for (auto&& e: obj.m_range)
        {
            os << sep << e;
            sep = " ";
        }

        return os;
    }

private:
    T m_range;
};

template<typename T>
auto stream_range(T&& range) -> StreamRange<T&&>
{
    return StreamRange<T&&>{ std::forward<T>(range) };
}

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
    utils::static_foreach(o.args, [&os](auto& e){
        os << e;
    });
    return os;
}

template<typename... Args>
auto stream_args(const Args&... args) -> StreamArgs<const Args&...>
{
    return StreamArgs<const Args&...>{ args... };
}

template<typename _Char, typename _Sep>
class SeparatedWriter
{
public:
    using Char = _Char;
    using Separator = _Sep;
    using Stream = std::basic_ostream<Char>;

    template<typename... Args>
    SeparatedWriter(Stream& stream, Args&&... args) :
        m_stream{ stream },
        m_separator{ std::forward<Args>(args)... }
    {}

    template<typename T>
    auto operator ()(const T& value) -> void
    {
        if (m_init)
            m_init = false;
        else
            m_stream << m_separator;

        m_stream << value;
    }

private:
    Stream& m_stream;
    Separator m_separator;

    bool m_init = true;
};

template<typename Char>
auto separated_writer(std::basic_ostream<Char>& stream, Char sep) -> SeparatedWriter<Char, Char>
{
    return SeparatedWriter<Char, Char>{ stream, sep };
}

template<typename Char>
auto separated_writer(std::basic_ostream<Char>& stream, const Char* sep) -> SeparatedWriter<Char, const Char*>
{
    return SeparatedWriter<Char, const Char*>{ stream, sep };
}

template<typename Char>
auto separated_writer(std::basic_ostream<Char>& stream, const std::basic_string<Char>& sep) -> SeparatedWriter<Char, std::basic_string<Char>>
{
    return SeparatedWriter<Char, std::basic_string<Char>>{ stream, sep };
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

} // namespace streams
} // namespace rafalw

#endif // RAFALW_STREAMS_MISC_HPP_
