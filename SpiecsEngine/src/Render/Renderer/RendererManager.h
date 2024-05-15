/**
* @file RendererManager.h
* @brief The RendererManager Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Render/FrameInfo.h"
#include "Core/Library/ClassLibrary.h"
#include "Core/Container/linked_unordered_map.h"

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
		static void Run(TimeStep& ts, FrameInfo& frameInfo);

		/**
		* @brief This function is called on swapchian resized or out of data.
		* Recreate all renderer renderpass, resource and framebuffer.
		* @todo Event from glfw.
		*/
		static void OnWindowResizeOver();

		static void OnSlateResize();

		/**
		* @brief Push a renderer to this manager, and initialize it.
		* @param[in] T Specific Renderer Class.
		* @param[in] rendererName Specific Renderer Name.
		*/
		template<typename T, typename ... Args>
		RendererManager& Push(Args&& ... args)
		{
			std::string rendererName = ClassLibrary::GetClassString(typeid(T));

			/**
			* @brief Push system to map
			*/
			if (m_Identities.has_key(rendererName))
			{
				std::stringstream ss;
				ss << rendererName << " has been pushed ";

				SPIECS_CORE_ERROR(ss.str());
			}

			m_Identities.push_back(rendererName, std::shared_ptr<Renderer>(new T(rendererName, std::forward<Args>(args)...)));
			m_Identities.find_value(rendererName)->OnSystemInitialize();

			/**
			* @brief System init
			*/
			std::stringstream ss;
			ss << rendererName << " pushed ";
			SPIECS_CORE_INFO(ss.str());

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
			if (!m_Identities.has_key(rendererName))
			{
				std::stringstream ss;
				ss << rendererName << " has been poped ";

				SPIECS_CORE_ERROR(ss.str());
			}

			/**
			* @brief System shutdown
			*/
			std::stringstream ss;
			ss << rendererName << " poped ";

			SPIECS_CORE_INFO(ss.str());

			m_Identities.erase(rendererName);

			return *m_RendererManager;
		}

		static inline std::shared_ptr<Renderer> GetRenderer(const std::string name) { return m_Identities.find_value(name); };

	private:

		/**
		* @brief Sttaic RendererManager.
		*/
		static std::unique_ptr<RendererManager> m_RendererManager;

		/**
		* @brief A container contains all renderer.
		*/
		static scl::linked_unordered_map<std::string, std::shared_ptr<Renderer>> m_Identities;
	};
}