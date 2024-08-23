/**
* @file DelegateBasic.h
* @brief The Delegate_Basic Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Core/Container/linked_unordered_map.h"

#include <queue>
#include <functional>
#include <future>

namespace Spices {

	/**
	* @brief Basic Class of Delegate.
	* Instance inherited from it and use delegate feature.
	*/
	template<typename... Args>
	class Delegate_Basic
	{
	public:

		/**
		* @brief Agent Function.
		*/
		using Agent = std::function<void(Args...)>;

	public:

		/**
		* @brief Constructor Function.
		*/
		Delegate_Basic() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~Delegate_Basic() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		Delegate_Basic(const Delegate_Basic&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		Delegate_Basic& operator=(const Delegate_Basic&) = delete;

		/**
		* @brief Bind Function pointer to delegate.
		* @param[in] func Function pointer.
		* @return Returns true if bind successfully.
		*/
		bool Bind(std::function<void(Args...)> func);

		/**
		* @brief UnBind Function pointer from delegate.
		* @param[in] func Function pointer.
		* @return Returns true if unbind successfully.
		*/
		bool UnBind(std::function<void(Args...)> func);

		/**
		* @brief Execute all function pointer. 
		*/
		void Broadcast(Args... args);

	private:

		/**
		* @brief Map of Agent Function Pointer.
		*/
		scl::linked_unordered_map<uint64_t, Agent> m_Agents;
	};

	template<typename ...Args>
	inline bool Delegate_Basic<Args...>::Bind(std::function<void(Args...)> func)
	{
		SPICES_PROFILE_ZONE;

		uint64_t* addr = reinterpret_cast<uint64_t*>(&func);
		if (m_Agents.has_key(*addr))
		{
			SPICES_CORE_WARN("Agent Function binding repeatly.");
			return false;
		}

		/**
		* @brief pack task as a lambda and to map.
		*/
		m_Agents.push_back(*addr, func);
		return true;
	}

	template<typename ...Args>
	inline bool Delegate_Basic<Args...>::UnBind(std::function<void(Args...)> func)
	{
		SPICES_PROFILE_ZONE;

		uint64_t* addr = reinterpret_cast<uint64_t*>(&func);
		if (!m_Agents.has_key(*addr))
		{
			SPICES_CORE_WARN("Agent Function not binded yet.");
			return false;
		}

		/**
		* @brief Erase from map.
		*/
		m_Agents.erase(*addr);
	}

	template<typename ...Args>
	inline void Delegate_Basic<Args...>::Broadcast(Args... args)
	{
		SPICES_PROFILE_ZONE;

		m_Agents.for_each([&](const auto& k, const auto& v) {
			v(std::forward<Args>(args)...);
		});
	}

/**
* @brief Use this macro to instance a Delegate Class.
* None Parameter Specific.
*/
#define DELEGATE_NONE_PARAM(name)                                      \
	class Delegate##name : public Spices::Delegate_Basic<>             \
	{                                                                  \
	public:                                                            \
		Delegate##name() : Spices::Delegate_Basic<>() {}               \
		virtual ~Delegate##name() = default;                           \
	};

/**
* @brief Use this macro to instance a Delegate Class.
* One Parameter Specific.
*/
#define DELEGATE_ONE_PARAM(name, p0)                                       \
	class Delegate##name : public Spices::Delegate_Basic<##p0>             \
	{                                                                      \
	public:                                                                \
		Delegate##name() : Spices::Delegate_Basic<##p0>() {}               \
		virtual ~Delegate##name() = default;                               \
	}; 

/**
* @brief Use this macro to instance a Delegate Class.
* Two Parameter Specific.
*/
#define DELEGATE_TWO_PARAM(name, p0, p1)                                                 \
	class Delegate##name : public Spices::Delegate_Basic<##p0, ##p1>                     \
	{                                                                                    \
	public:                                                                              \
		Delegate##name() : Spices::Delegate_Basic<##p0, ##p1>() {}                       \
		virtual ~Delegate##name() = default;                                             \
	};

/**
* @brief Use this macro to instance a Delegate Class.
* Three Parameter Specific.
*/
#define DELEGATE_THREE_PARAM(name, p0, p1, p2)                                                 \
	class Delegate##name : public Spices::Delegate_Basic<##p0, ##p1, ##p2>                     \
	{                                                                                          \
	public:                                                                                    \
		Delegate##name() : Spices::Delegate_Basic<##p0, ##p1, ##p2>() {}                       \
		virtual ~Delegate##name() = default;                                                   \
	};

/**
* @brief Use this macro to instance a Delegate Class.
* Four Parameter Specific.
*/
#define DELEGATE_FOUR_PARAM(name, p0, p1, p2, p3)                                                    \
	class Delegate##name : public Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3>                     \
	{                                                                                                \
	public:                                                                                          \
		Delegate##name() : Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3>() {}                       \
		virtual ~Delegate##name() = default;                                                         \
	};

/**
* @brief Use this macro to instance a Delegate Class.
* Five Parameter Specific.
*/
#define DELEGATE_FIVE_PARAM(name, p0, p1, p2, p3, p4)                                                      \
	class Delegate##name : public Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4>                     \
	{                                                                                                      \
	public:                                                                                                \
		Delegate##name() : Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4>() {}                       \
		virtual ~Delegate##name() = default;                                                               \
	};

/**
* @brief Use this macro to instance a Delegate Class.
* Six Parameter Specific.
*/
#define DELEGATE_SIX_PARAM(name, p0, p1, p2, p3, p4, p5)                                                         \
	class Delegate##name : public Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4, ##p5>                     \
	{                                                                                                            \
	public:                                                                                                      \
		Delegate##name() : Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4, ##p5>() {}                       \
		virtual ~Delegate##name() = default;                                                                     \
	};

/**
* @brief Use this macro to instance a Delegate Class.
* Seven Parameter Specific.
*/
#define DELEGATE_SEVEN_PARAM(name, p0, p1, p2, p3, p4, p5, p6)                                                         \
	class Delegate##name : public Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4, ##p5, ##p6>                     \
	{                                                                                                                  \
	public:                                                                                                            \
		Delegate##name() : Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4, ##p5, ##p6>() {}                       \
		virtual ~Delegate##name() = default;                                                                           \
	};

/**
* @brief Use this macro to instance a Delegate Class.
* Eight Parameter Specific.
*/
#define DELEGATE_EIGHT_PARAM(name, p0, p1, p2, p3, p4, p5, p6, p7)                                                           \
	class Delegate##name : public Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4, ##p5, ##p6, ##p7>                     \
	{                                                                                                                        \
	public:                                                                                                                  \
		Delegate##name() : Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4, ##p5, ##p6, ##p7>() {}                       \
		virtual ~Delegate##name() = default;                                                                                 \
	};

/**
* @brief Use this macro to instance a Delegate Class.
* Nine Parameter Specific.
*/
#define DELEGATE_NINE_PARAM(name, p0, p1, p2, p3, p4, p5, p6, p7, p8)                                                              \
	class Delegate##name : public Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4, ##p5, ##p6, ##p7, ##p8>                     \
	{                                                                                                                              \
	public:                                                                                                                        \
		Delegate##name() : Spices::Delegate_Basic<##p0, ##p1, ##p2, ##p3, ##p4, ##p5, ##p6, ##p7, ##p8>() {}                       \
		virtual ~Delegate##name() = default;                                                                                       \
	};

}