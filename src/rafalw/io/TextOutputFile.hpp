#ifndef RAFALW_IO_TEXTOUTPUTFILE_HPP_
#define RAFALW_IO_TEXTOUTPUTFILE_HPP_

#include <rafalw/io/OutputFile.hpp>

inline namespace rafalw {
namespace io {

template<typename StreamT>
class BasicTextOutputFile : public OutputFile<BasicTextOutputFile<StreamT>, StreamT, std::ios_base::out>
{
public:
    template<typename... Args>
    auto writeLine(const Args&... args) -> void
    {
        Base::write(args..., '\n');
    }

private:
    using Base = OutputFile<BasicTextOutputFile<StreamT>, StreamT, std::ios_base::out>;
    using Base::Base;

    friend Base;

    template<typename Arg>
    auto writeImpl(const Arg& arg) -> void
    {
        Base::stream().setf(std::ios::fixed, std::ios::floatfield);
        Base::stream() << arg;
    }
};

using TextOutputFile = BasicTextOutputFile<std::fstream>;

} // namespace io
} // namespace rafalw

#endif // RAFALW_IO_TEXTOUTPUTFILE_HPP_
