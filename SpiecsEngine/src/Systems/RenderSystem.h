/**
* @file RenderSystem.h.
* @brief The RenderSystem Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "SystemManager.h"
#include "Render/FrameInfo.h"
#include "Render/RenderFrontend.h"
#include "Render/Renderer/Renderer.h"
#include <unordered_map>
#include <memory>

namespace Spiecs {

	/**
	* @brief ResourceSystem Class.
	* This class defines the specific behaver of RenderSystem.
	*/
	class RenderSystem : public System
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] systemName The System name.
		*/
		RenderSystem(const std::string& systemName) : System(systemName) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~RenderSystem() override {};

		/**
		* @brief This interface defines the behaver on specific system initialized.
		* Called when system Pushed to SystemManager.
		*/
		virtual void OnSystemInitialize() override;

		/**
		* @brief This interface defines the behaver on specific system shutdown.
		* Called when system poped from SystemManager.
		*/
		virtual void OnSystemShutDown() override;

		/**
		* @brief This interface defines the bahaver on specific system updated every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnSystemUpdate(TimeStep& ts) override;

		/**
		* @brief This interface defines the bahaver on golbal event function pointer is called.
		* @param[in] event Event.
		*/
		virtual void OnEvent(Event& event) override;

		/**
		* @brief Get VulkanState.
		* @return Returns the VulkanState.
		*/
		VulkanState& GetState() { return m_RenderFrontend->GetState(); };

	private:

		/**
		* @brief The RenderFrontend shared pointer.
		*/
		std::unique_ptr<RenderFrontend> m_RenderFrontend;
	};
}