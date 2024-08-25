/**
* @file SpriteComponent.cpp.
* @brief The SpriteComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SpriteComponent.h"
#include "Render/FrameInfo.h"
#include "World/Entity.h"

namespace Spices {

	SpriteComponent::SpriteComponent()
	{
		SPICES_PROFILE_ZONE;

		std::shared_ptr<PlanePack> pack = std::make_shared<PlanePack>(2, 2);
		//pack->SetMaterial(path);

		// spritecomponent only take one meshpack
		m_Mesh = Mesh::Builder().AddPack(pack).Build();
	}

	void SpriteComponent::SetMaterial(const std::string& materialPath)
	{
		SPICES_PROFILE_ZONE;

		auto& e = FrameInfo::Get().m_World->QueryEntitybyID((uint32_t)m_Owner);

		m_Mesh->GetPacks().for_each([&](auto& k, auto& v) {
			v->SetMaterial(materialPath);
			v->GetMeshDesc().UpdatemodelAddress(e.GetComponent<TransformComponent>().GetModelBufferAddress());
			v->GetMeshDesc().UpdateentityID((uint32_t)m_Owner);
			return false;
		});
	}
}