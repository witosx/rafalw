#include <rafalw/utils/StringWrapper.hpp>
#include <iostream>

class MyObjectID : public utils::StringWrapper<MyObjectID, std::string>
{
public:
	using StringWrapper::StringWrapper;
};

auto test_utils_stringwrapper() -> void
{
	{
		auto id1 = MyObjectID{ "bolek" };
		auto id2 = MyObjectID{ "lolek" };

		std::cout << id1 << "\n";
		std::cout << id2 << "\n";

		std::cout << (id1 == id2) << "\n";
	}


}
