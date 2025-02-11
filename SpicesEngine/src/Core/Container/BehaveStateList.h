/**
* @file BehaveStateList.h.
* @brief The BehaveStateList Class Definitions and Implementation.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace scl {

	/**
	* @brief behave_state_list single node.
	* defines behaves in single state.
	* @tparam Ret Return type.
	* @tparam Args Parameter type.
	*/
	template<typename Ret, typename ...Args>
	class behave_state_node
	{
	private:

		/**
		* @brief Function behaves map.
		*/
		std::unordered_map<std::string, std::function<Ret(Args...)>> m_Behavies;

	public:

		/**
		* @brief Constructor Function.
		*/
		behave_state_node() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~behave_state_node() = default;

		/**
		* @brief Push a behave to this state.
		* @param[in] name behave name.
		* @param[in] fn behave function.
		*/
		void PushBehave(const std::string& name, const std::function<Ret(Args...)>& fn)
		{
			m_Behavies[name] = fn;
		}

		/**
		* @brief Execute a behave in this state.
		* @param[in] name behave name.
		* @param[in] args Parameters
		*/
		void ExecuteBehave(const std::string& name, Args... args)
		{
			if (m_Behavies[name])
			{
				m_Behavies[name](std::forward<Args>(args)...);
			}
		}
	};

	/**
	* @brief behave_state_list.
	* wrapper of combing all state behaves.
	* @tparam Ret Return type.
	* @tparam Args Parameter type.
	*/
	template<typename Ret, typename ...Args>
	class behave_state_list
	{
	private:

		/**
		* @brief Linear Node list.
		*/
		std::vector<std::unique_ptr<behave_state_node<Ret, Args...>>> m_List;

		/**
		* @brief current state.
		*/
		uint32_t m_CurrentState;

	public:

		/**
		* @brief Constructor Function.
		*/
		behave_state_list() : m_CurrentState(0) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~behave_state_list() = default;

		/**
		* @brief Add a empty node to this list.
		* @return Returns empty node.
		*/
		behave_state_node<Ret, Args...>* AddNode()
		{ 
			m_List.push_back(std::make_unique<behave_state_node<Ret, Args...>>());
			return m_List[m_List.size() - 1].get();
		};

		/**
		* @brief Get current state node.
		* @return Returns current state node.
		*/
		behave_state_node<Ret, Args...>* GetState() const
		{
			assert(m_CurrentState < m_List.size());

			return m_List[m_CurrentState].get();
		}

		/**
		* @brief Current State move forward.
		*/
		void IncreateState()
		{
			++m_CurrentState;
			
			m_CurrentState = std::min(m_CurrentState, uint32_t(m_List.size() - 1));
		}

		/**
		* @brief Current State move to start.
		*/
		void ResetState()
		{
			m_CurrentState = 0;
		}

		/**
		* @brief Set Current State.
		* @param[in] state Current State.
		*/
		void SetState(uint32_t state)
		{
			m_CurrentState = state;

			m_CurrentState = std::min(m_CurrentState, uint32_t(m_List.size() - 1));
		}
	};
}