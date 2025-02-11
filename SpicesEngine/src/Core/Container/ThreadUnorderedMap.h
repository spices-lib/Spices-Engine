/**
* @file ThreadUnorderedMap.h.
* @brief The thread_unordered_map Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"
#include <shared_mutex>

namespace scl {

	/**
	* @brief Thread safe unordered_map.
	*/
	template<typename K, typename V>
	class thread_unordered_map
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		thread_unordered_map() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~thread_unordered_map() = default;

		/**
		* @brief Is this queue is empty.
		* @reutrn Returns true if empty.
		*/
		bool IsEmpty() const { return m_Count.load() == 0; }

		/**
		* @brief Insert a element to this.
		* @param[in] k Key.
		* @param[in] v Value.
		*/
		void Insert(K&& k, V&& v);

		/**
		* @brief Insert a element to this.
		* @param[in] k Key.
		* @param[in] v Value.
		*/
		void Insert(const K& k, const V& v);

		/**
		* @brief Find element in this unordered_map.
		* @param[in] k Key.
		* @return Returns value.
		*/
		V& Find(K&& k);

		/**
		* @brief Find element in this unordered_map.
		* @param[in] k Key.
		* @return Returns value.
		*/
		V& Find(const K& k);

		/**
		* @brief Is element in this unordered_map.
		* @param[in] k Key.
		* @return Returns true if found..
		*/
		bool HasKey(K&& k);

		/**
		* @brief Is element in this unordered_map.
		* @param[in] k Key.
		* @return Returns true if found..
		*/
		bool HasKey(const K& k);

		/**
		* @brief Clear this unordered_map.
		*/
		void Clear();

	private:

		/**
		* @brief Mutex of this queue.
		*/
		std::shared_mutex m_Mutex;

		/**
		* @brief This wrapped unordered_map.
		*/
		std::unordered_map<K, V> m_Map;

		/**
		* @brief elements of this unordered_map.
		*/
		std::atomic_uint32_t m_Count;
	};

	
	template<typename K, typename V>
	inline void thread_unordered_map<K, V>::Insert(K&& k, V&& v)
	{
		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		m_Map[k] = v;
	}

	template<typename K, typename V>
	inline void thread_unordered_map<K, V>::Insert(const K& k, const V& v)
	{
		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		m_Map[k] = v;
	}

	template<typename K, typename V>
	inline V& thread_unordered_map<K, V>::Find(K&& k)
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		return m_Map[k];
	}

	template<typename K, typename V>
	inline V& thread_unordered_map<K, V>::Find(const K& k)
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		return m_Map[k];
	}

	template<typename K, typename V>
	inline bool thread_unordered_map<K, V>::HasKey(K&& k)
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		return m_Map.find(k) != m_Map.end();
	}

	template<typename K, typename V>
	inline bool thread_unordered_map<K, V>::HasKey(const K& k)
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		return m_Map.find(k) != m_Map.end();
	}

	template<typename K, typename V>
	inline void thread_unordered_map<K, V>::Clear()
	{
		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		m_Map.clear();
	}
}