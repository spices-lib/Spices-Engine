/**
* @file ObjectPool.h
* @brief The ObjectPool Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

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
			: m_Memory(nullptr), m_FreeList(nullptr), m_RemanentBytes(0)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~ObjectPool()
		{
			SPICES_PROFILE_ZONE;

			if (!m_Memory) return;

			free(m_Memory);
		}

		/**
		* @brief Alloc a memory block to store T.
		* @return Returns T pointer.
		*/
		T* New()
		{
			SPICES_PROFILE_ZONE;

			T* obj = nullptr;

			if (m_FreeList)
			{
				void* next = *(void**)m_FreeList;
				obj = (T*)m_FreeList;
				m_FreeList = next;
			}
			else
			{
				if (m_RemanentBytes < sizeof(T))
				{
					m_RemanentBytes = 128 * 1024;
					m_Memory = (char*)malloc(m_RemanentBytes);
					if (m_Memory = nullptr)
					{
						throw std::bad_alloc();
					}
				}
				if (m_Memory == nullptr)
				{
					m_Memory = (char*)malloc(128 * 1024);
					if (m_Memory == nullptr)
					{
						throw std::bad_alloc();
					}
				}

				obj = (T*)m_Memory;
				size_t objSize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);
				m_Memory += objSize;
				m_RemanentBytes -= objSize;
			}

			new(obj)T;

			return obj;
		}

		void* Delete(T* obj)
		{
			obj->~T();

			*(void**)obj = m_FreeList;
			m_FreeList = obj;
		}

	private:
		char* m_Memory;
		void* m_FreeList;
		size_t m_RemanentBytes;
	};
}