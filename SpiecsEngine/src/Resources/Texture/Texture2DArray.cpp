/**
* @file Texture2DArray.cpp.
* @brief The Texture2DArray Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Texture2DArray.h"

namespace Spiecs {

	Texture2DArray::Texture2DArray(const RendererResourceCreateInfo& info)
	{

	}

	Texture2DArray::Texture2DArray(const std::string& path)
	{
		m_ResourcePath = path;
		//TextureLoader::Load(m_ResourcePath, this);
	}

}