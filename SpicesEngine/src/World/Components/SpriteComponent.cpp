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

	SpriteComponent::SpriteComponent(const std::string& path)
	{
		SPICES_PROFILE_ZONE;

		auto& e = FrameInfo::Get().m_World->QueryEntitybyID((uint32_t)m_Owner);

		std::shared_ptr<SquarePack> pack = std::make_shared<SquarePack>(2, 2);
		pack->SetMaterial(path);

		// spritecomponent only take one meshpack
		m_Mesh = Mesh::Builder().AddPack(pack).Build();

		m_Mesh->GetPacks().for_each([&](auto& k, auto& v) {
			v->GetMeshDesc().UpdatemodelAddress(e.GetComponent<TransformComponent>().GetModelBufferAddress());
			v->GetMeshDesc().UpdateentityID((uint32_t)m_Owner);
			return false;
		});
	}
}