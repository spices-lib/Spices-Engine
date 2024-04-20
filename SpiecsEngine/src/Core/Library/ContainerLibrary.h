/**
* @file ContainerLibrary.h.
* @brief The ContainerLibrary Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"

namespace Spiecs {

	/**
	* @brief Container Static Function Library.
	*/
	class ContainerLibrary
	{
	public:

		/**
		* @brief Template Function.
		* Expand Our Container to a Specific Size if Container's size is smaller than it.
		* @param[in out] vector The Container we want to Expand size.
		* @param[in] newSize The size we want to Specific.
		*/
		template<typename T>
		static void Resize(std::vector<T>& vector, uint32_t newSize);

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
	inline void ContainerLibrary::Resize(std::vector<T>& vector, uint32_t newSize)
	{
		uint32_t size = (uint32_t)vector.size();

		if (size < newSize)
		{
			vector.resize(newSize);
		}
	}

	template<typename T>
	inline void ContainerLibrary::Append(std::vector<T>& dst, const std::vector<T>& src)
	{
		for (int i = 0; i < src.size(); i++)
		{
			dst.push_back(src[i]);
		}
	}
}