/**
* @file RadixTrie.h.
* @brief The RadixTrie Class Definitions and Implementation.
* @author tcmalloc.
*/

#pragma once
#include "Core/Memory/MemoryPool.h"
#include "Core/Memory/ObjectPool.h"

namespace scl {

	/**
	* @brief Declare of radix_trie.
	* @tparam BITS Bits number.
	* @tparam LAYER page index layer.
	*/
	template<size_t BITS, size_t LAYER>
	class radix_trie;

	/**
	* @brief Implementation of radix_trie with 1 LAYER.
	* @tparam BITS Bits number.
	*/
	template<size_t BITS>
	class radix_trie<BITS, 1>
	{
	private:

		/**
		* @brief array length.
		*/
		static constexpr size_t LENGTH = 1 << BITS;

		/**
		* @brief Leaf Defines.
		*/
		struct Leaf
		{
			std::array<void*, LENGTH> values { nullptr };
		};

		/**
		* @brief Root Node.
		*/
		Leaf* m_Root;

	public:

		/**
		* @brief Constructor Function.
		*/
		explicit radix_trie()
		{
			m_Root = new Leaf;
		}

		/**                           
	    * @brief Deconstruct Function.
	    */                            
		virtual ~radix_trie()
		{
			if (m_Root)
			{
				delete m_Root;
			}
		}

		/**
		* @brief Get item by key.
		* @param[in] k key.
		* @return Returns value;
		*/
		void* get(size_t k) const
		{
			if ((k >> BITS) > 0)
			{
				return nullptr;
			}
		
			return m_Root->values[k];
		}

		/**
		* @brief Set pair of key - value.
		* @param[in] k key.
		* @param[in] v value.
		*/
		void set(size_t k, void* v)
		{
			assert(k >> BITS == 0);
		
			m_Root->values[k] = v;
		}
	};

	/**
	* @brief Implementation of radix_trie with 2 LAYER.
	* @tparam BITS Bits number.
	*/
	template<size_t BITS>
	class radix_trie<BITS, 2>
	{
	private:

		/**
		* @brief root bits.
		*/
		static constexpr size_t ROOT_BITS = 5;

		/**
		* @brief root array length.
		*/
		static constexpr size_t ROOT_LENGTH = 1 << ROOT_BITS;

		/**
		* @brief leaf bits.
		*/
		static constexpr size_t LEAF_BITS = BITS - ROOT_BITS;

		/**
		* @brief leaf array length.
		*/
		static constexpr size_t LEAF_LENGTH = 1 << LEAF_BITS;

		/**
		* @brief Node Defines.
		*/
		struct Node
		{
			std::array<Node*, ROOT_LENGTH> ptrs { nullptr };
		};

		/**
		* @brief Leaf Defines.
		*/
		struct Leaf
		{
			std::array<void*, LEAF_LENGTH> values { nullptr };
		};

		/**
		* @brief Root Node.
		*/
		Node* m_Root;

		/**
		* @brief ObjectPool of Leaf.
		*/
		Spices::ObjectPool<Leaf> m_LeafPool;

	public:

		/**
		* @brief Constructor Function.
		*/
		explicit radix_trie()
			: m_LeafPool{ Spices::ObjectPoolSizeMode::FixedObjects, 20 }
		{
			m_Root = new Node;
		}

		/**
		* @brief Deconstruct Function.
		*/
		virtual ~radix_trie()
		{
			if (m_Root)
			{
				delete m_Root;
			}
		}

		/**
		* @brief Get item by key.
		* @param[in] k key.
		* @return Returns value;
		*/
		void* get(size_t k) const
		{
			const size_t i1 = k >> LEAF_BITS;
			const size_t i2 = k & (LEAF_LENGTH - 1 );

			if ((k >> BITS) > 0 || m_Root->ptrs[i1] == nullptr)
			{
				return nullptr;
			}

			return reinterpret_cast<Leaf*>(m_Root->ptrs[i1])->values[i2];
		}

