#pragma once
#include "Core/Core.h"

namespace Spiecs {

	class Texture2D;
	class Texture2DCube;


	class TextureLoader
	{
	public:
		static void Load(const std::string& filePath, Texture2D* outTexture);
		static void Load(const std::string& filePath, Texture2DCube* outTexture);
	};
}