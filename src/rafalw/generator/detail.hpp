#ifndef RAFALW_GENERATOR_DETAIL_HPP_
#define RAFALW_GENERATOR_DETAIL_HPP_

#include <experimental/optional>

inline namespace rafalw {
namespace generator {
namespace detail {

template<typename T>
class ResultStore
{
public:
    auto present() const -> bool
    {
        return static_cast<bool>(m_data);
    }

    auto reset() -> void
    {
        m_data = {};
    }

    auto set(T&& v) -> void
    {
        m_data.emplace(std::move(v));
    }

    auto get() const -> const T&
    {
        return *m_data;
    }

private:
    std::experimental::optional<T> m_data;
};

template<typename T>
class ResultStore<T&>
{
public:
    auto present() const -> bool
    {
        return static_cast<bool>(m_data);
    }

    auto reset() -> void
    {
        m_data = {};
    }

    auto set(T& v) -> void
    {
        m_data = &v;
    }

    auto get() const -> T&
    {
        return *m_data;
    }

private:
    T* m_data{ nullptr };
};

} // namespace detail
} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_DETAIL_HPP_
