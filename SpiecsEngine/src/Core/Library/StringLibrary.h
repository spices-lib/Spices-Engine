#pragma once
#include "Core/Core.h"
#include <vector>
#include <string>

namespace Spiecs {

	class StringLibrary
	{
	public:
		static std::vector<std::string> SplitString(const std::string& input, char delimiter);
	};
}