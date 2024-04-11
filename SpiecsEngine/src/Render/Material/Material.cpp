#include "Pchheader.h"
#include "Material.h"

namespace Spiecs {

	void Material::LoadMaterial(const std::string& materialPath)
	{
		m_MaterialPath = materialPath;

		MaterialLoader::Load(materialPath, this);
		BuildMaterial();
	}

	void Material::Reload()
	{
		if (m_MaterialPath.empty())
		{
			SPIECS_LOG("Please do not do that!");
		}

		MaterialLoader::Load(m_MaterialPath, this);
		BuildMaterial();
	}
}