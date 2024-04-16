#pragma once
#include "Core/Core.h"
#include "World/Components/Component.h"
#include "Resources/Mesh/Mesh.h"

namespace Spiecs {

	class SpriteComponent : public Component
	{
	public:
		SpriteComponent();
		virtual ~SpriteComponent() {};

		virtual void OnSerialize() override {};
		virtual void OnDeSerialize() override {};

	private:
		std::shared_ptr<Mesh> m_Mesh;
	};
}