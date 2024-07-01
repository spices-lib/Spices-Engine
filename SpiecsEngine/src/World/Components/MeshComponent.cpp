/**
* @file MeshComponent.cpp.
* @brief The MeshComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "MeshComponent.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"

namespace Spiecs {

	void MeshComponent::OnSerialize()
	{

	}

	void MeshComponent::OnDeSerialize()
	{

	}

	void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Set m_Mesh.
		*/
		m_Mesh = mesh;

		/**
		* @brief Mark World with MeshAddedToWorld bits.
		*/
		FrameInfo::Get().m_World->Mark(WorldMarkBits::MeshAddedToWorld);
	}
}