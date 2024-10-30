/**
* @file ObjectPool.h
* @brief The ObjectPool Class Definitions.
* @author tcmalloc.
*/

#pragma once
#include "Core/Core.h"
#include "MemoryHelper.h"

namespace Spices {

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
		* @brief Constructor Function.
		*/
		ObjectPool() 
			: m_pointer(nullptr)
			, m_FreeList(nullptr)
			, m_SpareBytes(0)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~ObjectPool()
		{
			SPICES_PROFILE_ZONE;

			if (m_Memories.empty()) return;

			/**
			* @brief Free all memory blocks.
			*/
			for(const auto& memoryBlock : m_Memories)
			{
				SystemFree(memoryBlock);
			}
		}

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
		T* New()
		{
			SPICES_PROFILE_ZONE;

			T* obj = nullptr;

			/**
			* @brief Reused space.
			*/
			if (m_FreeList)
			{
				void* next = MemoryHelper::ObjNext(m_FreeList);
				obj        = static_cast<T*>(m_FreeList);
				m_FreeList = next;
			}
			else
			{
				/**
				* @brief Alloc 128KB if there is no enough space.
				*/
				if (m_SpareBytes < sizeof(T))
				{
					m_SpareBytes = static_cast<size_t>(128 * 1024);

					/**
					* @brief Alloc 128 KB / 8KB = 16pages memory. 
					*/
					void* memoryBlock = SystemAlloc(m_SpareBytes >> 13);
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

		/**
		* @brief Free a obj of T.
		* @param[in] obj Object to be free.
		*/
		void Delete(T* obj)
		{
			/**
			* @brief Call Destructor manually.
			*/
			obj->~T();

			/**
			* @brief insert to head.
			*/
			MemoryHelper::ObjNext(obj) = m_FreeList;
			m_FreeList                 = obj;
			
			m_SpareBytes              += sizeof(T);
		}

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
		* @brief Get this mutex. 
		* @return Returns this mutex.
		*/
		std::mutex& GetMutex() { return m_Mutex; }

	private:

		/**
		* @brief objectPool current memory pointer.
		*/
		char* m_pointer;

		/**
		* @brief This objectPoll allocated memories.
		*/
		std::vector<void*> m_Memories;
		
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
	};
}