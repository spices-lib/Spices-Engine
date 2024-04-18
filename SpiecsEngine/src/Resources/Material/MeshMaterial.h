/**
* @file MeshMaterial.h.
* @brief The MeshMaterial Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Material.h"

namespace Spiecs {

	/**
	* @brief MeshMaterial Class.
	*/
	class MeshMaterial : public Material
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		MeshMaterial() {};

		/**
		* @brief Constructor Function.
		* Init member veriables and Dserialize.
		* @param[in] materialPath The path of material.
		*/
		MeshMaterial(const std::string& materialPath) : Material(materialPath) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~MeshMaterial() {};

	private:

		/**
		* @brief This interface is inherited from Material.
		* It defines how we build texture and descriptor set.
		*/
		virtual void BuildMaterial() override;
	};
}