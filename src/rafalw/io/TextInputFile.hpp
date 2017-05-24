#ifndef RAFALW_IO_TEXTINPUTFILE_HPP_
#define RAFALW_IO_TEXTINPUTFILE_HPP_

#include <rafalw/io/InputFile.hpp>
#include <rafalw/generator/Base.hpp>
#include <experimental/optional>

inline namespace rafalw {
namespace io {

template<typename StreamT>
class BasicTextInputFile : public InputFile<BasicTextInputFile<StreamT>, StreamT, std::ios_base::in>
{
public:
    class Lines : private generator::Base
    {
    public:
        Lines(BasicTextInputFile& input, char c) :
            m_input{ &input },
            m_char{ c }
        {
            generatorUpdate();
        }

    private:
        friend class generator::BaseAccess;

        BasicTextInputFile* m_input;
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

    using InputFile<BasicTextInputFile, StreamT, std::ios_base::in>::InputFile;

    auto readLine(std::string& line, char c = DEFAULT_LINE_SEP) -> bool
    {
        Base::requireOpened();
        return doReadLine(line, c);
    }

    auto readLine(char c = DEFAULT_LINE_SEP) -> std::experimental::optional<std::string>
    {
        Base::requireOpened();
        auto line = std::string{};
        if (!doReadLine(line, c))
            return {};
        return line;
    }

    auto lines(char c = DEFAULT_LINE_SEP) -> Lines
    {
        Base::requireOpened();
        return Lines{ *this, c };
    }

private:
    using Base = InputFile<BasicTextInputFile<StreamT>, StreamT, std::ios_base::in>;

    friend class Lines;
    friend Base;

    auto doReadLine(std::string& line, char c = DEFAULT_LINE_SEP) -> bool
    {
        auto pos = Base::stream().tellg();

        std::getline(Base::stream(), line, c);

        if (!Base::stream().eof() && !Base::stream())
            throw ReadError{ Base::path(), pos, "readline" };

        return !Base::stream().eof();
    }

    template<typename Arg>
    auto readImpl(Arg& arg) -> void
    {
        Base::stream() >> arg;
    }
};

using TextInputFile = BasicTextInputFile<std::fstream>;

} // namespace io
} // namespace rafalw

#endif // RAFALW_IO_TEXTINPUTFILE_HPP_
