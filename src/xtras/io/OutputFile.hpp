#ifndef XTRAS_IO_OUTPUTFILE_HPP_
#define XTRAS_IO_OUTPUTFILE_HPP_

#include <xtras/io/File.hpp>
#include <xtras/utils/static.hpp>
#include <tuple>

namespace xtras {
namespace io {

template<typename DerivedT, typename StreamT, std::ios_base::openmode M>
class OutputFile : public File<StreamT>
{
public:
    enum Mode
    {
        TRUNCATE,
        APPEND
    };

private:
    using Base = File<StreamT>;
    using Derived = DerivedT;

    static auto mode(Mode m) -> std::ios_base::openmode
    {
        return M | std::ios_base::out | (m == Mode::TRUNCATE ? std::ios_base::trunc : std::ios_base::app);
    }

public:
    OutputFile(const std::string& path, Mode m = Mode::TRUNCATE) :
        Base{ path, mode(m) }
    {}

    template<typename... Args>
    auto write(const Args&... args) -> void
    {
        Base::requireOpened();

        static_foreach(std::forward_as_tuple(args...), [this](auto& arg){
            static_cast<Derived*>(this)->writeImpl(arg);
        });

        if (!Base::stream())
            throw WriteError{ Base::path() };
    }

    auto flush() -> void
    {
        if (Base::stream())
            Base::stream().flush();
    }
};

} // namespace io
} // namespace xtras

#endif // XTRAS_IO_OUTPUTFILE_HPP_
