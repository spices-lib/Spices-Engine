/**
* @file MeshComponent.h.
* @brief The MeshComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Resources/Mesh/Mesh.h"

#include <memory>

namespace Spiecs {

	/**
	* @brief MeshComponent Class.
	* This class defines the specific behaver of MeshComponent.
	*/
	class MeshComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		MeshComponent() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~MeshComponent() {};

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override;

		/**
		* @brief This interface defines how to dserialize.
		* @todo Finsih it.
		*/
		virtual void OnDeSerialize() override;

		/**
		* @brief Set the Mesh this component handled.
		* @param[in] mesh The Mesh shared pointer.
		*/
		void SetMesh(std::shared_ptr<Mesh> mesh) { m_Mesh = mesh; };

		/**
		* @brief Get the Mesh variable.
		* @return Returns the Mesh variable.
		*/
		inline std::shared_ptr<Mesh> GetMesh() { return m_Mesh; };

	private:

		/**
		* @brief The Mesh this component handled.
		*/
		std::shared_ptr<Mesh> m_Mesh;
	};
}