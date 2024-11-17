#pragma once
#include "Core/Core.h"

#include "Resources/Object/GltfAccessors.h"
#include "Resources/Object/GltfAnimations.h"
#include "Resources/Object/GltfAsset.h"
#include "Resources/Object/GltfBuffers.h"
#include "Resources/Object/GltfBufferViews.h"
#include "Resources/Object/GltfCameras.h"
#include "Resources/Object/GltfImages.h"
#include "Resources/Object/GltfLights.h"
#include "Resources/Object/GltfMaterials.h"
#include "Resources/Object/GltfMeshes.h"
#include "Resources/Object/GltfNodes.h"
#include "Resources/Object/GltfSamplers.h"
#include "Resources/Object/GltfScene.h"
#include "Resources/Object/GltfScenes.h"
#include "Resources/Object/GltfSkins.h"
#include "Resources/Object/GltfTextures.h"

namespace Spices {

	class GltfLoader;
	class World;
	class GltfPack;

	class GltfCollection
	{
	public:
		GltfCollection(const std::string& path);
		virtual ~GltfCollection() = default;

		void CreateEntity(World* world, const std::string& tag);


	private:

		void CreateEntityRecursive(World* world, const std::string& tag, uint32_t node, const glm::mat4& model);

	private:

		std::unique_ptr<GltfAccessors  >   m_Accessors;
		std::unique_ptr<GltfAnimations >   m_Animations;
		std::unique_ptr<GltfAsset      >   m_Asset;
		std::unique_ptr<GltfBuffers    >   m_Buffers;
		std::unique_ptr<GltfBufferViews>   m_BufferViews;
		std::unique_ptr<GltfCameras    >   m_Cameras;
		std::unique_ptr<GltfImages     >   m_Images;
		std::unique_ptr<GltfLights     >   m_Lights;
		std::unique_ptr<GltfMaterials  >   m_Materials;
		std::unique_ptr<GltfMeshes     >   m_Meshes;
		std::unique_ptr<GltfNodes      >   m_Nodes;
		std::unique_ptr<GltfSamplers   >   m_Samplers;
		std::unique_ptr<GltfScene      >   m_Scene;
		std::unique_ptr<GltfScenes     >   m_Scenes;
		std::unique_ptr<GltfSkins      >   m_Skins;
		std::unique_ptr<GltfTextures   >   m_Textures;

		friend class GltfLoader;
	};
}