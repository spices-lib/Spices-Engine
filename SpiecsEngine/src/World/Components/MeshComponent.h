#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Render/Resources/Mesh.h"

#include <memory>

namespace Spiecs {

	class MeshComponent : public Component
	{
	public:
		MeshComponent() {};
		virtual ~MeshComponent() {};

		virtual void OnSerialize() override;
		virtual void OnDeSerialize() override;

		void SetMesh(std::shared_ptr<Mesh> mesh) { m_Mesh = mesh; };
		std::shared_ptr<Mesh> GetMesh() { return m_Mesh; };

	private:
		std::shared_ptr<Mesh> m_Mesh;
	};
}