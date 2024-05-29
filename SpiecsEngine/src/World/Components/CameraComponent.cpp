/**
* @file CameraComponent.cpp.
* @brief The CameraComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "CameraComponent.h"

namespace Spiecs {

	CameraComponent::CameraComponent(bool isActived)
		: m_IsActived(isActived)
	{}

	void CameraComponent::OnSerialize()
	{

	}

	void CameraComponent::OnDeSerialize()
	{

	}

	const std::string& CameraComponent::DefineStageIcon()
	{
		return "slate/stage/Stage.Light.png";
	}
}