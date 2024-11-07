/**
* @file ObjectPool.h
* @brief The ObjectPool Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"
#include "MemoryPool.h"
#include "Core/Library/MemoryLibrary.h"
#include "Core/Container/Vector.h"

namespace Spices {

	/**
	* @brief enum of ObjectPool expand mode
	*/
	enum class ObjectPoolSizeMode
	{
		FixedSize = 0,     // use fixed bytes.
		FixedObjects = 1,  // use fixed bytes of given number of object type.
	};
	
	/**
	* @brief ObjectPool Class.
	* Specific situation(Fixed size of block) of MemoryPool.
	* @tparam T Specific Object Type.
	*/
	template<typename T>
	class ObjectPool
	{
	public:

	/**
	* @brief use 128KB as default expand bytes.
	*/
	static constexpr size_t DefaultExpandKBytes = 128;
	
	public:

		/**
		* @brief Constructor Function.
		* @param[in] mode ObjectPoolSizeMode.
		* @param[in] size FixedSize: Expanded bytes(KB); FixedObjects: Expanded number of objects.
		*/
		ObjectPool(ObjectPoolSizeMode mode = ObjectPoolSizeMode::FixedSize, size_t size = DefaultExpandKBytes);
		
		/**
		* @brief Destructor Function.
		*/
		virtual ~ObjectPool();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		ObjectPool(const ObjectPool&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		ObjectPool& operator=(const ObjectPool&) = delete;
		
		/**
		* @brief Alloc a memory block to store T.
		* @return Returns T pointer.
		*/
		T* New();

		/**
		* @brief Free a obj of T.
		* @param[in] obj Object to be free.
		*/
		void Delete(T* obj);

		/**
		* @brief Get objectPool current memory pointer.
		* @return Returns objectPool current memory pointer.
		*/
		void* GetPointer() const { return static_cast<void*>(m_pointer); }

		/**
		* @brief Get number of memory blocks allocated to this objectPool.
		* @return Returns number of memory blocks allocated to this objectPool.
		*/
		size_t GetNMemoryBlocks() const { return m_Memories.size(); }
		
		/**
		* @brief Get FreeList.
		* @return Returns FreeList.
		*/
		void* GetFreeList() const { return m_FreeList; }

		/**
		* @brief Get SpareBytes.
		* @return Returns SpareBytes.
		*/
		size_t GetSpareBytes() const { return m_SpareBytes; }

		/**
		* @brief Thread Safe Version of New.
		* @return Returns T pointer.
		*/
		T* ThreadNew();

		/**
		* @brief Thread Safe Version of Delete.
		* @param[in] obj Object to be free.
		*/
		void ThreadDelete(T* obj);

	private:

		/**
		* @brief objectPool current memory pointer.
		*/
		char* m_pointer;

		/**
		* @brief This objectPoll allocated memories.
		*/
		scl::vector<void*> m_Memories;
		
		/**
		* @brief freelist.
		*/
		void* m_FreeList;

		/**
		* @brief Spare bytes of this objectPool.
		*/
		size_t m_SpareBytes;

		/**
		* @brief Mutex for thread safety.
		*/
		std::mutex m_Mutex;

		/**
		* @brief Expand bytes.
		*/
		size_t m_ExpandBytes;
	};

	template <typename T>
	ObjectPool<T>::ObjectPool(ObjectPoolSizeMode mode, size_t size)
		: m_pointer(nullptr)
		, m_FreeList(nullptr)
		, m_SpareBytes(0)
	{
		if(mode == ObjectPoolSizeMode::FixedSize)
		{
			m_ExpandBytes = size * 1024;
		}
		else
		{
			m_ExpandBytes = MemoryLibrary::align_up<size_t>(size * sizeof(T), 8 * 1024);
		}
	}

	template <typename T>
	ObjectPool<T>::~ObjectPool()
	{
		if (m_Memories.empty()) return;

		/**
		* @brief Free all memory blocks.
		*/
		for (size_t i = 0; i < m_Memories.size(); i++)
		{
			SystemFree(m_Memories.get(i));
		}
	}

	template <typename T>
	T* ObjectPool<T>::New()
	{
		T* obj = nullptr;

		/**
		* @brief Reused space.
		*/
		if (m_FreeList)
		{
			void* next    = MemoryPool::PointerSpace(m_FreeList);
			obj           = static_cast<T*>(m_FreeList);
			m_FreeList    = next;
			m_SpareBytes -= sizeof(T);
		}
		else
		{
			/**
			* @brief Alloc memory if there is no enough space.
			*/
			if (m_SpareBytes < sizeof(T))
			{
				m_SpareBytes = m_ExpandBytes;

				/**
				* @brief Alloc 128 KB / 8KB = 16pages memory. 
				*/
				void* memoryBlock = SystemAlloc(m_ExpandBytes >> 13);
				if (!memoryBlock)
				{
					SPICES_CORE_ERROR("Memory alloc failed")

					return nullptr;
				}

				m_pointer = static_cast<char*>(memoryBlock);
				m_Memories.push_back(memoryBlock);
			}

			/**
			* @brief Pop current memoryblock as T.
			*/
			obj               = reinterpret_cast<T*>(m_pointer);
				
			m_pointer        += sizeof(T);
			m_SpareBytes     -= sizeof(T);
		}

		/**
		* @brief Call Construct function of T in place.
		*/
		new(obj)T;

		return obj;
	}

	template <typename T>
	void ObjectPool<T>::Delete(T* obj)
	{
		/**
		* @brief Call Destructor manually.
		*/
		obj->~T();

		/**
		* @brief Set memory to 0.
		*/
		memset(obj, 0, sizeof(T));
			
		/**
		* @brief insert to head.
		*/
		MemoryPool::PointerSpace(obj) = m_FreeList;
		m_FreeList                 = obj;
			
		m_SpareBytes              += sizeof(T);
	}

	template<typename T>
	inline T* ObjectPool<T>::ThreadNew()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		
		return New();
	}

	template<typename T>
	inline void ObjectPool<T>::ThreadDelete(T* obj)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		Delete(obj);
	}
}
