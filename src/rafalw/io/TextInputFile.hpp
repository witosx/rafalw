#ifndef RAFALW_IO_TEXTINPUTFILE_HPP_
#define RAFALW_IO_TEXTINPUTFILE_HPP_

#include <rafalw/io/InputFile.hpp>
#include <rafalw/utils/Generator.hpp>
#include <boost/optional.hpp>

inline namespace rafalw {
namespace io {

class TextInputFile : public InputFile<TextInputFile, std::ios_base::in>
{
public:
    class Lines : public utils::Generator<Lines>
    {
    public:
        Lines(TextInputFile& input, char c) :
            m_input{ &input },
            m_char{ c }
        {
            generatorUpdate();
        }

    private:
        friend class utils::GeneratorAccess;

        TextInputFile* m_input;
        char m_char;

        std::string m_line;
        bool m_valid = true;

        auto generatorDone() const -> bool
        {
            return !m_valid;
        }

        auto generatorPeek() const -> const std::string&
        {
            return m_line;
        }

        auto generatorUpdate() -> void
        {
            m_valid = m_input->doReadLine(m_line, m_char);
        }
    };

    static constexpr auto DEFAULT_LINE_SEP = '\n';

    using InputFile<TextInputFile, std::ios_base::in>::InputFile;

    auto readLine(std::string& line, char c = DEFAULT_LINE_SEP) -> bool
    {
        requireOpened();
        return doReadLine(line, c);
    }

    auto readLine(char c = DEFAULT_LINE_SEP) -> boost::optional<std::string>
    {
        requireOpened();
        auto line = std::string{};
        if (!doReadLine(line, c))
            return {};
        return line;
    }

    auto lines(char c = DEFAULT_LINE_SEP) -> Lines
    {
        requireOpened();
        return Lines{ *this, c };
    }

private:
    friend class Lines;
    friend class InputFile<TextInputFile, std::ios_base::in>;

    auto doReadLine(std::string& line, char c = DEFAULT_LINE_SEP) -> bool
    {
        auto pos = m_stream.tellg();

        std::getline(m_stream, line, c);

        if (!m_stream.eof() && !m_stream)
            throw ReadError{ m_path, pos, "readline" };

        return !m_stream.eof();
    }

    template<typename Arg>
    auto readImpl(Arg& arg) -> void
    {
        m_stream >> arg;
    }
};

} // namespace io
} // namespace rafalw

#endif // RAFALW_IO_TEXTINPUTFILE_HPP_
