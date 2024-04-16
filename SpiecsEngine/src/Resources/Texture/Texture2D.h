#pragma once
#include "Core/Core.h"
#include "Texture.h"

namespace Spiecs {

	class Texture2D : public Texture
	{
	public:
		Texture2D() {};
		Texture2D(const std::string& path);
		virtual ~Texture2D() {};

	private:
		friend class TextureLoader;
	};
}