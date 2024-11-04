/**
* @file DirectedAcyclicGraph.cpp.
* @brief The DirectedAcyclicGraph Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "DirectedAcyclicGraph.h"

namespace scl {

	void directed_acyclic_graph::add_node(directed_acyclic_node* node)
	{
		m_Nodes[node->m_Name] = node;
	}

	void directed_acyclic_graph::execute()
	{
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