#ifndef XTRAS_IO_FILE_HPP_
#define XTRAS_IO_FILE_HPP_

#include <xtras/utils/Error.hpp>
#include <fstream>

namespace xtras {
namespace io {

class Error : public utils::Error
{
public:
    template<typename... Args>
    Error(const Args&... args) :
        utils::Error{ "File error: ", args... }
    {}
};

class OpenError : public Error
{
public:
    OpenError(const std::string& path) :
        Error{ "open failed for \"", path, "\"" }
    {}
};

class ClosedError : public Error
{
public:
    ClosedError() :
        Error{ "closed" }
    {}
};

class ReadError : public Error
{
public:
    ReadError(const std::string& path, std::istream::pos_type pos, const std::string& type) :
        Error{ "read failed for \"", path, "\" at offset ", pos, " [", type, "]" }
    {}
};

class WriteError : public Error
{
public:
    WriteError(const std::string& path) :
        Error{ "write failed for \"", path, "\"" }
    {}
};

template<typename StreamT = std::fstream>
class File
{
public:
    using Stream = StreamT;
    using Char = typename Stream::char_type;

    File(const std::string& path, std::ios_base::openmode mode) :
        m_path{ path },
        m_mode{ mode }
    {
        open();
    }

    auto reopen() -> void
    {
        close();
        open();
    }

    auto close() -> void
    {
        m_stream.close();
        m_stream.clear();
    }

    auto opened() const -> bool
    {
        return m_stream.is_open();
    }

    auto path() const -> const std::string&
    {
        return m_path;
    }

protected:
    auto requireOpened() const -> void
    {
        if (!opened())
            throw ClosedError{};
    }

    auto stream() -> Stream&
    {
        return m_stream;
    }

    auto stream() const -> const Stream&
    {
        return m_stream;
    }

private:
    Stream m_stream;
    std::string m_path;
    std::ios_base::openmode m_mode;

    auto open() -> void
    {
        m_stream.open(m_path, m_mode);

        if (!opened())
            throw OpenError{ m_path };
    }
};

} // namespace io
} // namespace xtras

#endif // XTRAS_IO_FILE_HPP_
