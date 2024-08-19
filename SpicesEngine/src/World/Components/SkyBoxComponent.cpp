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

	SkyBoxComponent::SkyBoxComponent(const std::string& path)
	{
		SPICES_PROFILE_ZONE;

		auto& e = FrameInfo::Get().m_World->QueryEntitybyID((uint32_t)m_Owner);

		/**
		* @brief Use SpherePack Instead
		*/
		std::shared_ptr<BoxPack> pack = std::make_shared<BoxPack>(2, 2);
		pack->SetMaterial(path);
		m_Mesh = Mesh::Builder().AddPack(pack).Build();

		m_Mesh->GetPacks().for_each([&](auto& k, auto& v) {
			v->GetMeshDesc().UpdatemodelAddress(e.GetComponent<TransformComponent>().GetModelBufferAddress());
			v->GetMeshDesc().UpdateentityID((uint32_t)m_Owner);
			return false;
		});
	}

	void SkyBoxComponent::OnSerialize()
	{
	}

	void SkyBoxComponent::OnDeSerialize()
	{
	}

	void SkyBoxComponent::DrawThis()
	{
		MeshComponent::DrawThis();
	}
}
