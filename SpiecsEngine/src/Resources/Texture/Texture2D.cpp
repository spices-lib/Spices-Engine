/**
* @file Texture2D.cpp.
* @brief The Texture2D Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Texture2D.h"

namespace Spiecs {

	Texture2D::Texture2D(const std::string& path)
	{
		m_ResourcePath = path;
		TextureLoader::Load(m_ResourcePath, this);
	}
}