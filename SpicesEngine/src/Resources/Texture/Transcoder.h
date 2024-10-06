#pragma once
#include "Core/Core.h"
#include <ktxvulkan.h>

namespace Spices {

	class Transcoder
	{
	public:

		Transcoder() = default;
		virtual ~Transcoder() = default;

		static void Init();

		static bool SaveSrcToKTX(const std::string& filePath, const unsigned char* data, int width, int height);

		static bool LoadFromKTX(const std::string& filePath, ktxVulkanTexture* vkTexture);

	private:

	};

}