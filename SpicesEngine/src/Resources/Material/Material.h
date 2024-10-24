/**
* @file Material.h.
* @brief The Material Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Texture/Texture2D.h"
#include "Resources/Loader/MaterialLoader.h"
#include "Render/Vulkan/VulkanShaderModule.h"
#include "Resources/Texture/Texture.h"
#include "Core/Math/Math.h"
#include "Core/Container/RuntimeMemoryBlock.h"
#include "Core/Container/LinkedUnorderedMap.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "MaterialProperties.h"
#include <glm/glm.hpp>
#include <unordered_map>

namespace Spices {

	/**
	* @brief This struct's data is defined from .material file.
	*/
	struct TextureParam
	{
		std::string textureType;      /* @brief Texture type. */
		std::string texturePath;      /* @brief Texture path. */
		uint32_t    index = 0;        /* @breif Which array index this texture will use. */
	};

	/**
	* @brief This struct's data is defined from .material file.
	*/
	struct ConstantParam
	{	
		std::string paramType;        /* @brief parameter type.  */
		std::any    paramValue;       /* @brief parameter value. */
	};
	
	/**
	* @brief This struct's data is defined from .material file.
	*/
	struct ConstantParams
	{
		ConstantParam value;             /* @brief Value.             */
		ConstantParam defaultValue;      /* @brief Default Value.     */

		bool hasMinValue = false;        /* @brief Is have Min Value. */
		ConstantParam min;               /* @brief Min Value.         */

		bool hasMaxValue = false;        /* @brief Is have Max Value. */
		ConstantParam max;               /* @brief Max Value.         */
	};

	/**
	* @brief Material Class.
	* This class contains a branch of parameter and shader, also descriptor.
	*/
	class Material
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Material() = default;

		/**
		* @brief Constructor Function.
		* Deserialize immediately.
		* Usually call it.
		* @param[in] materialPath Material Path in disk.
		*/
		Material(const std::string& materialPath);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Material();

		/**
		* @brief Serialize this class data to a disk file(.material).
		* @todo Finish it.
		*/
		void Serialize();

		/**
		* @brief Deserialize the data from a disk file(.material) to this class.
		*/
		void Deserialize();

		/**
		* @brief Get Material Path.
		* @return Return Material Path.
		*/
		const std::string& GetName() { return m_MaterialPath; }

		/**
		* @brief Get material shader path.
		* @param[in] stage Shader stage name.
		* @return Returns the stage shader path that needed.
		*/
		const std::vector<std::string>& GetShaderPath(const std::string& stage);

		/**
		* @brief Get material shader path.
		* @return Returns all the stage shader path that needed.
		*/
		const std::unordered_map<std::string, std::vector<std::string>>& GetShaderPath() { return m_Shaders; }

		/**
		* @brief Get material texture parameters.
		* @return Returns the material texture parameters.
		*/
		scl::linked_unordered_map<std::string, TextureParam>& GetTextureParams() { return m_TextureParams; }

		/**
		* @brief Get material constant parameters.
		* @return Returns the material constant parameters.
		*/
		scl::linked_unordered_map<std::string, ConstantParams>& GetConstantParams() { return m_ConstantParams; }

		/**
		* @brief Get default material constant parameter.
		* @param[in] name Parameter Name.
		* @return Returns the default material constant parameter.
		*/
		template<typename T>
		T GetDefaultConstantParams(const std::string& name);

		/**
		* @brief Get material parameter address on GPU.
		* @return the material parameter address on GPU.
		*/
		uint64_t GetMaterialParamsAddress() const;

		/**
		* @brief Get boolean of whether draw a material window.
		* @return the boolean of whether draw a material window.
		*/
		bool GetIsDrawWindow() const { return m_IsDrawWindow; }

		/**
		* @brief Set boolean of whether draw a material window.
		* @param[in] isDrawWindow the boolean of whether draw a material window.
		*/
		void SetIsDrawWindow(bool isDrawWindow) { m_IsDrawWindow = isDrawWindow; }
		
		/**
		* @brief This interface need to be overwritten by specific material.
		* It defines how we build texture and descriptor set.
		* @param[in] isAutoRegistry True if this material need registry to renderer automatically.
		* @todo empty texture.
		*/
		void BuildMaterial(bool isAutoRegistry = true);

		/**
		* @brief Update material data to buffer.
		*/
		void UpdateMaterial();

		/**
		* @brief Allow MaterialLoader access all data.
		*/
		friend class MaterialLoader;

		/**
		* @brief Allow MeshPack access all data.
		*/
		friend class MeshPack;

	protected:

		/**
		* @beief Material path
		*/
		std::string m_MaterialPath;

		/**
		* @brief Shader path
		* Key: shader usage, Value: shader file name.
		*/
		std::unordered_map<std::string, std::vector<std::string>> m_Shaders;
		std::unordered_map<std::string, std::vector<std::string>> m_DefaultShaders;

		/**
		* @brief Texture parameters.
		* Key: parameter name, Value: parameter value.
		*/
		scl::linked_unordered_map<std::string, TextureParam> m_TextureParams;
		scl::linked_unordered_map<std::string, TextureParam> m_DefaultTextureParams;

		/**
		* @brief Constant parameters.
		* Key: parameter name, Value: parameter value.
		*/
		scl::linked_unordered_map<std::string, ConstantParams> m_ConstantParams;

		/**
		* @brief m_Buffers's c++ data container.
		* Key: set, Value: scl::runtime_memory_block.
		*/
		scl::runtime_memory_block m_Buffermemoryblocks;

		/*
		* @brief Buffer takes all textures index and all constant params buffer address. 
		*/
		std::unique_ptr<VulkanBuffer> m_MaterialParameterBuffer;

		/**
		* @brief Properties of render options.
		*/
		MaterialProperties m_Properties;

		/**
		* @brief True if this material needs to draw a window.
		*/
		bool m_IsDrawWindow = false;
	};

	template<typename T>
	inline T Material::GetDefaultConstantParams(const std::string& name)
	{
		SPICES_PROFILE_ZONE;

		auto ptr = m_ConstantParams.find_value(name);
		if(ptr)
		{
			return std::any_cast<T>(ptr->defaultValue.paramValue);
		}
		else
		{
			return T();
		}
	}
}