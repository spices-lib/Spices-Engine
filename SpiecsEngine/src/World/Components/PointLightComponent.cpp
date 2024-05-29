/**
* @file PointLightComponent.cpp.
* @brief The PointLightComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "PointLightComponent.h"

namespace Spiecs {
	void PointLightComponent::OnSerialize()
	{
	}
	void PointLightComponent::OnDeSerialize()
	{
	}

	const std::string& PointLightComponent::DefineStageIcon()
	{
		return "slate/stage/Stage.Light.png";
	}
}