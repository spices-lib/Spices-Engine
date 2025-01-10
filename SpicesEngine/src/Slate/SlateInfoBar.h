/**
* @file SlateInfoBar.h.
* @brief The SlateInfoBar Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief This Class defines InfoBar instance.
	*/
	class SlateInfoBar
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] info InfoBar info content.
		* @param[in] rateFunc InfoBar progress rate.
		*/
		SlateInfoBar(const std::string& info, std::function<float()> rateFunc)
			: m_Info(info)
			, m_RateFunc(rateFunc)
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~SlateInfoBar() = default;

		/**
		* @brief Create InfoBar Instance, and registy to Slate.
		* @param[in] info InfoBar info content.
		* @param[in] rateFunc InfoBar progress rate.
		*/
		static void Create(const std::string& info, std::function<float()> rateFunc);

		/**
		* @brief Get this InfoBar info content.
		* @return Returns this InfoBar info content.
		*/
		const std::string& GetInfo() { return m_Info; };

		/**
		* @brief Get this InfoBar info progress rate.
		* @return Returns this InfoBar info progress rate.
		*/
		float GetRate() { return m_RateFunc(); };

	private:

		/**
		* @brief this InfoBar info content.
		*/
		std::string m_Info;

		/**
		* @brief InfoBar progress rate function.
		*/
		std::function<float()> m_RateFunc;
	};
}