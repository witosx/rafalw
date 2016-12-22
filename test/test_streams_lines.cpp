#include <rafalw/streams/Lines.hpp>
//#include <rafalw/utils/GeneratorMod.hpp>
#include <sstream>

auto test_streams_lines() -> void
{
	{
		auto ss = std::stringstream{};
		ss << "line number 1\n";
		ss << "line number 2\n";
		ss << "\n";
		ss << "line number 3";

		for (auto&& line: streams::lines(ss))
			std::cout << "line '" << line << "'\n";

		std::cout << "\n";
	}

	{
		auto ss = std::stringstream{};
		ss << "line number 1\n";
		ss << "line number 2\n";
		ss << "line number 3\n";

		for (auto&& line: streams::lines(ss))
			std::cout << "line '" << line << "'\n";

		std::cout << "\n";
	}

//	{
//		auto ss = std::stringstream{};
//		ss << "line number 1\n";
//		ss << "line number 2\n";
//		ss << "line number 3\n";
//
//
//		auto g = streams::lines(ss);
//
//		auto t = utils::gtransform([](const auto& e) {
//			return 0;
//		});
//
//		auto g2 = g | t;
//
//		for (auto&& e: g2)
//			std::cout << e << "\n";
//
//	}


}
