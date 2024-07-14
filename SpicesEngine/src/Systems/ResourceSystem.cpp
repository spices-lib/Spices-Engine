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

	void ResourceSystem::OnSystemInitialize()
	{
		/**
		* @brief Create Default Resource in different ResourccePool.
		*/
		ResourcePool<Texture>::Load<Texture2D>("default.jpg");
	}

	void ResourceSystem::OnSystemShutDown()
	{
		/**
		* @brief Release all Resources
		*/
		ResourcePool<Texture>  ::Destroy();
		ResourcePool<Material> ::Destroy();
		ResourcePool<Mesh>     ::Destroy();
	}

	void ResourceSystem::OnSystemUpdate(TimeStep& ts)
	{
	}

	void ResourceSystem::OnEvent(Event& event)
	{
	}

}