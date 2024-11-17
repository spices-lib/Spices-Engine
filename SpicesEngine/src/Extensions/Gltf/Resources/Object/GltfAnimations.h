#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfAnimations : public GltfObject
	{
	public:
		GltfAnimations(const Json& data) : GltfObject(data) {}
		virtual ~GltfAnimations() override = default;
	};
}
