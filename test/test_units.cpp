#include <rafalw/units.hpp>
#include <rafalw/utils/demangle.hpp>
#include <iostream>

struct PriceUnitPoints : public units::Tag {};
struct PriceUnitCash : public units::Tag {};

auto test_units() -> void
{
	{
		std::cout << utils::demangle<PriceUnitCash>() << "\n";
		std::cout << utils::demangle<PriceUnitPoints>() << "\n";

		using PriceUnitCashPerPoint = units::div<PriceUnitCash, PriceUnitPoints>;

		std::cout << utils::demangle<PriceUnitCashPerPoint>() << "\n";

		using Ux = units::mul<PriceUnitPoints, PriceUnitCashPerPoint>;

		std::cout << utils::demangle<Ux>() << "\n";

		using Uy = units::div<Ux, PriceUnitCash>;

		std::cout << utils::demangle<Uy>() << "\n";

		std::cout << utils::demangle<units::to_list<PriceUnitCashPerPoint>>() << "\n";
		std::cout << utils::demangle<units::to_map<PriceUnitCashPerPoint>>() << "\n";
		std::cout << utils::demangle<units::pow<PriceUnitCashPerPoint, std::ratio<1, 2>>>() << "\n";
	}
}
