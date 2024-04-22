/**
* @file RendererManager.h
* @brief The RendererManager Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Render/FrameInfo.h"

namespace Spiecs {

	/**
	* @brief Forward declare
	*/
	class Renderer;

	/**
	* @brief RendererManager Class.
	* This class Manages all renderer.
	*/
	class RendererManager
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		RendererManager() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererManager() {};

		/**
		* @brief Get Static RendererManager.
		* @return Returns the Static RendererManager.
		*/
		static RendererManager& Get();

		/**
		* @brief All renderer Start to Render.
		* @param[in] frameInfo The current frame data.
		*/
		static void Run(FrameInfo& frameInfo);

		/**
		* @brief This function is called on swapchian resized or out of data.
		* Recreate all renderer renderpass, resource and framebuffer.
		* @todo Event from glfw.
		*/
		static void OnWindowResized();

		/**
		* @brief Push a renderer to this manager, and initialize it.
		* @param[in] T Specific Renderer Class.
		* @param[in] rendererName Specific Renderer Name.
		*/
		template<typename T, typename ... Args>
		RendererManager& Push(const std::string& rendererName, Args&& ... args)
		{
			/**
			* @brief Push system to map
			*/
			if (m_Identities.find(rendererName) != m_Identities.end())
			{
				std::cout << "ERROR: " << rendererName << " has been pushed" << std::endl;
				__debugbreak();
			}

			m_Identities[rendererName] = std::unique_ptr<Renderer>(new T(rendererName, std::forward<Args>(args)...));
			m_Identities[rendererName]->OnSystemInitialize();
			
			/**
			* @brief System init
			*/
			std::cout << "INFO: " << rendererName << " pushed" << std::endl;

			return *m_RendererManager;
		}

		/**
		* @brief Pop a renderer from this manager, and destroy it.
		* @param[in] rendererName Specific Renderer Name.
		*/
		RendererManager& Pop(const std::string& rendererName)
		{
			/**
			* @brief Pop system to map
			*/
			if (m_Identities.find(rendererName) == m_Identities.end())
			{
				std::cout << "ERROR: " << rendererName << " has been poped" << std::endl;
				__debugbreak();
			}

			/**
			* @brief System shutdown
			*/
			std::cout << "INFO: " << rendererName << " poped" << std::endl;

			m_Identities.erase(rendererName);

			return *m_RendererManager;
		}

	private:

		/**
		* @brief Sttaic RendererManager.
		*/
		static std::unique_ptr<RendererManager> m_RendererManager;

		/**
		* @brief A container contains all renderer.
		*/
		static std::unordered_map<std::string, std::unique_ptr<Renderer>> m_Identities;
	};

}