#ifndef RAFALW_UTILS_VALUEHOLDER_HPP_
#define RAFALW_UTILS_VALUEHOLDER_HPP_

#include <rafalw/utils/FunctionPtr.hpp>
#include <rafalw/utils/assert.hpp>
#include <rafalw/math.hpp>
#include <type_traits>
#include <memory>
#include <cstdint>

inline namespace rafalw {
namespace utils {

template<typename T>
struct ValueHolderEmplaceTag {};

template<typename T>
constexpr auto ValueHolderEmplace = ValueHolderEmplaceTag<T>{};

class ValueHolder
{
public:
    ValueHolder() = default;
    ValueHolder(const ValueHolder&) = delete;

    template<typename T, typename... Args>
    ValueHolder(ValueHolderEmplaceTag<T>, Args&&... args)
    {
        emplace<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    ValueHolder(T&& obj)
    {
        *this = std::forward<T>(obj);
    }

    ValueHolder(ValueHolder&& other)
    {
        *this = std::move(other);
    }

    template<typename T>
    auto operator =(T&& obj) -> ValueHolder&
    {
        emplace<std::remove_reference_t<T>>(std::forward<T>(obj));
        return *this;
    }

    auto operator =(const ValueHolder&) -> ValueHolder& = delete;

    auto operator =(ValueHolder&& other) -> ValueHolder&
    {
        m_data = std::exchange(other.m_data, nullptr);
        return *this;
    }

    operator bool () const
    {
        return static_cast<bool>(m_data);
    }

    auto operator !() const -> bool
    {
        return !static_cast<bool>(*this);
    }

    auto pointer() const -> const void*
    {
        rafalw_utils_assert(m_data);
        return m_data->buffer();
    }

    auto pointer() -> void*
    {
        rafalw_utils_assert(m_data);
        return m_data->buffer();
    }

private:
    class Data
    {
    public:
        Data(const Data&) = delete;
        Data(Data&&) = delete;

        virtual ~Data()
        {
            destruct();
        }

        auto operator =(const Data&) -> Data& = delete;
        auto operator =(Data&&) -> Data& = delete;

        template<typename V, typename... Args>
        auto emplace(Args&&... args) -> void
        {
            destruct();

            rafalw_utils_assert(m_bufferSize >= sizeof(V));

            new (m_buffer) V{ std::forward<Args>(args)... };
            m_destructor = [](const void* o){ return static_cast<const V*>(o)->~V(); };
        }

        auto size() const -> std::size_t
        {
            return m_bufferSize;
        }

        auto buffer() const -> const void*
        {
            return m_buffer;
        }

        auto buffer() -> void*
        {
            return m_buffer;
        }

    protected:
        Data() = default;

        auto init(void* buffer, std::size_t buffer_size) -> void
        {
            rafalw_utils_assert(buffer);
            rafalw_utils_assert(buffer_size > 0);
            m_buffer = buffer;
            m_bufferSize = buffer_size;
        }

        auto destruct() -> void
        {
            if (!m_destructor)
                return;

            m_destructor(m_buffer);
            m_destructor = nullptr;
        }

    private:
        utils::FunctionPtr<void(const void*)> m_destructor = nullptr;
        std::size_t m_bufferSize = 0;
        void* m_buffer = nullptr;
    };

    template<std::size_t N>
    class DataImpl final : public Data
    {
    public:
        DataImpl()
        {
            init(m_buffer.data(), N);
        }

    private:
        std::array<std::uint8_t, N> m_buffer;
    };

    std::unique_ptr<Data> m_data;

    template<typename V, typename... Args>
    auto emplace(Args&&... args) -> void
    {
        constexpr auto OBJECT_SIZE = sizeof(V);

        if (m_data && m_data->size() < OBJECT_SIZE)
            m_data.reset();

        if (!m_data)
        {
            constexpr auto CHUNK_SIZE = std::size_t{ 32 };
            constexpr auto BUFFER_SIZE = math::int_round_up(OBJECT_SIZE, CHUNK_SIZE);
            m_data = std::make_unique<DataImpl<BUFFER_SIZE>>();
        }

        m_data->emplace<V>(std::forward<Args>(args)...);
    }
};

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_VALUEHOLDER_HPP_
