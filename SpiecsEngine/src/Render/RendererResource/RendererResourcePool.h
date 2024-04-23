#pragma once
#include "Core/Core.h"
#include "RendererResource.h"

#include <unordered_map>
#include <memory>

namespace Spiecs {

	class RendererResourcePool
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		RendererResourcePool() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererResourcePool() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		RendererResourcePool(const RendererResourcePool&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		RendererResourcePool& operator=(const RendererResourcePool&) = delete;


		VkImageView& AccessResource(const std::string& name, const RendererResourceCreateInfo& info);
		VkImageView& AccessDepthResource(const RendererResourceCreateInfo& info);



	private:
		std::unordered_map<std::string, std::unique_ptr<RendererResource>> m_RendererResource;
	};
}