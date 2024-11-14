#include "Pchheader.h"
#include "GltfLoader.h"

#include "Object/GltfAccessors.h"
#include "Object/GltfAnimations.h"
#include "Object/GltfAsset.h"
#include "Object/GltfBuffers.h"
#include "Object/GltfBufferViews.h"
#include "Object/GltfCameras.h"
#include "Object/GltfImages.h"
#include "Object/GltfLights.h"
#include "Object/GltfMaterials.h"
#include "Object/GltfMeshes.h"
#include "Object/GltfNodes.h"
#include "Object/GltfSamplers.h"
#include "Object/GltfScene.h"
#include "Object/GltfScenes.h"
#include "Object/GltfTextures.h"

#include <nlohmann/json.hpp>

namespace Spices {

	using Json = nlohmann::json;

	bool GltfLoader::Load(const std::string& fileName)
	{
		SPICES_PROFILE_ZONE;

		Json data;
		std::ifstream f(fileName);
		f >> data;

		GltfAsset asset(data["asset"]);
		GltfAccessors accessors(data["accessors"]);
		GltfBuffers buffers(data["buffers"]);
		GltfBufferViews bufferViews(data["bufferViews"]);
		GltfImages images(data["images"]);
		GltfMaterials materials(data["materials"]);
		GltfMeshes meshes(data["meshes"]);
		GltfNodes nodes(data["nodes"]);
		GltfSamplers samplers(data["samplers"]);
		GltfScene scene(data["scene"]);
		GltfScenes scenes(data["scenes"]);
		GltfTextures textures(data["textures"]);
		

		return true;
	}
}