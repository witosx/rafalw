#ifndef RAFALW_IO_OUTPUTFILE_HPP_
#define RAFALW_IO_OUTPUTFILE_HPP_

#include <rafalw/io/File.hpp>
#include <rafalw/utils/static.hpp>
#include <tuple>

inline namespace rafalw {
namespace io {

template<typename Impl, std::ios_base::openmode M>
class OutputFile : public File
{
public:
    class WriteError : public Error
    {
    public:
        WriteError(const std::string& path) :
            Error{ "write failed for \"", path, "\"" }
        {}
    };

    enum Mode
    {
        TRUNCATE,
        APPEND
    };

private:
    static auto mode(Mode m) -> std::ios_base::openmode
    {
        return M | std::ios_base::out | (m == Mode::TRUNCATE ? std::ios_base::trunc : std::ios_base::app);
    }

public:
    OutputFile(Mode m = Mode::TRUNCATE) :
        File{ mode(m) }
    {}

    OutputFile(const std::string& path, Mode m = Mode::TRUNCATE) :
        File{ mode(m), path }
    {}

    template<typename... Args>
    auto write(const Args&... args) -> void
    {
        requireOpened();

        utils::static_foreach(std::forward_as_tuple(args...), [this](auto& arg){
            static_cast<Impl*>(this)->writeImpl(arg);
        });

        if (!m_stream)
            throw WriteError{ m_path };
    }

    auto flush() -> void
    {
        if (m_stream.is_open())
            m_stream.flush();
    }
};

} // namespace io
} // namespace rafalw

#endif // RAFALW_IO_OUTPUTFILE_HPP_
