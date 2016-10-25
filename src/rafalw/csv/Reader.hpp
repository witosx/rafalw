#ifndef RAFALW_CSV_READER_HPP_
#define RAFALW_CSV_READER_HPP_

#include <rafalw/csv/ReaderBase.hpp>

inline namespace rafalw {
namespace csv {

template<typename T, typename F>
class Reader : public ReaderBase<Reader<T, F>, T, true>
{
private:
    using Parser = F;
    using Base = ReaderBase<Reader<T, F>, T, true>;
    using typename Base::Char;
    using typename Base::String;

    Parser m_parser;

public:
    Reader(const std::string& path, String delimiters, F parser, Empty ep = Empty::KEEP) :
        Base{ path, delimiters, ep },
        m_parser{ parser }
    {
        Base::update();
    }

    auto processLine(Line&& l) const -> T
    {
        return m_parser(std::move(l));
    }
};

template<typename F>
auto reader(const std::string& path, const std::string& delimiters, F&& f) -> Reader<decltype(f(std::declval<Line>())), F>
{
    return Reader<decltype(f(std::declval<Line>())), F>{ path, delimiters, f };
}

template<typename F>
auto reader(const std::string& path, const std::string& delimiters, Empty ep, F&& f) -> Reader<decltype(f(std::declval<Line>())), F>
{
    return Reader<decltype(f(std::declval<Line>())), F>{ path, delimiters, f, ep };
}

} // namespace csv
} // inline namespace rafalw

#endif // RAFALW_CSV_READER_HPP_
