#ifndef XTRAS_IO_BINARYOUTPUTFILE_HPP_
#define XTRAS_IO_BINARYOUTPUTFILE_HPP_

#include <xtras/io/OutputFile.hpp>

namespace xtras {
namespace io {

template<typename StreamT>
class BasicBinaryOutputFile : public OutputFile<BasicBinaryOutputFile<StreamT>, StreamT, std::ios_base::binary>
{
private:
    using Base = OutputFile<BasicBinaryOutputFile<StreamT>, StreamT, std::ios_base::binary>;
    using Base::Base;

    friend Base;

    template<typename Arg>
    auto writeImpl(const Arg& arg) -> void
    {
        Base::stream().write(reinterpret_cast<const char*>(&arg), sizeof(arg));
    }
};

using BinaryOutputFile = BasicBinaryOutputFile<std::fstream>;

} // namespace io
} // namespace xtras

#endif // XTRAS_IO_BINARYOUTPUTFILE_HPP_
