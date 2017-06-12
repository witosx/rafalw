#include <rafalw/strings/Tokens.hpp>
#include <iostream>

auto test_strings_tokens() -> void
{
	{
		auto str = std::string{ "ala ma kota" };
		auto tokens = string::Tokens{ str, " " };

		std::cout << "- string '" << str << "'\n";

		for (auto&& token: tokens)
		{
			std::cout << "- token '" << token << "'\n";
		}

		std::cout << "\n";
	}

	{
		auto str = std::string{ "foo,bar,baz," };
		auto tokens = string::Tokens{ str, "," };

		std::cout << "- string '" << str << "'\n";

		for (auto&& token: tokens)
		{
			std::cout << "- token '" << token << "'\n";
		}

		std::cout << "\n";
	}

	{
		auto str = std::string{ "" };
		auto tokens = string::Tokens{ str, " " };

		std::cout << "- string '" << str << "'\n";

		for (auto&& token: tokens)
		{
			std::cout << "- token '" << token << "'\n";
		}

		std::cout << "\n";
	}

	{
		auto str = std::string{ "foo   bar baz," };
		auto tokens = string::Tokens{ str, " " };

		std::cout << "- string '" << str << "'\n";

		for (auto&& token: tokens)
		{
			std::cout << "- token '" << token << "'\n";
		}

		std::cout << "\n";
	}
}
