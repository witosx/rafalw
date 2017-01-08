#include <rafalw/streams/Lines.hpp>
#include <rafalw/generator/Transform.hpp>
#include <rafalw/generator/Filter.hpp>
#include <rafalw/utils/demangle.hpp>
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
		ss << "\n";
		ss << "line number 3";

		auto g = streams::lines(ss);

		while (!done(g))
		{
			std::cout << "line '" << peek(g) << "'\n";
			update(g);
		}

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

	{
		auto ss = std::stringstream{};
		ss << "foo bar\n";
		ss << "foo\n";
		ss << "foo bar\n";
		ss << "foo bar baz\n";

		const auto f = [](const auto& e) {
			return e.length();
		};

		auto r1 = streams::lines(ss);
		auto r2 = r1 % f;

		for (auto&& e: r2)
			std::cout << e << "\n";

		std::cout << "\n";
	}

	{
		auto ss = std::stringstream{};
		ss << "foo bar\n";
		ss << "foo\n";
		ss << "foo bar\n";
		ss << "foo bar baz\n";

		const auto f = [](const auto& e) {
			return e.length() > 5;
		};

		for (auto&& e: streams::lines(ss) | f)
			std::cout << e << "\n";

		std::cout << "\n";
	}

}
