/**
* @file ClassLibrary.h.
* @brief The ClassLibrary Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"

namespace Spiecs {

	/**
	* @brief Using ClassType instead std::reference_wrapper<const std::type_info>.
	*/
	using ClassType = std::reference_wrapper<const std::type_info>;

	/**
	* @brief Class Static Function Library.
	*/
	class ClassLibrary
	{
	public:

		/**
		* @brief Get Class Name as string.
		* @param[in] t ClassType.
		* @return Returns Class Name.
		*/
		static std::string GetClassString(ClassType t);
	};
}