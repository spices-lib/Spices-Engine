/**
* @file SpriteComponent.cpp.
* @brief The SpriteComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SpriteComponent.h"

namespace Spices {

	SpriteComponent::SpriteComponent(const std::string& path)
	{
		std::shared_ptr<SquarePack> pack = std::make_shared<SquarePack>(2, 2);
		pack->SetMaterial(path);

		// spritecomponent only take one meshpack
		m_Mesh = Mesh::Builder().AddPack(pack).Build();
	}
}