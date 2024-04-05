#pragma once
#include "Core/Core.h"
#include "World/Components/Component.h"
#include "Render/Resources/Mesh.h"

namespace Spiecs {

	class SpriteComponent : public Component
	{
	public:
		SpriteComponent();
		virtual ~SpriteComponent() {};

		virtual void OnSerialize() override {};
		virtual void OnDeSerialize() override {};
		virtual void OnComponentAdded() override {};

	private:
		std::shared_ptr<Mesh> m_Mesh;
	};
}