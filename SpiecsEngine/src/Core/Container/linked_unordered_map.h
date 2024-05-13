/**
* @file linked_unordered_map.h.
* @brief The linked_unordered_map Class Definitions.
* @author Spiecs.
*/

#pragma once

#include <unordered_map>
#include <list>
#include <functional>

namespace scl {
	/**
	* @brief The container combines hashmap and list together.
	* Used in the case that we want iter in order.
	*/
	template<typename K, typename V>
	class linked_unordered_map
	{
	private:

		/**
		* @brief The container keeps iter in order.
		*/
		std::list<K> keys_;

		/**
		* @breif The container keeps quick search.
		*/
		std::unordered_map<K, V> map_;

	public:

		/**
		* @brief Constructor Function.
		*/
		linked_unordered_map() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~linked_unordered_map() {};

		/**
		* @brief Clear this container's data.
		*/
		void clear();

		/**
		* @brief The container's element size.
		* @return Returns the size of the container.
		*/
		int size();

		/**
		* @brief Determain whether the container's element size is same.
		* @return Returns true if the size of the keys_ and map_.
		* @note Used for unit test, shou not be called during game.
		*/
		bool has_equalsize();

		/**
		* @brief Add a element to this container.
		* @param[in] key K the key.
		* @param[in] value V the value.
		*/
		void push_back(const K& key, const V& value);

		/**
		* @breif Find the value by key.
		* @param[in] key K the key.
		* @return Returns the value that founded.
		*/
		V& find_value(const K& key);

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
		* @breif Iter the container in order.
		* @param[in] fn The function of how to iter the container.
		*/
		void for_each(std::function<void(const K&, const V&)> fn);
	};

	template<typename K, typename V>
	inline void linked_unordered_map<K, V>::clear()
	{
		/**
		* @breif Clear list.
		*/
		keys_.clear();

		/**
		* @breif Clear hashmap.
		*/
		map_.clear();
	}

	template<typename K, typename V>
	inline int linked_unordered_map<K, V>::size()
	{
		/**
		* @brief Though we already does unit test, 
		* So not use map_.size() here.
		*/
		return keys_.size();
	}

	template<typename K, typename V>
	inline bool linked_unordered_map<K, V>::has_equalsize()
	{
		return keys_.size() == map_.size();
	}

	template<typename K, typename V>
	inline void linked_unordered_map<K, V>::push_back(const K& key, const V& value)
	{
		if (!has_key(key))
		{
			/**
			* @breif Add to list.
			*/
			keys_.push_back(key);
		}

		/**
		* @breif Update hashmap.
		*/
		map_[key] = value;
	}

	template<typename K, typename V>
	inline V& linked_unordered_map<K, V>::find_value(const K& key)
	{
		/**
		* @brief Get V from map_ only while it does has the key.
		* Otherwise return a new V.
		*/
		if(has_key(key)) return map_[key];
		return V();
	}

	template<typename K, typename V>
	inline bool linked_unordered_map<K, V>::has_key(const K& key)
	{
		if (map_.find(key) != map_.end()) return true;
		else return false;
	}

	template<typename K, typename V>
	inline void linked_unordered_map<K, V>::erase(const K& key)
	{
		auto it = map_.find(key);
		if (it != map_.end())
		{
			/**
			* @brief Remove from list.
			*/
			keys_.remove(key);

			/**
			* @breif Remove from hashmap.
			*/
			map_.erase(it);
		}
	}

	template<typename K, typename V>
	inline void linked_unordered_map<K, V>::for_each(std::function<void(const K&, const V&)> fn)
	{
		for (const K& key : keys_)
		{
			/**
			* @brief The function defines how to iter.
			* @param[in] key K the key.
			* @param[in] value V the value.
			*/
			fn(key, map_[key]);
		}
	}
}