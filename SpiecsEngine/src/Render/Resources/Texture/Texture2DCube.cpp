#include "Pchheader.h"
#include "Texture2DCube.h"

namespace Spiecs {

	Texture2DCube::Texture2DCube(const std::string& path)
	{
		m_ResourcePath = path;
		TextureLoader::Load(m_ResourcePath, this);
	}
}