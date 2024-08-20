/**
* @file SkyBoxComponent.h.
* @brief The SkyBoxComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Resources/Mesh/Mesh.h"
#include "MeshComponent.h"

#include <memory>

namespace Spices {

	/**
	* @brief SkyBoxComponent Class.
	* This class defines the specific behaves of SkyBoxComponent.
	*/
	class SkyBoxComponent : public MeshComponent
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		SkyBoxComponent();

		/**
		* @brief Destructor Function.
		*/
		virtual ~SkyBoxComponent() override = default;

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override;

		/**
		* @brief This interface defines how to deserialize.
		* @todo Finish it.
		*/
		virtual void OnDeSerialize() override;

		/**
		* @brief This interface defines how to draw this component to property panel.
		*/
		virtual void DrawThis() override;
		
		/**
		* @brief Set SkyBox Material.
		* @param[in] material SkyBox Material Path.
		*/
		void SetMaterial(const std::string& materialPath);

		/**
		* @brief Get the Mesh variable.
		* @return Returns the Mesh variable.
		*/
		//std::shared_ptr<Mesh> GetMesh() { return m_Mesh; }

	private:

		/**
		* @brief The Mesh this component handled.
		*/
		//std::shared_ptr<Mesh> m_Mesh;
	};
}