/**
* @file Tree.h.
* @brief The Tree Class Definitions and Implementation.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace scl {

	/**
	* @brief simple tree.
	* @tparam T specific stored type.
	*/
	template<typename T>
	class tree
	{
	private:

		/**
		* @brief Children node.
		*/
		std::vector<std::unique_ptr<tree>> m_Childs;

		/**
		* @brief This node stored data.
		*/
		T m_Data;

	public:

		/**
		* @brief Constructor Function.
		*/
		template<typename ...Args>
		tree(Args&&... args)
			: m_Data{ std::forward<Args>(args)... }
			, m_Childs{}
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~tree() = default;

		/**
		* @brief Add a child to this tree.
		* @param[in] args T construct params.
		*/
		template<typename ...Args>
		tree* AddChild(Args&&... args)
		{
			m_Childs.push_back(std::make_unique<tree>(std::forward<Args>(args)...));

			return m_Childs[m_Childs.size() - 1].get();
		}

		/**
		* @brief Get all this node children.
		* @return Returns all this node children.
		*/
		const std::vector<std::unique_ptr<tree>>& GetChilds() const { return m_Childs; }

		/**
		* @breif Get this node data.
		* @return Returns this node data.
		*/
		T& GetData() { return m_Data; }
	};
}