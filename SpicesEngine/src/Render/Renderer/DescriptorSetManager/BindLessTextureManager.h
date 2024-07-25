#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "Core/Math/Math.h"

namespace Spices {

	class BindLessTextureManager
	{
	public:
		BindLessTextureManager() = default;
		virtual ~BindLessTextureManager() = default;

		static uint32_t Registry(const std::string& name);
		static void UnRegistry(const std::string& name);

	private:

		static std::unordered_map<std::string, uint32_t> m_TextureIDMap;
		static std::unordered_map<uint32_t, std::string> m_TextureInfoMap;

	};
}