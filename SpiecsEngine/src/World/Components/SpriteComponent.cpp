/**
* @file SpriteComponent.cpp.
* @brief The SpriteComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SpriteComponent.h"

namespace Spiecs {

	SpriteComponent::SpriteComponent()
	{
		std::shared_ptr<SquarePack> pack = std::make_shared<SquarePack>();
		pack->OnCreatePack();

		// spritecomponent only take one meshpack
		m_Mesh = Mesh::Builder().AddPack(pack).Build();
	}

}