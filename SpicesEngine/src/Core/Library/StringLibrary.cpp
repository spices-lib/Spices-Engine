/**
* @file StringLibrary.cpp.
* @brief The StringLibrary Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "StringLibrary.h"

#include <string>
#include <locale>
#include <codecvt>

namespace Spices {

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

    bool StringLibrary::StringsEqual(const char* str0, const char* str1)
    {
        return strcmp(str0, str1) == 0;
    }

    std::wstring StringLibrary::CharToWChar(const char* c)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

        return converter.from_bytes(c);
    }

    std::string StringLibrary::WCharToChar(const wchar_t* wc)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

        return converter.to_bytes(wc);
    }
}