#pragma once
#include "Core/Core.h"

namespace Spices {

	enum class GltfObjectType
	{
		asset        = 0,
		accessors    = 1,
		animations   = 2,
		buffers      = 3,
		bufferViews  = 4,
		images       = 5,
		materials    = 6,
		meshes       = 7,
		nodes        = 8,
		samplers     = 9,
		scenes       = 10,
		scene        = 11,
		textures     = 12
	};

	class GltfUtils
	{
	public:

		static std::string ToString(GltfObjectType type);
		static GltfObjectType ToType(const std::string& type);
	};

	 

}