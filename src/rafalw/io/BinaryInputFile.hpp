#ifndef RAFALW_IO_BINARYINPUTFILE_HPP_
#define RAFALW_IO_BINARYINPUTFILE_HPP_

#include <rafalw/io/InputFile.hpp>

inline namespace rafalw {
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
} // namespace rafalw

#endif // RAFALW_IO_BINARYINPUTFILE_HPP_
