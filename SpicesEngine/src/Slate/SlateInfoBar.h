/**
* @file SlateInfoBar.h.
* @brief The SlateInfoBar Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Systems/SlateSystem.h"
#include "Slate/Imgui/ImguiInfoBar.h"

namespace Spices {

	/**
	* @brief This Class defines InfoBar instance.
	*/
	class SlateInfoBar
	{
	public:

		/**
		* @brief InfoBar Type.
		*/
		enum class Type
		{
			progress,
			count
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] info InfoBar info content.
		* @param[in] rateFunc InfoBar progress rate.
		* @param[in] type InfoBar Type.
		*/
		SlateInfoBar(const std::string& info, std::function<std::any()>& rateFunc, std::function<bool(SlateInfoBar*)>& destroyFunc, Type type)
			: m_Info(info)
			, m_RateFunc(rateFunc)
			, m_DestroyFunc(destroyFunc)
			, m_Type(type)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~SlateInfoBar() = default;

		/**
		* @brief Create InfoBar Instance, and registy to Slate.
		* @tparam R InfoBar return type.
		* @param[in] info InfoBar info content.
		* @param[in] rateFunc InfoBar progress rate.
		* @param[in] destroyFunc InfoBar destroy condition.
		*/
		template<typename R>
		static void Create(const std::string& info, std::function<std::any()> rateFunc, std::function<bool(SlateInfoBar*)> destroyFunc = nullptr);

		/**
		* @brief Get this InfoBar info content.
		* @return Returns this InfoBar info content.
		*/
		const std::string& GetInfo() { return m_Info; }

		/**
		* @brief Get this InfoBar info progress rate.
		* @return Returns this InfoBar info progress rate.
		*/
		std::any GetRate() 
		{ 
			assert(m_RateFunc);
			return m_RateFunc(); 
		}

		/**
		* @brief Determine if this slate info bar needs to be destroy.
		* @return Returns true if this slate info bar needs to be destroy.
		*/
		bool IsDestroy() 
		{ 
			if (m_DestroyFunc)
			{
				return m_DestroyFunc(this);
			}
			else
			{
				return false;
			}
		}

		/**
		* @brief Get this type.
		* @return Returns this type.
		*/
		Type GetType() const { return m_Type; }

	private:

		/**
		* @brief this InfoBar info content.
		*/
		std::string m_Info;

		/**
		* @brief InfoBar progress rate function.
		*/
		std::function<std::any()> m_RateFunc;

		/**
		* @brief InfoBar progress destroy function.
		*/
		std::function<bool(SlateInfoBar*)> m_DestroyFunc;

		/**
		* @brief InfoBar Type.
		*/
		Type m_Type;

	};

	template<typename R>
	inline void SlateInfoBar::Create(const std::string& info, std::function<std::any()> rateFunc, std::function<bool(SlateInfoBar*)> destroyFunc)
	{
		SPICES_PROFILE_ZONE;

		auto ptr = dynamic_cast<ImguiInfoBar*>(SlateSystem::GetRegister()->GetSlate("InfoBar").get());

		if constexpr (std::is_same_v<R, float>)
		{
			ptr->Push(std::make_shared<SlateInfoBar>(info, rateFunc, destroyFunc, Type::progress));
		}
		else if constexpr (std::is_same_v<R, int>)
		{
			ptr->Push(std::make_shared<SlateInfoBar>(info, rateFunc, destroyFunc, Type::count));
		}
	}
}