#ifndef RAFALW_IO_FILE_HPP_
#define RAFALW_IO_FILE_HPP_

#include <rafalw/utils/Error.hpp>
#include <fstream>

inline namespace rafalw {
namespace io {

class File
{
public:
    class Error : public utils::Error
    {
    public:
        template<typename... Args>
        Error(const Args&... args) :
            utils::Error{ "File error: ", args... }
        {}
    };

    class ClosedError : public Error
    {
    public:
        ClosedError() :
            Error{ "closed" }
        {}
    };

    class OpenError : public Error
    {
    public:
        OpenError(const std::string& path) :
            Error{ "open failed for \"", path, "\"" }
        {}
    };

protected:
    std::fstream m_stream;
    std::string m_path;
    std::ios_base::openmode m_mode;

    File(std::ios_base::openmode mode) :
        m_mode{ mode }
    {}

    File(std::ios_base::openmode mode, const std::string& path) :
        File{ mode }
    {
        open(path);
    }

    auto requireOpened() const -> void
    {
        if (!opened())
            throw ClosedError{};
    }

public:
    auto open(const std::string& path) -> void
    {
        if (m_stream.is_open())
        {
            m_stream.close();
            m_stream.clear();
        }

        m_stream.open(path, m_mode);

        if (!opened())
            throw OpenError{ path };

        m_path = path;
    }

    auto close() -> void
    {
        m_stream.close();
        m_path.clear();
    }

    auto opened() const -> bool
    {
        return m_stream.is_open();
    }

    auto path() const -> const std::string&
    {
        requireOpened();
        return m_path;
    }
};

} // namespace io
} // namespace rafalw

#endif // RAFALW_IO_FILE_HPP_
