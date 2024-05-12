#pragma once
#include "Core/Core.h"

namespace Spiecs {

	/**
	* @brief Using Type instead std::reference_wrapper<const std::type_info>.
	*/
	using ClassType = std::reference_wrapper<const std::type_info>;

	class ClassLibrary
	{
	public:
		
		static std::string GetClassString(ClassType t);
	};
}