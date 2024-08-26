/**
* @file directed_acyclic_graph.cpp.
* @brief The directed_acyclic_graph Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "directed_acyclic_graph.h"

namespace scl {

	void directed_acyclic_graph::add_node(directed_acyclic_node* node)
	{
		SPICES_PROFILE_ZONE;

		m_Nodes[node->m_Name] = node;
	}

	void directed_acyclic_graph::execute()
	{
		SPICES_PROFILE_ZONE;

		std::unordered_map<std::string, bool> visited;
		for (auto& node : m_Nodes)
		{
			if (!visited[node.first])
			{
				execute_internal(node.second, visited);
			}
		}
	}

	void directed_acyclic_graph::execute_internal(directed_acyclic_node* node, std::unordered_map<std::string, bool>& visited)
	{
		SPICES_PROFILE_ZONE;

		visited[node->m_Name] = true;
		for (auto& dep : node->m_Dependencies)
		{
			if (!visited[dep])
			{
				execute_internal(m_Nodes[dep], visited);
			}
		}
		node->m_Func();
	}
}