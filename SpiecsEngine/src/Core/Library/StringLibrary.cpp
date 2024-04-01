#include "pchheader.h"
#include "StringLibrary.h"

namespace Spiecs {

	std::vector<std::string> StringLibrary::SplitString(const std::string& input, char delimiter)
	{
        std::vector<std::string> result;
        size_t start = 0;
        size_t end = 0;

        while ((end = input.find(delimiter, start)) != std::string::npos) {
            result.push_back(input.substr(start, end - start));
            start = end + 1;
        }

        result.push_back(input.substr(start));

        return result;
	}
}