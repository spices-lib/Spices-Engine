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
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Set local variable.
		*/
		m_IsResizeable = info.isResizeable;

		/**
		* @brief Create the texture with specific info.
		*/
		switch (info.type)
		{
		case TextureType::Texture2D:
			m_Texture = std::make_unique<Texture2D>(info);
			break;
		case TextureType::Texture2DArray:
			m_Texture = std::make_unique<Texture2DArray>(info);
			break;
		case TextureType::Texture2DCube:
			m_Texture = std::make_unique<Texture2DCube>(info);
			break;
		default:
			SPIECS_CORE_ERROR("Not supported Texture Class Type");
			break;
		}
	}

	void RendererResource::OnResized(const uint32_t width, const uint32_t height)
	{
		SPIECS_PROFILE_ZONE;

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
		switch (m_Info.type)
		{
		case TextureType::Texture2D:
			m_Texture = std::make_unique<Texture2D>(m_Info);
			break;
		case TextureType::Texture2DArray:
			m_Texture = std::make_unique<Texture2DArray>(m_Info);
			break;
		case TextureType::Texture2DCube:
			m_Texture = std::make_unique<Texture2DCube>(m_Info);
			break;
		default:
			SPIECS_CORE_ERROR("Not supported Texture Class Type");
			break;
		}
	}
}