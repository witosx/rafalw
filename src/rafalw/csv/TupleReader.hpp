#ifndef RAFALW_CSV_TUPLEREADER_HPP_
#define RAFALW_CSV_TUPLEREADER_HPP_

#include <rafalw/csv/ReaderBase.hpp>
#include <rafalw/utils/static.hpp>
#include <tuple>

inline namespace rafalw {
namespace csv {

template<typename... Args>
class TupleReader : public ReaderBase<TupleReader<Args...>, std::tuple<Args...>>
{
public:
    using Tuple = std::tuple<Args...>;
    using ReaderBase<TupleReader<Args...>, std::tuple<Args...>>::ReaderBase;

    auto processLine(Line&& l) const -> Tuple
    {
        auto res = Tuple{};
        utils::static_foreach(res, [&l](auto& e) {
            l >> e;
        });
        return res;
    }
};

} // namespace csv
} // inline namespace rafalw

#endif // RAFALW_CSV2_TUPLEREADER_HPP_
