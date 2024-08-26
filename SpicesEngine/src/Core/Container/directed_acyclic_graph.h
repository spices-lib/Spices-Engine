/**
* @file directed_acyclic_graph.h.
* @brief The directed_acyclic_graph Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace scl {

	/**
	* @brief DAG Node Class.
	*/
	class directed_acyclic_node
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		directed_acyclic_node() = delete;

		/**
		* @brief Constructor Function.
		* @param[in] name Node name.
		* @param[in] dependencies Node dependencies name.
		* @param[in] func Node function.
		*/
		directed_acyclic_node(std::string name, std::vector<std::string> dependencies, std::function<void()> func)
			: m_Name(name)
			, m_Dependencies(dependencies)
			, m_Func(func)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~directed_acyclic_node() = default;
		
	private:

		/**
		* @brief Node name.
		*/
		std::string m_Name;

		/**
		* @brief Node dependencies name.
		*/
		std::vector<std::string> m_Dependencies;

		/**
		* @brief Node function.
		*/
		std::function<void()> m_Func;

		/**
		* @brief Allow directed_acyclic_graph access all data.
		*/
		friend class directed_acyclic_graph;
	};

	/**
	* @brief DAG Class.
	*/
	class directed_acyclic_graph
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		directed_acyclic_graph() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~directed_acyclic_graph() = default;

		/**
		* @brief Add a node to this graph.
		* @param[in] node directed_acyclic_node.
		*/
		void add_node(directed_acyclic_node* node);

		/**
		* @brief Execute all node function by order.
		*/
		void execute();

		/**
		* @brief Get Node size.
		* @return Returns Node size.
		*/
		uint32_t size() const { return m_Nodes.size(); }

	private:

		/**
		* @brief Execute a node's function.
		* @param[in] node directed_acyclic_node.
		* @param[in] visited graph execute states.
		*/
		void execute_internal(directed_acyclic_node* node, std::unordered_map<std::string, bool>& visited);

	private:

		/**
		* @brief Graph Nodes.
		*/
		std::unordered_map<std::string, directed_acyclic_node*> m_Nodes;
	};
}