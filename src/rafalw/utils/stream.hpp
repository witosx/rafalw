#ifndef RAFALW_UTILS_STREAM_HPP_
#define RAFALW_UTILS_STREAM_HPP_

#include <rafalw/utils/static.hpp>
#include <istream>
#include <ostream>

inline namespace rafalw {
namespace utils {

template<typename T>
auto stream_get(std::istream& is) -> T
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

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_STREAM_HPP_
