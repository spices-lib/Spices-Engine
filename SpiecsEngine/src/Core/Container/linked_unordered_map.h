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
	* Used in the case that we want iter a hashmap in order.
	*/
	template<typename K, typename V>
	class linked_unordered_map
	{
	private:

		/**
		* @brief The container keeps iter in order.
		*/
		std::list<K> keys_ = {};
		
		/**
		* @breif The container keeps quick search.
		*/
		std::unordered_map<K, V> map_ = {};

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
		size_t size();

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
		* @brief Find the value by key.
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
		* @brief Iter the container in order.
		* @param[in] fn The function of how to iter the container.
		*/
		void for_each(std::function<void(const K&, const V&)> fn);

		/**
		* @brief Get the previous element by the key.
		* @param[in] key the key.
		* @return Returns the previous element.
		*/
		V* prev_value(const K& key);

		/**
		* @brief Get the next element by the key.
		* @param[in] key the key.
		* @return Returns the next element.
		*/
		V* next_value(const K& key);

		/**
		* @brief Get the first element this container.
		* @return Returns the first element finded.
		*/
		V& first();

		/**
		* @brief Get the end element this container.
		* @return Returns the end element finded.
		*/
		V& end();
	};

	template<typename K, typename V>
	inline void linked_unordered_map<K, V>::clear()
	{
		/**
		* @brief Clear list.
		*/
		keys_.clear();

		/**
		* @brief Clear hashmap.
		*/
		map_.clear();
	}

	template<typename K, typename V>
	inline size_t linked_unordered_map<K, V>::size()
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
			* @brief Add to list.
			*/
			keys_.push_back(key);
		}

		/**
		* @brief Update hashmap.
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
			* @brief Remove from hashmap.
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

	template<typename K, typename V>
	inline V* linked_unordered_map<K, V>::prev_value(const K& key)
	{
		if (!has_key(key))
		{
			return nullptr;
		}

		for (it = keys_.begin(); it != keys_.end(); it++)
		{
			if (*it == key)
			{
				if (it == keys_.begin())
				{
					return nullptr;
				}
				else
				{
					return it--;
				}
			}
		}

		return nullptr;
	}

	template<typename K, typename V>
	inline V* linked_unordered_map<K, V>::next_value(const K& key)
	{
		if (!has_key(key))
		{
			return nullptr;
		}

		for (it = keys_.begin(); it != keys_.end(); it++)
		{
			if (*it == key)
			{
				if (it == keys_.end())
				{
					return nullptr;
				}
				else
				{
					return it++;
				}
			}
		}

		return nullptr;
	}

	template<typename K, typename V>
	V& linked_unordered_map<K, V>::first()
	{
		return map_[keys_.front()];
	}

	template<typename K, typename V>
	V& linked_unordered_map<K, V>::end()
	{
		return map_[keys_.back()];
	}
}