#ifndef RAFALW_IO_BINARYOUTPUTFILE_HPP_
#define RAFALW_IO_BINARYOUTPUTFILE_HPP_

#include <rafalw/io/OutputFile.hpp>

inline namespace rafalw {
namespace io {

class BinaryOutputFile : public OutputFile<BinaryOutputFile, std::ios_base::binary>
{
private:
    using OutputFile<BinaryOutputFile, std::ios_base::binary>::OutputFile;

    friend class OutputFile<BinaryOutputFile, std::ios_base::binary>;

    template<typename Arg>
    auto writeImpl(const Arg& arg) -> void
    {
        m_stream.write(reinterpret_cast<const char*>(&arg), sizeof(arg));
    }
};

} // namespace io
} // namespace rafalw

#endif // RAFALW_IO_BINARYOUTPUTFILE_HPP_
