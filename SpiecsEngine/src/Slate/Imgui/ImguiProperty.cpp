/**
* @file ImguiProperty.cpp.
* @brief The ImguiProperty Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiProperty.h"

namespace Spiecs {

	void ImguiProperty::OnRender()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Begin render Property.
		*/
		Begin();

		/**
		* @brief End render Property.
		*/
		End();
	}
}