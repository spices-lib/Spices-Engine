/**
* @file TransformComponent.cpp.
* @brief The TransformComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "TransformComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Spiecs {

	void TransformComponent::OnSerialize()
	{

	}

	void TransformComponent::OnDeSerialize()
	{

	}

	void TransformComponent::DefineStageIcon()
	{
		m_SlateStageIconPath = "slate/stage/Stage.Light.png";
	}

	void TransformComponent::CalMatrix()
	{
		/**
		* @brief Use raidans
		*/
		glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));
		m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
	}
}