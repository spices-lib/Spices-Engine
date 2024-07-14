/**
* @file ContainerLibrary.h.
* @brief The ContainerLibrary Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief Container Static Function Library.
	*/
	class ContainerLibrary
	{
	public:

		/**
		* @brief Template Function.
		* Append a Container to another.
		* @param[in out] dst The Container we want to Append to.
		* @param[in] src The Container we want to Append from.
		*/
		template<typename T>
		static void Append(std::vector<T>& dst, const std::vector<T>& src);
	};

	template<typename T>
	void ContainerLibrary::Append(std::vector<T>& dst, const std::vector<T>& src)
	{
		for (int i = 0; i < src.size(); i++)
		{
			dst.push_back(src[i]);
		}
	}
}