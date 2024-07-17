/**
* @file MeshComponent.h.
* @brief The MeshComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Resources/Mesh/Mesh.h"

#include <memory>

namespace Spices {

	/**
	* @brief MeshComponent Class.
	* This class defines the specific behaves of MeshComponent.
	*/
	class MeshComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		MeshComponent() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~MeshComponent() override = default;

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
		* @brief Set the Mesh this component handled.
		* @param[in] mesh The Mesh shared pointer.
		*/
		void SetMesh(std::shared_ptr<Mesh> mesh);

		/**
		* @brief Get the Mesh variable.
		* @return Returns the Mesh variable.
		*/
		std::shared_ptr<Mesh> GetMesh() { return m_Mesh; }

	protected:

		void DrawMaterialPanel(std::shared_ptr<Material> material);
		
	protected:
		
		/**
		* @brief The Mesh this component handled.
		*/
		std::shared_ptr<Mesh> m_Mesh;
	};
}