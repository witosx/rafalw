#include <rafalw/value.hpp>
#include <rafalw/utils/demangle.hpp>
#include <iostream>

auto test_value() -> void
{
	{
	    struct Unit1 : public units::Tag {};
	    struct Unit2 : public units::Tag {};

	    using Value1 = value::Value<Unit1, int>;
	    using Value2 = value::Value<Unit2, int>;

	    const auto v1a = Value1{ 10 };
	    const auto v1b = Value1{ 10 };
	    const auto v2a = Value2{ 10 };

	    std::cout << utils::demangle<decltype(v1a)>() << "\n";
	    std::cout << utils::demangle<decltype(v1b)>() << "\n";
	    std::cout << utils::demangle<decltype(v2a)>() << "\n";

	    std::cout << utils::demangle<decltype(v1a + v1b)>() << "\n";
        std::cout << utils::demangle<decltype(v1a - v1b)>() << "\n";
        std::cout << utils::demangle<decltype(v1a * 10)>() << "\n";
        std::cout << utils::demangle<decltype(10 * v1a)>() << "\n";
        std::cout << utils::demangle<decltype(v1a * v1b)>() << "\n";
        std::cout << utils::demangle<decltype(v1a * v2a)>() << "\n";
        std::cout << utils::demangle<decltype(v1a / v1b)>() << "\n";

        auto vx = v1a / v1b;

        std::cout << utils::demangle<decltype(vx)>() << "\n";

        auto v2b = Value2{ 2 };

        std::cout << v2b.quantity() << "\n";

        v2b += v2a;

        std::cout << v2b.quantity() << "\n";

        std::cout << utils::demangle<decltype(cbrt(sqrt(v2b)))>() << "\n";
        std::cout << quantity(cbrt(sqrt(v2b))) << "\n";
	}
}
