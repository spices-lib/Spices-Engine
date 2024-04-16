#pragma once
#include "Texture2D.h"

namespace Spiecs {

	class Texture2DArray : public Texture2D
	{
	public:
		Texture2DArray() {};
		Texture2DArray(const std::string& path);
		virtual ~Texture2DArray() {};
	};
}