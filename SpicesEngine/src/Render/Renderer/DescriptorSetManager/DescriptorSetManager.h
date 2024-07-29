/**
* @file DescriptorSetManager.h
* @brief The DescriptorSetManager Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "Core/Math/Math.h"

namespace Spices {
	
	using DescriptorSetInfo          = std::unordered_map<uint32_t, std::shared_ptr<VulkanDescriptorSet>>;
	using DescriptorManagerContainer = std::unordered_map<String2, DescriptorSetInfo>;

	/**
	* @brief This Class manages all descriptor sets this project.
	*/
	class DescriptorSetManager
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		DescriptorSetManager() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~DescriptorSetManager() = default;

		/**
		* @brief Registry a VulkanDescriptorSet, create one if find none.
		* @param name The owner's name of the descriptor set.
		* usually used with renderer descriptor set.
		* @param set The set number of the descriptor set.
		* @return The shared pointer of VulkanDescriptorSet.
		*/
		static std::shared_ptr<VulkanDescriptorSet> Registry(const String2& name, uint32_t set);

		/**
		* @brief Registry a VulkanDescriptorSet, create one if find none.
		* @param name The owner's name of the descriptor set.
		* usually used with material descriptor set.
		* @param set The set number of the descriptor set.
		* @return The shared pointer of VulkanDescriptorSet.
		*/
		static std::shared_ptr<VulkanDescriptorSet> Registry(const std::string& name, uint32_t set);

		/**
		* @brief UnLoad a VulkanDescriptorSet.
		* @param name The owner's name of the descriptor set.
		*/
		static void UnLoad(const String2& name);

		/**
		* @brief UnLoad a VulkanDescriptorSet.
		* @param name The owner's name of the descriptor set.
		*/
		static void UnLoad(const std::string& name);

		/**
		* @brief Forcing UnLoad a VulkanDescriptorSet.
		* @param name The owner's name of the descriptor set.
		*/
		static void UnLoadForce(const String2& name);

		/**
		* @brief Forcing UnLoad a VulkanDescriptorSet.
		* @param name The owner's name of the descriptor set.
		*/
		static void UnLoadForce(const std::string& name);

		/**
		* @brief Forcing UnLoad all VulkanDescriptorSet.
		*/
		static void UnLoadAll();

		/**
		* @brief Get a DescriptorSetInfo by owner's name.
		* @param name The owner's name of the descriptor set.
		* @return DescriptorSetInfo.
		*/
		static DescriptorSetInfo& GetByName(const String2& name);

		/**
		* @brief Get a DescriptorSetInfo by owner's name.
		* @param name The owner's name of the descriptor set.
		* @return DescriptorSetInfo.
		*/
		static DescriptorSetInfo& GetByName(const std::string& name);

	private:
		
		/**
		* @brief The container of all descriptor sets, identified with owner's name and set number.
		*/
		static DescriptorManagerContainer m_DescriptorSets;
	};
}