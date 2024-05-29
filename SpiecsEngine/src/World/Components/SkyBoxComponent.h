/**
* @file SkyBoxComponent.h.
* @brief The SkyBoxComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Resources/Mesh/Mesh.h"

#include <memory>

namespace Spiecs {

	/**
	* @brief SkyBoxComponent Class.
	* This class defines the specific behaver of SkyBoxComponent.
	*/
	class SkyBoxComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		SkyBoxComponent() {};

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] path The SkyBoxMaterial file path.
		*/
		SkyBoxComponent(const std::string& path);

		/**
		* @brief Destructor Function.
		*/
		virtual ~SkyBoxComponent() {};

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
		* @brief Defines Component icon on stage.
		* Set m_SlateStageIconPath here.
		* @return Returns Specific m_SlateStageIconPath.
		*/
		virtual const std::string& DefineStageIcon() override;

		/**
		* @brief Get the Mesh variable.
		* @return Returns the Mesh variable.
		*/
		inline const std::shared_ptr<Mesh> GetMesh() { return m_Mesh; };

	private:

		/**
		* @brief The Mesh this component handled.
		*/
		std::shared_ptr<Mesh> m_Mesh;
	};
}