/**
* @file SkyBoxComponent.cpp.
* @brief The SkyBoxComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SkyBoxComponent.h"
#include "Render/FrameInfo.h"
#include "World/Entity.h"

namespace Spices {

	SkyBoxComponent::SkyBoxComponent()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Use SpherePack Instead
		*/
		std::shared_ptr<CubePack> pack = std::make_shared<CubePack>(2, 2);
		//pack->SetMaterial(path);

		m_Mesh = Mesh::Builder().AddPack(pack).Build();
	}

	void SkyBoxComponent::OnSerialize()
	{

	}

	void SkyBoxComponent::OnDeSerialize()
	{

	}

	void SkyBoxComponent::DrawThis()
	{
		SPICES_PROFILE_ZONE;

		MeshComponent::DrawThis();
	}

	void SkyBoxComponent::SetMaterial(const std::string& materialPath)
	{
		SPICES_PROFILE_ZONE;

		auto& e = FrameInfo::Get().m_World->QueryEntitybyID((uint32_t)m_Owner);

		m_Mesh->GetPacks().for_each([&](const auto& k, const auto& v) {
			v->SetMaterial(materialPath);
			v->GetMeshDesc().UpdatemodelAddress(e.GetComponent<TransformComponent>().GetModelBufferAddress());
			v->GetMeshDesc().UpdateentityID((uint32_t)m_Owner);

			return false;
		});
	}
}
