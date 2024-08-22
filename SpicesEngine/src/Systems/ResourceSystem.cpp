/**
* @file ResourceSystem.cpp.
* @brief The ResourceSystem Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ResourceSystem.h"
#include "Resources/ResourcePool/ResourcePool.h"

#include "Resources/Texture/Texture.h"
#include "Resources/Material/Material.h"
#include "Resources/Mesh/Mesh.h"

namespace Spices {

	std::vector<std::string> ResourceSystem::m_ResourceSearchFolder = { SPICES_ENGINE_ASSETS_PATH };

	void ResourceSystem::OnSystemInitialize()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create Default Resource in different ResourccePool.
		*/
		ResourcePool<Texture>::Load<Texture2D>("default.jpg", "default.jpg");

		//ResourcePool<MeshPack>::Load<SpherePack>("Sphere")->OnCreatePack();
		ResourcePool<MeshPack>::Load<SquarePack>("Square")->OnCreatePack();
		ResourcePool<MeshPack>::Load<BoxPack>("Box")->OnCreatePack();
	}

	void ResourceSystem::OnSystemShutDown()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Release all Resources
		*/
		ResourcePool<Texture>  ::Destroy();
		ResourcePool<Material> ::Destroy();
		ResourcePool<MeshPack> ::Destroy();
	}

	void ResourceSystem::OnSystemUpdate(TimeStep& ts)
	{
	}

	void ResourceSystem::OnEvent(Event& event)
	{
	}

	void ResourceSystem::RegistryResourceFolder(const std::string& folder)
	{
		m_ResourceSearchFolder.push_back(folder);
	}

}