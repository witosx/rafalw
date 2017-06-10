#ifndef RAFALW_CSV_FILEREADER_HPP_
#define RAFALW_CSV_FILEREADER_HPP_

#include <rafalw/csv/StreamReader.hpp>
#include <rafalw/generator/Base.hpp>
#include <fstream>

inline namespace rafalw {
namespace csv {

template<typename StreamT>
class BasicFileReader : private generator::Base
{
private:
    using Stream = StreamT;
    using StreamReader = BasicStreamReader<typename Stream::char_type>;

public:
    using Char = typename StreamReader::Char;
    using String = typename StreamReader::String;
    using StringView = typename StreamReader::StringView;
    using Row = typename StreamReader::Row;

    BasicFileReader(std::string_view path, StringView row_delimiters, BaseRow::EmptyPolicy row_policy = BaseRow::EmptyPolicy::KEEP) :
        m_stream{ path.data() },
        m_streamReader{ m_stream, path, row_delimiters, row_policy }
    {}

private:
    friend class generator::BaseAccess;

    Stream m_stream;
    StreamReader m_streamReader;

    auto generatorDone() const -> bool
    {
        return done(m_streamReader);
    }

    auto generatorPeek() const -> const Row&
    {
        return peek(m_streamReader);
    }

    auto generatorUpdate() -> void
    {
        update(m_streamReader);
    }
};

using FileReader = BasicFileReader<std::basic_ifstream<char>>;

template<typename StreamT = std::basic_ifstream<char>>
inline auto reader(std::string_view path, typename BasicFileReader<StreamT>::StringView row_delimiters, BaseRow::EmptyPolicy row_policy = BaseRow::EmptyPolicy::KEEP) -> BasicFileReader<StreamT>
{
    return BasicFileReader<StreamT>{ path, row_delimiters, row_policy };
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_FILEREADER_HPP_
