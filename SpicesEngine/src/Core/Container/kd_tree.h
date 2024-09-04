/**
* @file kd_tree.h.
* @brief The kd_tree Class Definitions and Implementation.
* https://www.geeksforgeeks.org/kd-trees-in-cpp/
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace scl {

	/**
	* @brief The kd_tree with K dimensions container Class.
	* K the number of dimensions.
	* Every node in the tree is a k-dimensional point.
	* Every non-leaf node generates a splitting hyperplane that divides the space into two parts.
	* Points to the left of the splitting hyperplane are represented by the left subtree of that 
	* node and points to the right of the hyperplane are represented by the right subtree.
	* The hyperplane direction is chosen in the following way: it is perpendicular to the axis 
	* corresponding to the depth of the node (modulo k).
	* The tree is balanced when constructed with points that are uniformly distributed.
	*/
	template<uint32_t K>
	class kd_tree
	{
	public:

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

	private:

		/**
		* @brief Pointer to the root node of the tree.
		*/
		Node* m_Root;

	private:

		/**
		* @brief Recursive function to insert a point into the kd_tree.
		*/
		auto insert_recursive(Node* node, const std::array<float, K>& point, int depth) ->Node*;

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
		* @brief Insert a point into the kd_tree.
		* Start at the root, comparing the new point¡¯s first dimension with the root¡¯s first dimension.
		* If the new point¡¯s value is less than the root¡¯s, go to the left child; otherwise, go to the right child.
		* At the next level, compare the second dimension. Continue this process, cycling through dimensions.
		* When a leaf is reached, create a new node and insert the new point.
		*/
		void insert(const std::array<float, K>& point);

		/**
		* @brief Search for a point in the kd_tree.
		* Start at the root, comparing the search point¡¯s first dimension with the root¡¯s first dimension.
		* If the search point¡¯s value is less than the root¡¯s, go to the left child; otherwise, go to the right child.
		* At the next level, compare the second dimension. Continue this process, cycling through dimensions.
		* If an exact match is found, return true. If a leaf is reached without finding a match, return false.
		*/
		bool search(const std::array<float, K>& point) const;

		/**
		* @brief Search for a nearest point in the kd_tree.
		* Traverse the tree as in a normal search to find the leaf node closest to the query point.
		* Unwind the recursion, considering at each step whether there could be any points on the other side of the splitting plane that are closer to the query point than the current best.
		* If there could be, recurse down the other branch, adding any closer points found to the current best.
		* The final best point is the nearest neighbor.
		* @todo Implement it.
		*/
		void nearest_neighbour_search() const;

		/**
		* @brief Search for all points within given range.
		* Start at the root. If the current node is within the range, add it to the result.
		* Recursively search the left and/or right subtrees if the range intersects their respective spaces.
		* Prune the search if the current node¡¯s space does not intersect the query range.
		* @todo Implement it.
		*/
		void range_search() const;

		/**
		* @brief Public function to print the kd_tree.
		*/
		void print() const;
	};

	template<uint32_t K>
	inline auto kd_tree<K>::insert_recursive(Node* node, const std::array<float, K>& point, int depth) -> kd_tree<K>::Node*
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
	inline void kd_tree<K>::nearest_neighbour_search() const
	{
		SPICES_PROFILE_ZONE;
	}

	template<uint32_t K>
	inline void kd_tree<K>::range_search() const
	{
		SPICES_PROFILE_ZONE;
	}

	template<uint32_t K>
	inline void kd_tree<K>::print() const
	{
		SPICES_PROFILE_ZONE;

		print_recursive(m_Root, 0);
	}
}