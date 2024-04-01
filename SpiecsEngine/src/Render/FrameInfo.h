#pragma once
#include "Core/Core.h"
#include "Render/Resources/Mesh.h"

#include <vector>

namespace Spiecs {

	class FrameInfo
	{
	public:

		uint32_t m_FrameIndex = 0;
		uint32_t m_Imageindex = 0;

		std::vector<std::shared_ptr<Mesh>> m_Meshes;
	};
}