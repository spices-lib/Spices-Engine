#pragma once
#include "Core/Core.h"
#include "Render/Resources/Mesh.h"

#include <vector>
#include <memory>

namespace Spiecs {

	class World;

	class FrameInfo
	{
	public:
		FrameInfo() {};

		FrameInfo(const FrameInfo&) = delete;
		FrameInfo& operator=(const FrameInfo&) = delete;

		static FrameInfo& Get();

	public:
		uint32_t m_FrameIndex = 0;
		uint32_t m_Imageindex = 0;

		std::shared_ptr<World> m_World;
	};
}