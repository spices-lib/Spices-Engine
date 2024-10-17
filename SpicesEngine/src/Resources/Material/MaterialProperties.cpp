/**
* @file MaterialProperties.cpp.
* @brief The MaterialProperties Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "MaterialProperties.h"

namespace Spices {

	MaterialProperties::MaterialProperties()
		: bTwoSided(false)
		, bCastShadow(true)
		, bReceiveShadow(true)
		, bWireFrame(false)
	{}
}