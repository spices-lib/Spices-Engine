#pragma once
#include "Core/Core.h"
#include "Material.h"

namespace Spiecs {

	class SkyBoxMaterial : public Material
	{
	public:
		SkyBoxMaterial() {};
		SkyBoxMaterial(const std::string& materialPath) : Material(materialPath) {};
		virtual ~SkyBoxMaterial() {};

	private:
		virtual void BuildMaterial() override;
	};

}