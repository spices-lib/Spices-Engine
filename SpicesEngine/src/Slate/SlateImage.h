/**
* @file SlateImage.h.
* @brief The SlateImage Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"
#include "Resources/Material/Material.h"

namespace Spices {

	/**
	* @brief Slate image draw context.
	*/
	class SlateImage
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] textureName TextureID texture name.
		* @param[in] materialName Material name.
		*/
		SlateImage(const std::string& textureName, const std::string& materialName);

		/**
		* @brief Destructor Function.
		*/
		virtual ~SlateImage();

		/**
		* @brief Rebuild this ImTextureID.
		*/
		void ReBuildTextureID();

		/**
		* @brief Rebuild this Material.
		*/
		void ReBuildMaterial();

		/**
		* @brief Get this TextureID.
		* @return Return this TextureID.
		*/
		ImTextureID GetTextureID() { return m_TextureID; }

		/**
		* @brief Get this Material.
		* @return Return this Material.
		*/
		std::shared_ptr<Material> GetMaterial() { return m_Material; }

	private:

		/**
		* @brief ImTextureID.
		*/
		ImTextureID m_TextureID;

		/**
		* @brief This Slate image draw material.
		*/
		std::shared_ptr<Material> m_Material;

		/**
		* @brief Texture name.
		*/
		std::string m_TextureName;

		/**
		* @brief Material name.
		*/
		std::string m_MaterialName;
	};
}