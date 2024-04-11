#include "Pchheader.h"
#include "Material.h"

namespace Spiecs {

	Material::Material(const std::string& materialPath)
		: m_MaterialPath(materialPath)
	{
		Dserialize();
	}

	void Material::Serialize()
	{
		if (m_MaterialPath.empty())
		{
			SPIECS_LOG("Please do not do that!");
		}

		// TODO: 
	}

	void Material::Dserialize()
	{
		if (m_MaterialPath.empty())
		{
			SPIECS_LOG("Please do not do that!");
		}

		MaterialLoader::Load(m_MaterialPath, this);
	}
}