		/**
		* @brief Set pair of key - value.
		* @param[in] k key.
		* @param[in] v value.
		*/
		void set(size_t k, void* v)
		{
			assert(k >> BITS == 0);

			const size_t i1 = k >> LEAF_BITS;
			const size_t i2 = k & (LEAF_LENGTH - 1);

			assert(i1 < ROOT_LENGTH);
			assert(i2 < LEAF_LENGTH);

			if (!m_Root->ptrs[i1])
			{
				m_Root->ptrs[i1] = reinterpret_cast<Node*>(m_LeafPool.New());
			}

			reinterpret_cast<Leaf*>(m_Root->ptrs[i1])->values[i2] = v;
		}
	};

	/**
	* @brief Implementation of radix_trie with 3 LAYER.
	* @tparam BITS Bits number.
	*/
	template<size_t BITS>
	class radix_trie<BITS, 3>
	{
	private:

		/**
		* @brief interior bits.
		*/
		static constexpr size_t INTERIOR_BITS = (BITS + 2) / 3;

		/**
		* @brief interior array length.
		*/
		static constexpr size_t INTERIOR_LENGTH = 1 << INTERIOR_BITS;

		/**
		* @brief leaf bits.
		*/
		static constexpr size_t LEAF_BITS = BITS - 2 * INTERIOR_BITS;

		/**
		* @brief leaf array length.
		*/
		static constexpr size_t LEAF_LENGTH = 1 << LEAF_BITS;

		/**
		* @brief Node Defines.
		*/
		struct Node
		{
			std::array<Node*, INTERIOR_LENGTH> ptrs { nullptr };
		};

		/**
		* @brief Leaf Defines.
		*/
		struct Leaf
		{
			std::array<void*, LEAF_LENGTH> values { nullptr };
		};

		/**
		* @brief Root Node.
		*/
		Node* m_Root;

		/**
		* @brief ObjectPool of Node.
		*/
		Spices::ObjectPool<Node> m_NodePool;

		/**
		* @brief ObjectPool of Leaf.
		*/
		Spices::ObjectPool<Leaf> m_LeafPool;

	public:

		/**
		* @brief Constructor Function.
		*/
		explicit radix_trie()
			: m_NodePool{ Spices::ObjectPoolSizeMode::FixedObjects, 20 }
			, m_LeafPool{ Spices::ObjectPoolSizeMode::FixedObjects, 20 }
		{
			m_Root = m_NodePool.New();
		}

		/**
		* @brief Deconstruct Function.
		*/
		virtual ~radix_trie() = default;

		/**
		* @brief Get item by key.
		* @param[in] k key.
		* @return Returns value;
		*/
		void* get(size_t k) const
		{
			const size_t i1 = k >> (LEAF_BITS + INTERIOR_BITS);
			const size_t i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH - 1);
			const size_t i3 = k & (LEAF_LENGTH - 1);

			if((k >> BITS) || m_Root->ptrs[i1] == nullptr || m_Root->ptrs[i1]->ptrs[i2] == nullptr)
			{
				return nullptr;
			}

			return reinterpret_cast<Leaf*>(m_Root->ptrs[i1]->ptrs[i2])->values[i3];
		}

		/**
		* @brief Set pair of key - value.
		* @param[in] k key.
		* @param[in] v value.
		*/
		void set(size_t k, void* v)
		{
			assert(k >> BITS == 0);

			const size_t i1 = k >> (LEAF_BITS + INTERIOR_BITS);
			const size_t i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH - 1);
			const size_t i3 = k & (LEAF_LENGTH - 1);

			assert(i1 < INTERIOR_LENGTH);
			assert(i2 < INTERIOR_LENGTH);
			assert(i3 < LEAF_LENGTH);

			if (!m_Root->ptrs[i1])
			{
				m_Root->ptrs[i1] = m_NodePool.New();
			}

			if (!m_Root->ptrs[i1]->ptrs[i2])
			{
				m_Root->ptrs[i1]->ptrs[i2] = reinterpret_cast<Node*>(m_LeafPool.New());
			}

			reinterpret_cast<Leaf*>(m_Root->ptrs[i1]->ptrs[i2])->values[i3] = v;
		}
	};
}