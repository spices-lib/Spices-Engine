/**
* @file RendererResource.cpp.
* @brief The RendererResource Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "RendererResource.h"

namespace Spiecs {

	RendererResource::RendererResource(const RendererResourceCreateInfo& info)
		: m_Info(info)
	{
		/**
		* @brief Set local variable.
		*/
		m_IsResizeable = info.isResizeable;

		/**
		* @brief Create the texture with specific info.
		*/
		m_Texture = std::make_unique<Texture2D>(info);
	}

	void RendererResource::OnResized(uint32_t width, uint32_t height)
	{
		/**
		* @brief Go on if can resize,
		*/
		if (!m_IsResizeable) return;

		/**
		* @brief Update info's width.
		*/
		m_Info.width = width;

		/**
		* @brief Update info's height.
		*/
		m_Info.height = height;

		/**
		* @brief Recreate the texture with specific info.
		*/
		m_Texture = std::make_unique<Texture2D>(m_Info);
	}
}