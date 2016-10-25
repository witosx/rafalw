#ifndef RAFALW_IO_TEXTOUTPUTFILE_HPP_
#define RAFALW_IO_TEXTOUTPUTFILE_HPP_

#include <rafalw/io/OutputFile.hpp>

inline namespace rafalw {
namespace io {

class TextOutputFile : public OutputFile<TextOutputFile, std::ios_base::out>
{
public:
    template<typename... Args>
    auto writeLine(const Args&... args) -> void
    {
        write(args..., '\n');
    }

private:
    using OutputFile<TextOutputFile, std::ios_base::out>::OutputFile;

    friend class OutputFile<TextOutputFile, std::ios_base::out>;

    template<typename Arg>
    auto writeImpl(const Arg& arg) -> void
    {
        m_stream.setf(std::ios::fixed, std::ios::floatfield);
        m_stream << arg;
    }
};

} // namespace io
} // namespace rafalw

#endif // RAFALW_IO_TEXTOUTPUTFILE_HPP_
