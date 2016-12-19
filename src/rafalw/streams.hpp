#ifndef RAFALW_STREAMS_HPP_
#define RAFALW_STREAMS_HPP_

#include <rafalw/utils/static.hpp>
#include <istream>
#include <ostream>

inline namespace rafalw {
namespace streams {

template<typename T>
auto get(std::istream& is) -> T
{
    auto e = T{};
    is >> e;
    return e;
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
    StreamRange(const T& range) :
        m_range{ range }
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
    const T& m_range;
};

template<typename T>
auto stream_range(const T& range) -> StreamRange<T>
{
    return StreamRange<T>{ range };
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

} // namespace streams
} // namespace rafalw

#endif // RAFALW_STREAMS_HPP_
