#ifndef RAFALW_STRINGS_TOKENS_HPP_
#define RAFALW_STRINGS_TOKENS_HPP_

#include <rafalw/utils/Generator.hpp>
#include <boost/utility/string_view.hpp>

inline namespace rafalw {
namespace strings {

template<typename CharT>
class BasicTokens : public utils::Generator<BasicTokens<CharT>, boost::basic_string_view<CharT>>
{
public:
	using Base = utils::Generator<BasicTokens<CharT>, boost::basic_string_view<CharT>>;

	using Char = CharT;
	using StringView = boost::basic_string_view<Char>;

	BasicTokens(const StringView& string, const StringView& separators) :
        m_string{ string },
        m_separators{ separators }
    {
    	Base::update();
    }

private:
    friend class utils::GeneratorAccess;

    StringView m_string;
    StringView m_separators;
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
        	m_string.clear();
        }
        else
        {
        	m_string.remove_prefix(idx + 1);
        }
    }
};

using Tokens = BasicTokens<char>;

} // namespace strings
} // namespace rafalw

#endif // RAFALW_STRINGS_TOKENS_HPP_
