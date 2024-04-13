#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Render/Resources/Mesh.h"

#include <memory>

namespace Spiecs {

	class SkyBoxComponent : public Component
	{
	public:
		SkyBoxComponent() {};
		SkyBoxComponent(const std::string& path);
		virtual ~SkyBoxComponent() {};

		virtual void OnSerialize() override;
		virtual void OnDeSerialize() override;

		std::shared_ptr<Mesh> GetMesh() { return m_Mesh; };

	private:
		std::shared_ptr<Mesh> m_Mesh;
	};
}