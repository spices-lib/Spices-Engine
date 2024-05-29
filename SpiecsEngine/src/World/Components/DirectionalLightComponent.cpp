/**
* @file CameraComponent.cpp.
* @brief The CameraComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "DirectionalLightComponent.h"

namespace Spiecs {

	void DirectionalLightComponent::OnSerialize()
	{
	}
	void DirectionalLightComponent::OnDeSerialize()
	{
	}

	void DirectionalLightComponent::DefineStageIcon()
	{
		m_SlateStageIconPath = "slate/stage/Stage.Light.png";
	}
}