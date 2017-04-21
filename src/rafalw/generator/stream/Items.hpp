#ifndef RAFALW_GENERATOR_STREAM_ITEMS_HPP_
#define RAFALW_GENERATOR_STREAM_ITEMS_HPP_

#include <rafalw/generator/Base.hpp>
#include <istream>
#include <string>

inline namespace rafalw {
namespace generator {
namespace stream {

template<typename ItemT, typename CharT = char>
class Items : private Base
{
public:
    using Item = ItemT;
    using Char = CharT;
    using Stream = std::basic_istream<Char>;

    explicit Items(Stream& stream) :
        m_stream{ stream }
    {
        generatorUpdate();
    }

private:
    friend class generator::BaseAccess;

    Stream& m_stream;
    Item m_item;

    auto generatorPeek() const -> const Item&
    {
        return m_item;
    }

    auto generatorUpdate() -> void
    {
        m_stream >> m_item;
    }

    auto generatorDone() const -> bool
    {
        return m_stream.fail() || m_stream.bad();
    }
};

template<typename Item = std::string, typename Char = char>
auto items(std::basic_istream<Char>& stream) -> Items<Item, Char>
{
    return Items<Item, Char>{ stream };
}

} // namespace stream
} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_STREAM_ITEMS_HPP_
