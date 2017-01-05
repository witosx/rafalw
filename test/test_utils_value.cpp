#include <rafalw/utils/Value.hpp>
#include <rafalw/utils/Value_math.hpp>
#include <rafalw/utils/demangle.hpp>
#include <iostream>

auto test_utils_value() -> void
{
	{
	    struct Unit1 {};
	    struct Unit2 {};

	    using Value1 = utils::Value<units::make<Unit1>, int>;
	    using Value2 = utils::Value<units::make<Unit2>, int>;

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

        auto v2b = Value2{ 2 };

        std::cout << v2b.quantity() << "\n";

        v2b += v2a;

        std::cout << v2b.quantity() << "\n";

        std::cout << utils::demangle<decltype(cbrt(sqrt(v2b)))>() << "\n";
        std::cout << cbrt(sqrt(v2b)).quantity() << "\n";
	}
}
