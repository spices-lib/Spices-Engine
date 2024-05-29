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

	void PointLightComponent::DefineStageIcon()
	{
		m_SlateStageIconPath = "slate/stage/Stage.Light.png";
	}
}