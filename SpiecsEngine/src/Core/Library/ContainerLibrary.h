#pragma once
#include "Core/Core.h"

namespace Spiecs {

	class ContainerLibrary
	{
	public:

		template<typename T>
		static void Resize(std::vector<T>& vector, uint32_t newSize)
		{
			uint32_t size = vector.size();

			if (size < newSize)
			{
				vector.resize(newSize);
			}
		};

	};
}