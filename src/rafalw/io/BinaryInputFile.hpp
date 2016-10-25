#ifndef RAFALW_IO_BINARYINPUTFILE_HPP_
#define RAFALW_IO_BINARYINPUTFILE_HPP_

#include <rafalw/io/InputFile.hpp>

inline namespace rafalw {
namespace io {

class BinaryInputFile : public InputFile<BinaryInputFile, std::ios_base::binary>
{
private:
    using InputFile<BinaryInputFile, std::ios_base::binary>::InputFile;

    friend class InputFile<BinaryInputFile, std::ios_base::binary>;

    template<typename Arg>
    auto readImpl(Arg& arg) -> void
    {
        m_stream.read(reinterpret_cast<char*>(&arg), sizeof(arg));
    }
};

} // namespace io
} // inline namespace rafalw

#endif // RAFALW_IO_BINARYINPUTFILE_HPP_
