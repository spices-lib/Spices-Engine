#pragma once
#include "Core/Core.h"

namespace Spiecs {

	class ContainerLibrary
	{
	public:

		template<typename T>
		static void Resize(std::vector<T>& vector, uint32_t newSize);

		template<typename T>
		static void Append(std::vector<T>& dst, const std::vector<T>& src);
	};

	template<typename T>
	inline void ContainerLibrary::Resize(std::vector<T>& vector, uint32_t newSize)
	{
		uint32_t size = vector.size();

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