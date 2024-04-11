#pragma once
#include "Core/Core.h"
#include "Material.h"

#include <optional>

namespace Spiecs {

	class MeshMaterial : public Material
	{
	public:
		MeshMaterial() {};
		virtual ~MeshMaterial() {};

	private:
		virtual void BuildMaterial() override;
	};
}