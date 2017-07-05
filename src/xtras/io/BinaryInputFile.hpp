#ifndef XTRAS_IO_BINARYINPUTFILE_HPP_
#define XTRAS_IO_BINARYINPUTFILE_HPP_

#include <xtras/io/InputFile.hpp>

namespace xtras {
namespace io {

template<typename StreamT>
class BasicBinaryInputFile : public InputFile<BasicBinaryInputFile<StreamT>, StreamT, std::ios_base::binary>
{
private:
    using Base = InputFile<BasicBinaryInputFile<StreamT>, StreamT, std::ios_base::binary>;
    using Base::Base;

    friend Base;

    template<typename Arg>
    auto readImpl(Arg& arg) -> void
    {
        Base::stream().read(reinterpret_cast<char*>(&arg), sizeof(arg));
    }
};

using BinaryInputFile = BasicBinaryInputFile<std::fstream>;

} // namespace io
} // namespace xtras

#endif // XTRAS_IO_BINARYINPUTFILE_HPP_
