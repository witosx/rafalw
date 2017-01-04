#include <rafalw/units.hpp>
#include <rafalw/utils/demangle.hpp>
#include <iostream>

struct PriceUnitPointsTag {};
struct PriceUnitCashTag {};

auto test_units() -> void
{
	{
		using PriceUnitPoints = units::make<PriceUnitPointsTag>;
		using PriceUnitCash = units::make<PriceUnitCashTag>;

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
