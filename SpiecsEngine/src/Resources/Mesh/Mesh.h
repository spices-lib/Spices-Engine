/**
* @file Mesh.h.
* @brief The Mesh Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "MeshPack.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Spiecs {

	/**
	* @brief MeshRenderer Class.
	* This class is a wrapper of mashpack.
	*/
	class Mesh
	{
	public:

		/**
		* @brief Builder Class.
		* This class helps to create a mesh.
		*/
		class Builder
		{
		public:

			/**
			* @brief Constructor Function.
			*/
			Builder() = default;

			/**
			* @brief Destructor Function.
			*/
			virtual ~Builder() = default;

			/**
			* @brief Add pack to mesh.
			* @param[in] meshPack Which pack we want add to.
			* @return Returns this reference.
			*/
			Builder& AddPack(std::shared_ptr<MeshPack> meshPack);

			/**
			* @brief Build a mesh shared pointer.
			* @return Returns the mesh shared pointer.
			*/
			std::shared_ptr<Mesh> Build() const;

		private:

			/**
			* @brief The packs that used for create mesh.
			*/
			std::unordered_map<uint32_t, std::shared_ptr<MeshPack>> m_Pack;

			/**
			* @brief The packs nums.
			*/
			uint32_t m_PackNums = 0;
		};

	public:

		/**
		* @brief Constructor Function.
		* Init member variables.
		* @param[in] meshPacks The meshpack that used for create mesh.
		*/
		Mesh(std::unordered_map<uint32_t, std::shared_ptr<MeshPack>> meshPacks);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Mesh() = default;

		/**
		* @brief Call meshpacks Draw().
		* @param[in] commandBuffer Which command buffer we want submit command.
		* @param[in] func the function pointer used for bind material parameters.
		*/
		template<typename F>
		void Draw(VkCommandBuffer& commandBuffer, F func);

		/**
		* @brief Get m_Pack.
		* @return Returns m_Pack.
		*/
		std::unordered_map<uint32_t, std::shared_ptr<MeshPack>>& GetPacks() { return m_Pack; }

#ifdef RENDERAPI_VULKAN

		/**
		* @brief Create all meshpack AS Input and return it.
		* @return Returns all  meshpack AS Input.
		*/
		std::vector<VulkanRayTracing::BlasInput> CreateMeshPackASInput();

		/**
		* @brief Add Mesh's material to hit group.
		* @param[in,out] hitGroup RayTracingRenderer HitGroup.
		*/
		void AddMaterialToHitGroup(std::unordered_map<std::string, uint32_t>& hitGroup);

#endif

	private:

		/**
		* @brief All packs in this mesh.
		*/
		std::unordered_map<uint32_t, std::shared_ptr<MeshPack>> m_Pack;
	};

	template<typename F>
	inline void Mesh::Draw(VkCommandBuffer& commandBuffer, F func)
	{
		for (auto& pair : m_Pack)
		{
			/**
			* @brief This function is used for bind material parameters.
			* @param[in] meshpackId MeshPack index of array.
			* @param[in] material MeshPack's material.
			*/
			func(pair.first, pair.second->GetMaterial());

			pair.second->OnBind(commandBuffer);
			pair.second->OnDraw(commandBuffer);
		}
	}
}