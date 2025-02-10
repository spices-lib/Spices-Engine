/**
* @file LinkedUnorderedMap.h.
* @brief The LinkedUnorderedMap Class Definitions and Implementation.
* @author Spices.
*/

#pragma once

#include "Core/Core.h"

#include <unordered_map>
#include <shared_mutex>
#include <list>

namespace scl {

	/**
	* @brief The container combines hashmap and list together.
	* Used in the case that we want iter a hashmap in order.
	* Thread Safe.
	*/
	template<typename K, typename V>
	class linked_unordered_map
	{
	private:

		/**
		* @brief The container keeps iter in order.
		*/
		std::list<K> m_Keys;
		
		/**
		* @breif The container keeps quick search.
		*/
		std::unordered_map<K, V> m_Map;

		/**
		* @brief Mutex for this container.
		*/
		std::shared_mutex m_Mutex;

		/**
		* @brief This container size.
		*/
		std::atomic_int m_Size;

	public:

		/**
		* @brief Constructor Function.
		*/
		linked_unordered_map()
			: m_Size(0)
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~linked_unordered_map() = default;

		/**
		* @brief Determine whether the container's element size is same.
		* @return Returns true if the size of the keys_ and map_.
		* @note Used for unit test, shou not be called during game.
		*/
		bool has_equal_size();

		/**
		* @brief Clear this container's data.
		*/
		void clear();

		/**
		* @brief The container's element size.
		* @return Returns the size of the container.
		*/
		size_t size();

		/**
		* @brief Add a element to this container.
		* @param[in] key K the key.
		* @param[in] value V the value.
		*/
		void push_back(const K& key, const V& value);

		/**
		* @brief Find the value by key.
		* @param[in] key K the key.
		* @return Returns the value that founded.
		*/
		V* find_value(const K& key);

		/**
		* @brief Determine whether the key is in the container.
		* @param[in] key K the key.
		* @return Returns true if has key inside the container.
		*/
		bool has_key(const K& key);

		/**
		* @brief Remove a element inside the container if founded by key.
		* @param[in] key K the key.
		*/
		void erase(const K& key);

		/**
		* @brief Iter the container in order.
		* @param[in] fn The function of how to iter the container.
		*/
		template<typename F>
		void for_each(F& fn);

		/**
		* @brief Iter the container in order.
		* @param[in] fn The function of how to iter the container.
		*/
		template<typename F>
		void for_each(F&& fn);
		
		/**
		* @brief Get the previous element by the key.
		* @param[in] key the key.
		* @return Returns the previous element.
		* @note It's slow, so do not use it unnecessary. 
		*/
		V* prev_value(const K& key);

		/**
		* @brief Get the next element by the key.
		* @param[in] key the key.
		* @return Returns the next element.
		* @note It's slow, so do not use it unnecessary. 
		*/
		V* next_value(const K& key);

		/**
		* @brief Get the first element of this container.
		* @return Returns the first element found.
		*/
		V* first();

		/**
		* @brief Get the end element of this container.
		* @return Returns the end element found.
		*/
		V* end();

		/**
		* @breif Get the end key of this container.
		* @return Returns the end key found.
		*/
		K* end_k();
	};

	template<typename K, typename V>
	inline bool linked_unordered_map<K, V>::has_equal_size()
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		return m_Keys.size() == m_Map.size();
	}

	template<typename K, typename V>
	inline void linked_unordered_map<K, V>::clear()
	{
		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		m_Keys.clear();
		m_Map.clear();
		m_Size = 0;
	}

	template<typename K, typename V>
	inline size_t linked_unordered_map<K, V>::size()
	{
		return m_Size.load();
	}

	template<typename K, typename V>
	inline void linked_unordered_map<K, V>::push_back(const K& key, const V& value)
	{
		bool hasKey = has_key(key);

		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		if (!hasKey)
		{
			m_Keys.push_back(key);

			++m_Size;
		}

		m_Map[key] = value;
	}

	template<typename K, typename V>
	inline V* linked_unordered_map<K, V>::find_value(const K& key)
	{
		bool hasKey = has_key(key);

		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		if (hasKey)
		{
			return &m_Map[key];
		}

		return nullptr;
	}

	template<typename K, typename V>
	inline bool linked_unordered_map<K, V>::has_key(const K& key)
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		if (m_Map.find(key) != m_Map.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename K, typename V>
	inline void linked_unordered_map<K, V>::erase(const K& key)
	{
		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		auto it = m_Map.find(key);
		if (it != m_Map.end())
		{
			m_Keys.remove(key);
			m_Map.erase(it);

			--m_Size;
		}
	}

	template<typename K, typename V>
	template<typename F>
	inline void linked_unordered_map<K, V>::for_each(F& fn)
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		for (const K& key : m_Keys)
		{
			/**
			* @brief The function defines how to iter.
			* @param[in] key K the key.
			* @param[in] value V the value.
			* @return Retunrs True if want break this for loop.
			*/
			if(fn(key, m_Map[key])) break;   //Break If want.
		}
	}

	template <typename K, typename V>
	template <typename F>
	void linked_unordered_map<K, V>::for_each(F&& fn)
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		for (const K& key : m_Keys)
		{
			/**
			* @brief The function defines how to iter.
			* @param[in] key K the key.
			* @param[in] value V the value.
			* @return Retunrs True if want break this for loop.
			*/
			if(fn(key, m_Map[key])) break;   //Break If want.
		}
	}

	template<typename K, typename V>
	inline V* linked_unordered_map<K, V>::prev_value(const K& key)
	{
		/**
		* @breif Returns nullptr if not find key.
		*/
		if (!has_key(key)) return nullptr;

		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		/**
		* @brief Returns nullptr if not a prev value;
		*/
		if (key == m_Keys.front()) return nullptr;

		/**
		* @brief Iter the list.
		*/
		for (auto it = m_Keys.begin(); it != m_Keys.end(); ++it)
		{
			if (*it == key)
			{
				return &(*std::prev(it));
			}
		}

		return nullptr;
	}

	template<typename K, typename V>
	inline V* linked_unordered_map<K, V>::next_value(const K& key)
	{
		/**
		* @breif Returns nullptr if not find key.
		*/
		if (!has_key(key)) return nullptr;

		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		/**
		* @brief Returns nullptr if not a prev value;
		*/
		if (key == m_Keys.back()) return nullptr;

		/**
		* @brief Iter the list.
		*/
		for (auto it = m_Keys.begin(); it != m_Keys.end(); ++it)
		{
			if (*it == key)
			{
				return &(*std::next(it));
			}
		}

		return nullptr;
	}

	template<typename K, typename V>
	V* linked_unordered_map<K, V>::first()
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		if (size() == 0) return nullptr;

		return &m_Map[m_Keys.front()];
	}

	template<typename K, typename V>
	V* linked_unordered_map<K, V>::end()
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		if (size() == 0) return nullptr;

		return &m_Map[m_Keys.back()];
	}

	template<typename K, typename V>
	K* linked_unordered_map<K, V>::end_k()
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		if (size() == 0) return nullptr;

		return &m_Keys.back();
	}
}