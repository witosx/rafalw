#ifndef RAFALW_IO_INPUTFILE_HPP_
#define RAFALW_IO_INPUTFILE_HPP_

#include <rafalw/io/File.hpp>
#include <rafalw/utils/static.hpp>
#include <rafalw/utils/demangle.hpp>
#include <experimental/optional>

inline namespace rafalw {
namespace io {

template<typename DerivedT, typename StreamT, std::ios_base::openmode OpenModeV>
class InputFile : public File<StreamT>
{
public:
    InputFile(const std::string& path) :
        Base{ path, OpenModeV | std::ios_base::in }
    {}

    auto position() -> std::istream::pos_type
    {
        return Base::stream().tellg();
    }

    template<typename... Args>
    auto read(Args&... args) -> bool
    {
        Base::requireOpened();
        return doRead(args...);
    }

    template<typename T>
    auto read() -> std::experimental::optional<T>
    {
        Base::requireOpened();
        return doRead<T>();
    }

    template<typename... Args>
    auto readAt(std::fstream::pos_type pos, Args&... args) -> bool
    {
        Base::requireOpened();
        doSeek(pos);
        return doRead(args...);
    }

    template<typename T>
    auto readAt(std::fstream::pos_type pos) -> std::experimental::optional<T>
    {
        Base::requireOpened();
        doSeek(pos);
        return doRead<T>();
    }

    auto seek(std::istream::pos_type pos) -> void
    {
        Base::requireOpened();
        doSeek(pos);
    }

private:
    using Base = File<StreamT>;
    using Derived = DerivedT;

    auto doSeek(std::istream::pos_type pos) -> void
    {
        Base::stream().seekg(pos);
        Base::stream().clear();
    }

    template<typename... Args>
    auto doRead(Args&... args) -> bool
    {
        utils::static_foreach(std::tuple<Args&...>(args...), [this](auto& arg) {

            auto pos = Base::stream().tellg();

            static_cast<Derived*>(this)->readImpl(arg);

            if (!Base::stream().eof() && !Base::stream())
                throw ReadError{ Base::path(), pos, utils::demangle<decltype(arg)>() };
        });

        return !Base::stream().eof();
    }

    template<typename T>
    auto doRead() -> std::experimental::optional<T>
    {
        auto v = T{};
        if (!doRead(v))
            return {};

        return v;
    }
};

} // namespace io
} // namespace rafalw

#endif // RAFALW_IO_INPUTFILE_HPP_
