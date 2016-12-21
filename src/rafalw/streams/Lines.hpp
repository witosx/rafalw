#ifndef RAFALW_STREAMS_LINES_HPP_
#define RAFALW_STREAMS_LINES_HPP_

#include <rafalw/utils/Generator.hpp>
#include <string>
#include <istream>

inline namespace rafalw {
namespace streams {

class Lines : public utils::Generator<Lines, std::string>
{
public:
    Lines(std::istream& stream) :
        m_stream{ stream }
    {
        update();
    }

    auto peek() const -> const std::string&
    {
        return m_line;
    }

    auto update() -> void
    {
        std::getline(m_stream, m_line);
    }

    auto done() const -> bool
    {
        return !m_stream;
    }

private:
    std::istream& m_stream;
    std::string m_line;
};

} // namespace streams
} // namespace rafalw

#endif // RAFALW_STREAMS_LINES_HPP_
