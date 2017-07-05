#ifndef XTRAS_STRING_TOKENS_HPP_
#define XTRAS_STRING_TOKENS_HPP_

#include <xtras/generator/Base.hpp>
#include <string_view>

namespace xtras {
namespace string {

template<typename CharT>
class BasicTokens : private generator::Base
{
public:
    using Char = CharT;
    using StringView = std::basic_string_view<Char>;

    BasicTokens(const StringView& string, const StringView& separators) :
        m_stringFull{ string },
        m_separators{ separators }
    {
        generatorUpdate();
    }

private:
    friend class generator::BaseAccess;

    StringView m_stringFull;
    StringView m_separators;

    StringView m_string = m_stringFull;
    StringView m_token;
    int m_count = 0;

    auto generatorDone() const -> bool
    {
        return m_count >= 2;
    }

    auto generatorPeek() const -> const StringView&
    {
        return m_token;
    }

    auto generatorUpdate() -> void
    {
        const auto idx = m_string.find_first_of(m_separators);

        m_token = m_string.substr(0, idx);

        if (idx == StringView::npos && m_count < 2)
        {
            m_count++;
            m_string = {};
        }
        else
        {
            m_string.remove_prefix(idx + 1);
        }
    }

    auto generatorReset()
    {
        m_string = m_stringFull;
        m_token = {};
        m_count = 0;
        return generator::RESET_TAG<true>;
    }
};

using Tokens = BasicTokens<char>;

template<typename Char, typename Sep>
auto tokens(const Char* str, Sep&& sep) -> BasicTokens<Char>
{
    return BasicTokens<Char>{ str, std::forward<Sep>(sep) };
}

template<typename Char, typename Sep>
auto tokens(const std::basic_string<Char>& str, Sep&& sep) -> BasicTokens<Char>
{
    return BasicTokens<Char>{ str, std::forward<Sep>(sep) };
}

template<typename Char, typename Sep>
auto tokens(const std::basic_string_view<Char>& str, Sep&& sep) -> BasicTokens<Char>
{
    return BasicTokens<Char>{ str, std::forward<Sep>(sep) };
}

} // namespace string
} // namespace xtras

#endif // XTRAS_STRING_TOKENS_HPP_
