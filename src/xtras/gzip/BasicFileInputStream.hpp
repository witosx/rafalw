#ifndef XTRAS_GZIP_BASICFILEINPUTSTREAM_HPP_
#define XTRAS_GZIP_BASICFILEINPUTSTREAM_HPP_

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <istream>
#include <fstream>
#include <memory>

namespace xtras {
namespace gzip {

template<typename CharT>
class BasicFileInputStream : public std::basic_istream<CharT>
{
private:
    using Base = std::basic_istream<CharT>;

    using Char = CharT;
    using BufferRaw = std::basic_filebuf<Char>;

public:
    using Buffer = boost::iostreams::filtering_streambuf<boost::iostreams::input, Char>;

    static constexpr auto DEFAULT_MODE = std::ios_base::in | std::ios_base::binary;

    BasicFileInputStream()
    {
        Base::init(&m_impl->buffer);
    }

    BasicFileInputStream(BasicFileInputStream&& other) :
        Base{ std::move(other) },
        m_impl{ std::move(other.m_impl) }
    {
        other.init(nullptr);
        Base::set_rdbuf(&m_impl->buffer);
    }

    explicit BasicFileInputStream(const char* path, std::ios_base::openmode mode = DEFAULT_MODE) :
        BasicFileInputStream{}
    {
        open(path, mode);
    }

    explicit BasicFileInputStream(const std::string& path, std::ios_base::openmode mode = DEFAULT_MODE) :
        BasicFileInputStream{ path.c_str(), mode }
    {}

    BasicFileInputStream(const BasicFileInputStream&) = delete;

    auto operator=(const BasicFileInputStream&) -> BasicFileInputStream& = delete;

    auto operator=(BasicFileInputStream&& other) -> BasicFileInputStream&
    {
        Base::operator =(std::move(other));
        m_impl = std::move(other.m_impl);
        Base::set_rdbuf(&m_impl->buffer);
        other.init(nullptr);
        return *this;
    }

    auto rdbuf() const -> const Buffer*
    {
        if (!m_impl)
            return nullptr;

        return &m_impl->buffer;
    }

    auto is_open() const -> bool
    {
        return m_impl && m_impl->bufferRaw.is_open();
    }

    auto open(const char* path, std::ios_base::openmode mode = DEFAULT_MODE) -> void
    {
        if (!m_impl)
        {
            m_impl = std::make_unique<Impl>();
            Base::set_rdbuf(&m_impl->buffer);
        }

        if (!m_impl->bufferRaw.open(path, mode | DEFAULT_MODE))
        {
            Base::setstate(std::ios_base::failbit);
            return;
        }

        m_impl->buffer.push(boost::iostreams::gzip_decompressor{});
        m_impl->buffer.push(m_impl->bufferRaw);

        Base::clear();
    }

    auto open(const std::string& path, std::ios_base::openmode mode = DEFAULT_MODE) -> void
    {
        open(path.c_str(), mode);
    }

    auto close() -> void
    {
        if (!m_impl)
            return;

        if (!m_impl->bufferRaw.close())
            Base::setstate(std::ios_base::failbit);

        m_impl->buffer.reset();
    }

private:
    struct Impl
    {
        BufferRaw bufferRaw;
        Buffer buffer;
    };

    std::unique_ptr<Impl> m_impl = std::make_unique<Impl>();
};

using FileInputStream = BasicFileInputStream<char>;

} // namespace gzip
} // namespace xtras

#endif // XTRAS_GZIP_BASICFILEINPUTSTREAM_HPP_
