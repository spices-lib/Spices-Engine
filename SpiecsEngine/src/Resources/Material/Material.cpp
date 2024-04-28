/**
* @file Material.cpp.
* @brief The Material Class Implementation.
* @author Spiecs.
*/

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
			SPIECS_CORE_INFO("Please do not do that!");
		}
	}

	void Material::Dserialize()
	{
		if (m_MaterialPath.empty())
		{
			SPIECS_CORE_INFO("Please do not do that!");
		}

		MaterialLoader::Load(m_MaterialPath, this);
	}
}