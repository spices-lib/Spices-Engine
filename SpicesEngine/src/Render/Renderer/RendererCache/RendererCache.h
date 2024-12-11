/**
* @file RendererCache.h
* @brief The RendererCache Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include <any>

namespace Spices {

	/**
	* @brief Caches of Renderer data.
	* This class treats all stored caches as garbage and actively cleans them up.
	*/
	class RendererCache
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		RendererCache() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererCache()
		{
			SPICES_PROFILE_ZONE;

			for (int i = 0; i < MaxFrameInFlight; i++)
			{
				ClearCaches(i);
			}
		};

		/**
		* @brief Push a garbage to caches.
		* @param[in] frameIndex .
		* @param[in] any anything.
		*/
		template<typename T>
		void PushToCaches(uint32_t frameIndex, T&& any)
		{
			SPICES_PROFILE_ZONE;

			m_Caches[frameIndex].push_back(any);
		}

		/**
		* @brief Clear all caches.
		* @param[in] frameIndex .
		*/
		void ClearCaches(uint32_t frameIndex)
		{
			SPICES_PROFILE_ZONE;

			m_Caches[frameIndex].clear();
		}

	private:

		/**
		* @brief Renderer caches data.
		*/
		std::array<std::vector<std::any>, MaxFrameInFlight> m_Caches;
	};
}