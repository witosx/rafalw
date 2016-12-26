#ifndef RAFALW_IO_INPUTFILE_HPP_
#define RAFALW_IO_INPUTFILE_HPP_

#include <rafalw/io/File.hpp>
#include <rafalw/utils/Generator.hpp>
#include <rafalw/utils/static.hpp>
#include <rafalw/utils/demangle.hpp>
#include <boost/optional.hpp>

inline namespace rafalw {
namespace io {

template<typename Impl, std::ios_base::openmode M>
class InputFile : public File
{
public:
    class ReadError : public Error
    {
    public:
        ReadError(const std::string& path, std::istream::pos_type pos, const std::string& type) :
            Error{ "read failed for \"", path, "\" at offset ", pos, " [", type, "]" }
        {}
    };

    InputFile() :
        File{ M | std::ios_base::in }
    {}

    InputFile(const std::string& path) :
        File{ M | std::ios_base::in, path }
    {}

    auto position() -> std::istream::pos_type
    {
        return m_stream.tellg();
    }

    template<typename... Args>
    auto read(Args&... args) -> bool
    {
        requireOpened();
        return doRead(args...);
    }

    template<typename T>
    auto read() -> boost::optional<T>
    {
        requireOpened();
        return doRead<T>();
    }

    template<typename... Args>
    auto readAt(std::fstream::pos_type pos, Args&... args) -> bool
    {
        requireOpened();
        doSeek(pos);
        return doRead(args...);
    }

    template<typename T>
    auto readAt(std::fstream::pos_type pos) -> boost::optional<T>
    {
        requireOpened();
        doSeek(pos);
        return doRead<T>();
    }

    auto seek(std::istream::pos_type pos) -> void
    {
        requireOpened();
        doSeek(pos);
    }

private:
    auto doSeek(std::istream::pos_type pos) -> void
    {
        m_stream.seekg(pos);
        m_stream.clear();
    }

    template<typename... Args>
    auto doRead(Args&... args) -> bool
    {
        utils::static_foreach(std::tuple<Args&...>(args...), [this](auto& arg) {

            auto pos = m_stream.tellg();

            static_cast<Impl*>(this)->readImpl(arg);

            if (!m_stream.eof() && !m_stream)
                throw ReadError{ m_path, pos, utils::demangle<decltype(arg)>() };
        });

        return !m_stream.eof();
    }

    template<typename T>
    auto doRead() -> boost::optional<T>
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
