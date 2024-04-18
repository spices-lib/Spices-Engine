/**
* @file SkyBoxMaterial.h.
* @brief The SkyBoxMaterial Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Material.h"

namespace Spiecs {

	/**
	* @brief SkyBoxMaterial Class.
	*/
	class SkyBoxMaterial : public Material
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		SkyBoxMaterial() {};

		/**
		* @brief Constructor Function.
		* Init member veriables and Dserialize.
		* @param[in] materialPath The path of material.
		*/
		SkyBoxMaterial(const std::string& materialPath) : Material(materialPath) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~SkyBoxMaterial() {};

	private:

		/**
		* @brief This interface is inherited from Material.
		* It defines how we build texture and descriptor set.
		*/
		virtual void BuildMaterial() override;
	};

}