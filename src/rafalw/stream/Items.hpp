#ifndef RAFALW_STREAM_ITEMS_HPP_
#define RAFALW_STREAM_ITEMS_HPP_

#include <rafalw/stream/Error.hpp>
#include <rafalw/generator/Base.hpp>
#include <istream>
#include <string>

inline namespace rafalw {
namespace stream {

template<typename ItemT, typename CharT = char>
class Items : private generator::Base
{
public:
    using Item = ItemT;
    using Char = CharT;
    using Stream = std::basic_istream<Char>;

    template<typename... ItemArgsT>
    explicit Items(Stream& stream, ItemArgsT&&... item_args) :
        m_stream{ stream },
        m_item{ std::forward<ItemArgsT>(item_args)... }
    {
        generatorUpdate();
    }

private:
    friend class generator::BaseAccess;

    Stream& m_stream;
    Item m_item;

    auto generatorPeek() const -> const Item&
    {
        streamCheckErrors();
        return m_item;
    }

    auto generatorUpdate() -> void
    {
        m_stream >> m_item;
    }

    auto generatorDone() const -> bool
    {
        return !m_stream && m_stream.eof();
    }

    auto streamCheckErrors() const -> void
    {
        if (!m_stream && !m_stream.eof())
            throw Error{ "read failed" };
    }
};

template<typename Item = std::string, typename Char, typename... ItemArgsT>
auto items(std::basic_istream<Char>& stream, ItemArgsT&&... item_args) -> Items<Item, Char>
{
    return Items<Item, Char>{ stream, std::forward<ItemArgsT>(item_args)... };
}

} // namespace stream
} // namespace rafalw

#endif // RAFALW_STREAM_ITEMS_HPP_
