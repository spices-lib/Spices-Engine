#pragma once

#include <unordered_map>
#include <list>
#include <functional>

namespace scl
{
	template<typename K, typename V>
	class linked_unordered_map
	{
	private:
		std::list<K> keys_;
		std::unordered_map<K, V> map_;

	public:
		linked_unordered_map() {};
		virtual ~linked_unordered_map() {};

		void clear()
		{
			keys_.clear();
			map_.clear();
		}

		int size()
		{
			return keys_.size();
		}

		void insert(const K& key, const V& value)
		{
			if (map_.find(key) == map_.end())
			{
				keys_.push_back(key);
			}
			map_[key] = value;
		}

		V& find(const K& key)
		{
			return map_[key];
		}

		void erase(const K& key)
		{
			auto it = map_.find(key);
			if (it != map_.end())
			{
				keys_.remove(key);
				map_.erase(it);
			}
		}

		void for_each(std::function<void(const K&, const V&)> fn)
		{
			for (const K& key : keys_)
			{
				fn(key, map_[key]);
			}
		}
	};
}