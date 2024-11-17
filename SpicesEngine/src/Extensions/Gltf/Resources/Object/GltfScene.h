#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfScene : public GltfObject
	{
	public:

		GltfScene(const Json data) 
			: GltfObject(data) 
		{ 
			scene = data.get<int>();
		}

		virtual ~GltfScene() override = default;

	private:

		uint32_t scene;
	};
}
