#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfScene : public GltfObject
	{
	public:

		GltfScene(const Json data) 
			: GltfObject(GltfObjectType::scene, data) 
		{
			scene = data["scene"];
		}

		virtual ~GltfScene() override = default;

	private:

		uint32_t scene;
	};
}
