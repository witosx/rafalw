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

private:
    friend class utils::GeneratorAccess;

    std::istream& m_stream;
    std::string m_line;

    auto generatorPeek() const -> const std::string&
    {
        return m_line;
    }

    auto generatorUpdate() -> void
    {
        std::getline(m_stream, m_line);
    }

    auto generatorDone() const -> bool
    {
        return !m_stream;
    }
};

} // namespace streams
} // namespace rafalw

#endif // RAFALW_STREAMS_LINES_HPP_
