/**
* @file kd_tree.h.
* @brief The kd_tree Class Definitions and Implementation.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace scl {

	/**
	* @brief The kd_tree with K dimensions container Class.
	* K the number of dimensions.
	*/
	template<uint32_t K>
	class kd_tree
	{
	private:

		/**
		* @brief Structure representing a node in the kd tree.
		*/
		struct Node
		{
			/**
			* @brief Constructor Function.
			* @param[in] pt Points.
			*/
			Node(const std::array<float, K>& pt)
				: m_Point(pt)
				, m_Left(nullptr)
				, m_Right(nullptr) 
			{}

			virtual ~Node();

			/**
			* @brief Array to store the coordinates.
			*/
			std::array<float, K> m_Point;

			/**
			* @brief Pointer to left child.
			*/
			Node* m_Left;

			/**
			* @brief Pointer to right child.
			*/
			Node* m_Right;
		};

		/**
		* @brief Pointer to the root node of the tree.
		*/
		Node* m_Root;

	private:

		/**
		* @brief Recursive function to insert a point into the kd_tree
		*/
		Node* insert_recursive(Node* node, const std::array<float, K>& point, int depth);

		/**
		* @brief Recursive function to search for a point in the kd_tree.
		*/
		bool search_recursive(Node* node, const std::array<float, K>& point, int depth) const;

		/**
		* @brief Recursive function to print the kd_tree.
		*/
		void print_recursive(Node* node, int depth) const;

	public:

		/**
		* @brief Constructor to initialize the kd_tree with a null root.
		*/
		kd_tree()
			: m_Root(nullptr) 
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~kd_tree();

		/**
		* @brief Public function to insert a point into the kd_tree.
		*/
		void insert(const std::array<float, K>& point);

		/**
		* @brief Public function to search for a point in the kd_tree.
		*/
		bool search(const std::array<float, K>& point) const;

		/**
		* @brief Public function to print the kd_tree.
		*/
		void print() const;
	};

	template<uint32_t K>
	inline kd_tree<K>::Node* kd_tree<K>::insert_recursive(Node* node, const std::array<float, K>& point, int depth)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Base case: If node is null, create a new node.
		*/
		if (node == nullptr)
		{
			return new Node(point);
		}

		/**
		* @brief Calculate current dimension (cd).
		*/
		int cd = depth % K;

		/**
		* @brief Compare point with current node and decide to go left or right.
		*/
		if (point[cd] < node->m_Point[cd])
		{
			node->m_Left = insert_recursive(node->m_Left, point, depth + 1);
		}
		else
		{
			node->m_Right = insert_recursive(node->m_Right, point, depth + 1);
		}

		return node;
	}

	template<uint32_t K>
	inline bool kd_tree<K>::search_recursive(Node* node, const std::array<float, K>& point, int depth) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Base case: If node is null, the point is not found.
		*/
		if (node == nullptr)
		{
			return false;
		}

		/**
		* @brief If the current node matches the point, return true.
		*/
		if (node->m_Point == point)
		{
			return true;
		}

		/**
		* @brief Calculate current dimension (cd).
		*/
		int cd = depth % K;

		/**
		* @brief Compare point with current node and decide to go left or right.
		*/
		if (point[cd] < node->m_Point[cd])
		{
			return search_recursive(node->m_Left, point, depth + 1);
		}
		else
		{
			return search_recursive(node->m_Right, point, depth + 1);
		}
	}

	template<uint32_t K>
	inline void kd_tree<K>::print_recursive(Node* node, int depth) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Base case: If node is null, return.
		*/
		if (node == nullptr) return;

		/**
		* @brief Print current node with indentation based on depth.
		*/
		for (int i = 0; i < depth; i++) std::cout << "  ";
		std::cout << "(";
		for (size_t i = 0; i < K; i++) 
		{
			std::cout << node->m_Point[i];
			if (i < K - 1) std::cout << ", ";
		}

		std::cout << ")" << std::endl;

		/**
		* @brief Recursively print left and right children.
		*/
		print_recursive(node->m_Left, depth + 1);
		print_recursive(node->m_Right, depth + 1);
	}

	template<uint32_t K>
	inline kd_tree<K>::Node::~Node()
	{
		SPICES_PROFILE_ZONE;

		if (m_Left)
		{
			delete m_Left;
			m_Left = nullptr;
		}

		if (m_Right)
		{
			delete m_Right;
			m_Right = nullptr;
		}
	}

	template<uint32_t K>
	inline kd_tree<K>::~kd_tree()
	{
		SPICES_PROFILE_ZONE;

		if (m_Root)
		{
			delete m_Root;
			m_Root = nullptr;
		}
	}

	template<uint32_t K>
	inline void kd_tree<K>::insert(const std::array<float, K>& point)
	{
		SPICES_PROFILE_ZONE;

		m_Root = insert_recursive(m_Root, point, 0);
	}

	template<uint32_t K>
	inline bool kd_tree<K>::search(const std::array<float, K>& point) const
	{
		SPICES_PROFILE_ZONE;

		return search_recursive(m_Root, point, 0);
	}

	template<uint32_t K>
	inline void kd_tree<K>::print() const
	{
		SPICES_PROFILE_ZONE;

		print_recursive(m_Root, 0);
	}
}