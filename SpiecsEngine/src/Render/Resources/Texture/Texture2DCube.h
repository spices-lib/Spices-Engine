#pragma once
#include "Core/Core.h"
#include "Texture2D.h"

namespace Spiecs {

	class Texture2DCube : public Texture2D
	{
	public:
		Texture2DCube() {};
		Texture2DCube(const std::string& path);
		virtual ~Texture2DCube() {};
	};
}