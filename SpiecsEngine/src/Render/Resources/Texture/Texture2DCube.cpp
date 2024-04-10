#include "Pchheader.h"
#include "Texture2DCube.h"

namespace Spiecs {

	Texture2DCube::Texture2DCube(const std::string& path)
		: Texture2D(path)
	{
		TextureLoader::Load(m_ResourcePath, this);
	}
}