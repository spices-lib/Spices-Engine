/**
* @file SpriteComponent.cpp.
* @brief The SpriteComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SpriteComponent.h"
#include "Resources/Material/SpriteMaterial.h"

namespace Spiecs {

	SpriteComponent::SpriteComponent(const std::string& path)
	{
		std::shared_ptr<SquarePack> pack = std::make_shared<SquarePack>(2, 2);
		pack->SetMaterial<SpriteMaterial>(path);

		// spritecomponent only take one meshpack
		m_Mesh = Mesh::Builder().AddPack(pack).Build();
	}

}