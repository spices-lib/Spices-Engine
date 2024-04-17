/**
* @file StringLibrary.h
* @brief The StringLibrary Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include <vector>
#include <string>

namespace Spiecs {

	/**
	* @brief String Static Function Library.
	*/
	class StringLibrary
	{
	public:

		/**
		* @brief Split a string to a string vector container use a char.
		* @param[in] input The string we want to split.
		* @param[in] delimiter The char we want to use.
		* @return Returns a string vector container.
		*/
		static std::vector<std::string> SplitString(const std::string& input, char delimiter);

		/**
		* @brief Determine if the strings given are equal.
		* Platform Specific.
		* @param[in] str0.
		* @param[in] str1.
		* @return Returns true if equal.
		* @todo Platform Implementation.
		*/
		static bool StringsEqual(const char* str0, const char* str1);
	};
}