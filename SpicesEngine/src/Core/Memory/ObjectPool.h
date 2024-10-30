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
			: m_Memory(nullptr)
			, m_FreeList(nullptr)
			, m_RemanentBytes(0)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~ObjectPool()
		{
			SPICES_PROFILE_ZONE;

			if (!m_Memory) return;

			//free(m_Memory);
		}

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
				void* next = *(void**)m_FreeList;
				obj = (T*)m_FreeList;
				m_FreeList = next;
			}
			else
			{
				/**
				* @brief Alloc 128k if there is no empty space.
				*/
				if (m_RemanentBytes < sizeof(T))
				{
					m_RemanentBytes = 128 * 1024;

					m_Memory = (char*)SystemAlloc(m_RemanentBytes >> 13);
					if (m_Memory == nullptr)
					{
						SPICES_CORE_ERROR("Memory alloc failed");

						return nullptr;
					}
				}

				/**
				* @brief Min block size is 1 bytes.
				*/
				obj               = (T*)m_Memory;
				size_t objSize    = std::max(size_t(1), sizeof(T));
				m_Memory         += objSize;
				m_RemanentBytes  -= objSize;
			}

			/**
			* @brief Call Construct function of T.
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
			*(void**)obj = m_FreeList;
			m_FreeList = obj;

			size_t objSize = std::max(size_t(1), sizeof(T));
			m_RemanentBytes += objSize;
		}

		/**
		* @brief Get Memory.
		* @return Returns Memory.
		*/
		void* GetMemory() { return (void*)m_Memory; }

		/**
		* @brief Get FreeList.
		* @return Returns FreeList.
		*/
		void* GetFreeList() { return m_FreeList; }

		/**
		* @brief Get RemainBytes.
		* @return Returns RemainBytes.
		*/
		size_t GetRemainBytes() { return m_RemanentBytes; }

		std::mutex& GetMutex() { return m_Mutex; }

	private:

		/**
		* @brief objectpool pointer.
		*/
		char* m_Memory;

		/**
		* @brief freelist.
		*/
		void* m_FreeList;

		/**
		* @brief Remains bytes of objectpool.
		*/
		size_t m_RemanentBytes;

		std::mutex m_Mutex;
	};
}