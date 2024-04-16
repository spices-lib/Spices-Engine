#pragma once
#include "Core/Core.h"
#include "Material.h"

namespace Spiecs {

	class MeshMaterial : public Material
	{
	public:
		MeshMaterial() {};
		MeshMaterial(const std::string& materialPath) : Material(materialPath) {};
		virtual ~MeshMaterial() {};

	private:
		virtual void BuildMaterial() override;
	};
}