#pragma once


#include <fmt/core.h>

namespace FNetShark
{
	namespace Utils
	{
#define WINSOCKET_CHECK(x)\
if(0 != x) {\
	fmt::print("A winsocket related function failed in line:: {}\n File: {}\n", __LINE__, __FILE__);\
	throw "Serious failure occured!\n";\
}
	}
}