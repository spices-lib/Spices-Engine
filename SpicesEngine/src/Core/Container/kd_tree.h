/**
* @file kd_tree.h.
* @brief The kd_tree Class Definitions and Implementation.
* https://www.geeksforgeeks.org/kd-trees-in-cpp/
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Core/Thread/ThreadPool.h"

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
		* @brief using item reperest point in k d.
		*/
		using item = std::array<float, K>;

		/**
		* @brief Structure representing a node in the kd tree.
		*/
		struct Node
		{
			/**
			* @brief Constructor Function.
			* @param[in] pt Points.
			*/
			Node(const item& pt)
				: m_Point(pt)
				, m_Left(nullptr)
				, m_Right(nullptr)
			{}

			/**
			* @brief Destructor Function.
			*/
			virtual ~Node();

			/**
			* @brief Array to store the coordinates.
			*/
			item m_Point;

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

		/**
		* @brief Sizes of this kd tree.
		*/
		std::atomic_size_t m_Size;

	private:

		/**
		* @brief Recursive function to insert a point into the kd_tree.
		* @param[in] node recursive node.
		* @param[in] points Inserted points in k d.
		* @param[in] depth recursive depth.
		*/
		void insert_recursive(
			Node*&                             node  , 
			std::shared_ptr<std::vector<item>> points ,
			int                                depth
		);

		/**
		* @brief Recursive function to insert a point into the kd_tree async.
		* @param[in] node recursive node.
		* @param[in] points Inserted points in k d.
		* @param[in] threadPool ThreadPool.
		* @param[in] depth recursive depth.
		*/
		void insert_recursive_async(
			Node*&                             node  , 
			std::shared_ptr<std::vector<item>> points ,
			Spices::ThreadPool*                threadPool,
			int                                depth
		);

		/**
		* @brief Recursive function to search for a point in the kd_tree.
		* @param[in] node recursive node.
		* @param[in] point Searched point in k d.
		* @param[in] depth recursive depth.
		* @return Returns true if finded.
		*/
		bool search_recursive(
			Node*       node  , 
			const item& point , 
			int         depth
		) const;

		/**
		* @brief Recursive function to search for all nearest points in the kd_tree.
		* @param[in] node recursive node.
		* @param[in] point Searched point in k d.
		* @param[in] condition nearest condition.
		* @param[in] rangePoints in range points.
		* @param[in] depth recursive depth.
		*/
		void range_search_recursive(
			Node*              node        , 
			const item&        point       , 
			const item&        condition   , 
			std::vector<item>& rangePoints ,
			int                depth
		) const;

		/**
		* @brief Recursive function to print the kd_tree.
		* @param[in] node recursive node.
		* @param[in] depth recursive depth.
		*/
		void print_recursive(Node* node, int depth) const;

	public:

		/**
		* @brief Constructor to initialize the kd_tree with a null root.
		*/
		kd_tree()
			: m_Root(nullptr) 
			, m_Size(0)
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
		* @param[in] points Inserted points in k d.
		*/
		void insert(const std::vector<item>& points);

		/**
		* @brief Insert a point into the kd_tree async.
		* Start at the root, comparing the new point¡¯s first dimension with the root¡¯s first dimension.
		* If the new point¡¯s value is less than the root¡¯s, go to the left child; otherwise, go to the right child.
		* At the next level, compare the second dimension. Continue this process, cycling through dimensions.
		* When a leaf is reached, create a new node and insert the new point.
		* @param[in] points Inserted points in k d.
		* @param[in] threadPool ThreadPool.
		*/
		void insert_async(const std::vector<item>& points, Spices::ThreadPool* threadPool);

		/**
		* @brief Search for a point in the kd_tree.
		* Start at the root, comparing the search point¡¯s first dimension with the root¡¯s first dimension.
		* If the search point¡¯s value is less than the root¡¯s, go to the left child; otherwise, go to the right child.
		* At the next level, compare the second dimension. Continue this process, cycling through dimensions.
		* If an exact match is found, return true. If a leaf is reached without finding a match, return false.
		* @param[in] point Searched point in k d.
		*/
		bool search(const item& point) const;

		/**
		* @brief Search for a nearest point in the kd_tree.
		* Traverse the tree as in a normal search to find the leaf node closest to the query point.
		* Unwind the recursion, considering at each step whether there could be any points on the other side of the splitting plane that are closer to the query point than the current best.
		* If there could be, recurse down the other branch, adding any closer points found to the current best.
		* The final best point is the nearest neighbor.
		* @param[in] point Searched point in k d.
		* @param[in] condition allowed distance for neighbours.
		* @return Returns nearest point.
		*/
		auto nearest_neighbour_search(
			const item& point     , 
			const item& condition
		) const -> item;

		/**
		* @brief Search for all points within given range.
		* Start at the root. If the current node is within the range, add it to the result.
		* Recursively search the left and/or right subtrees if the range intersects their respective spaces.
		* Prune the search if the current node¡¯s space does not intersect the query range.
		* @todo Implement it.
		*/
		auto range_search(
			const item& point     , 
			const item& condition
		) const -> std::vector<item>;

		/**
		* @brief Public function to print the kd_tree.
		*/
		void print() const;

		/**
		* @brief Get KD Tree size.
		*/
		size_t size() const { return m_Size.load(); }
	};

	template<uint32_t K>
	inline void kd_tree<K>::insert_recursive(
		Node*&                             node  , 
		std::shared_ptr<std::vector<item>> points,
		int                                depth
	)
	{
		SPICES_PROFILE_ZONE;

		if (points->size() == 0) return;

		/**
		* @brief Calculate current dimension (cd).
		*/
		int cd = depth % K;

		/**
		* @brief Sort points in cd.
		*/
		std::multimap<float, uint32_t> sorted;
		if (points->size() <= 512)
		{
			for (int i = 0; i < points->size(); i++)
			{
				sorted.emplace((*points)[i][cd], i);
			}
		}
		else
		{
			for (int i = 0; i < 512; i++)
			{
				uint32_t index = static_cast<uint32_t>((points->size() - 1) * std::rand() / float(RAND_MAX));
				sorted.emplace((*points)[index][cd], index);
			}
		}

		/**
		* @brief Get Center iterator.
		*/
		auto centerit = sorted.begin();
		for (int i = 0; i < std::floor(sorted.size() * 0.5); i++) centerit++;

		/**
		* @brief Base case: If node is null, create a new node.
		*/
		if (node == nullptr)
		{
			node = new Node((*points)[centerit->second]);
			++m_Size;
		}
		else
		{
			SPICES_CORE_ERROR("Cannot insert a KDTree Node which is not empty.");
		}

		/**
		* @brief Split points to left and right.
		*/
		std::shared_ptr<std::vector<item>> leftPoints  = std::make_shared<std::vector<item>>();
		std::shared_ptr<std::vector<item>> rightPoints = std::make_shared<std::vector<item>>();

		for (int i = 0; i < points->size(); i++)
		{
			if (i == centerit->second) continue;
			else if ((*points)[i][cd] <= centerit->first)
			{
				leftPoints->push_back((*points)[i]);
			}
			else
			{
				rightPoints->push_back((*points)[i]);
			}
		}

		/**
		* @brief Insert in left and right.
		*/
		insert_recursive(node->m_Left,  leftPoints,  depth + 1);
		insert_recursive(node->m_Right, rightPoints, depth + 1);
	}

	template<uint32_t K>
	inline void kd_tree<K>::insert_recursive_async(
		Node*&                             node       , 
		std::shared_ptr<std::vector<item>> points     ,
		Spices::ThreadPool*                threadPool ,
		int                                depth
	)
	{
		SPICES_PROFILE_ZONE;

		const int nPointsWithoutSplitTask = 30000;

		if (points->size() <= nPointsWithoutSplitTask)
		{
			auto in = std::chrono::high_resolution_clock::now();
			insert_recursive(node, points, depth);
			auto out = std::chrono::high_resolution_clock::now();
			std::cout << "    Cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(out - in).count() << "    " << points->size() << std::endl;
			return;
		}

		/**
		* @brief Return if there is no point needs to insert.
		*/
		if (points->size() == 0) return;

		/**
		* @brief Calculate current dimension (cd).
		*/
		int cd = depth % K;

		/**
		* @brief Sort points in cd.
		*/
		std::multimap<float, uint32_t> sorted;
		if (points->size() <= 512)
		{
			for (int i = 0; i < points->size(); i++)
			{
				sorted.emplace((*points)[i][cd], i);
			}
		}
		else
		{
			for (int i = 0; i < 512; i++)
			{
				uint32_t index = static_cast<uint32_t>((points->size() - 1) * std::rand() / float(RAND_MAX));
				sorted.emplace((*points)[index][cd], index);
			}
		}

		/**
		* @brief Get Center iterator.
		*/
		auto centerit = sorted.begin();
		for(int i = 0; i < std::floor(sorted.size() * 0.5); i++) centerit++;

		/**
		* @brief Base case: If node is null, create a new node.
		*/
		if (node == nullptr)
		{
			node = new Node((*points)[centerit->second]);
			++m_Size;
		}
		else
		{
			SPICES_CORE_ERROR("Cannot insert a KDTree Node which is not empty.");
		}

		/**
		* @brief Insert in left.
		*/
		threadPool->SubmitPoolTask([this, cd, points, node, threadPool, depth](float centerVal, uint32_t centerIndex) {
			std::shared_ptr<std::vector<item>> leftPoints = std::make_shared<std::vector<item>>();
			for (int i = 0; i < points->size(); i++)
			{
				if (i == centerIndex) continue;
				else if ((*points)[i][cd] <= centerVal)
				{
					leftPoints->push_back((*points)[i]);
				}
			}
			insert_recursive_async(node->m_Left, leftPoints, threadPool, depth + 1);
		}, centerit->first, centerit->second);
		
		/**
		* @brief Insert in right.
		*/
		threadPool->SubmitPoolTask([this, cd, points, node, threadPool, depth](float centerVal, uint32_t centerIndex) {
			std::shared_ptr<std::vector<item>> rightPoints = std::make_shared<std::vector<item>>();
			for (int i = 0; i < points->size(); i++)
			{
				if (i == centerIndex) continue;
				else if ((*points)[i][cd] > centerVal)
				{
					rightPoints->push_back((*points)[i]);
				}
			}
			insert_recursive_async(node->m_Right, rightPoints, threadPool, depth + 1);
		}, centerit->first, centerit->second);
	}

	template<uint32_t K>
	inline bool kd_tree<K>::search_recursive(
		Node*       node  , 
		const item& point , 
		int         depth
	) 
		const
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
	inline void kd_tree<K>::range_search_recursive(
		Node*              node        , 
		const item&        point       , 
		const item&        condition   , 
		std::vector<item>& rangePoints ,
		int                depth
	) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Base case: If node is null, the point is not found.
		*/
		if (node == nullptr) return;

		/**
		* @brief If the current node within range, than sote it.
		*/
		bool satisfied = true;
		for (int i = 0; i < K; i++)
		{
			if (std::abs(node->m_Point[i] - point[i]) > condition[i])
			{
				satisfied = false;
				break;
			}
		}
		if (satisfied)
		{
			rangePoints.push_back(node->m_Point);
		}

		/**
		* @brief Calculate current dimension (cd).
		*/
		int cd = depth % K;

		/**
		* @brief Recursive both in left anf right.
		*/
		if (std::abs(node->m_Point[cd] - point[cd]) <= condition[cd])
		{
			if (node->m_Left)
			{
				range_search_recursive(node->m_Left, point, condition, rangePoints, depth + 1);
			}
			if (node->m_Right)
			{
				range_search_recursive(node->m_Right, point, condition, rangePoints, depth + 1);
			}
		}
		else if (node->m_Point[cd] - point[cd] > 0.0f)
		{
			if (node->m_Left)
			{
				range_search_recursive(node->m_Left, point, condition, rangePoints, depth + 1);
			}
		}
		else
		{
			if (node->m_Right)
			{
				range_search_recursive(node->m_Right, point, condition, rangePoints, depth + 1);
			}
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
		for (int i = 0; i < depth; i++)
		{
			std::cout << "  ";
		}

		std::cout << "(";
		for (size_t i = 0; i < K; i++) 
		{
			std::cout << node->m_Point[i];
			if (i < K - 1)
			{
				std::cout << ", ";
			}
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
	inline void kd_tree<K>::insert(const std::vector<item>& points)
	{
		SPICES_PROFILE_ZONE;

		insert_recursive(m_Root, std::make_shared<std::vector<item>>(points), 0);
	}

	template<uint32_t K>
	inline void kd_tree<K>::insert_async(const std::vector<item>& points, Spices::ThreadPool* threadPool)
	{
		SPICES_PROFILE_ZONE;

		std::future<bool> rval = threadPool->SubmitPoolTask([&]() {
			insert_recursive_async(m_Root, std::make_shared<std::vector<item>>(points), threadPool, 0);
			return true;
		});
		rval.get();

		/**
		* @todo Only block this task.
		*/
		threadPool->Wait();
	}

	template<uint32_t K>
	inline bool kd_tree<K>::search(const item& point) const
	{
		SPICES_PROFILE_ZONE;

		return search_recursive(m_Root, point, 0);
	}

	template<uint32_t K>
	inline auto kd_tree<K>::nearest_neighbour_search(
		const item& point, 
		const item& condition
	)
		const -> kd_tree<K>::item
	{
		SPICES_PROFILE_ZONE;

		std::vector<kd_tree<K>::item> rangePoints;
		range_search_recursive(m_Root, point, condition, rangePoints, 0);

		if (rangePoints.size() == 0) return {};

		int nearIndex = -1;
		float nearRate = 1E11;
		for (int i = 0; i < rangePoints.size(); i++)
		{
			float rate = 0;
			for (int j = 0; j < K; j++)
			{
				rate += std::abs(rangePoints[i][j] - point[j]);
			}
			if (rate <= nearRate)
			{
				nearRate = rate;
				nearIndex = i;
			}
		}

		return rangePoints[nearIndex];
	}

	template<uint32_t K>
	inline auto kd_tree<K>::range_search(
		const item& point, 
		const item& condition
	) 
		const -> std::vector<kd_tree<K>::item>
	{
		SPICES_PROFILE_ZONE;
		
		std::vector<kd_tree<K>::item> rangePoints;
		range_search_recursive(m_Root, point, condition, rangePoints, 0);
		return rangePoints;
	}

	template<uint32_t K>
	inline void kd_tree<K>::print() const
	{
		SPICES_PROFILE_ZONE;

		print_recursive(m_Root, 0);
	}
}