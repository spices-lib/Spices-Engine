/**
* @file SpanList.h.
* @brief The span_list Class Definitions.
* @author tcmalloc.
*/

#pragma once

namespace scl {

	/**
	* @brief Used for manage multiple page memory.
	*/
	class span
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		span();

		/**
		* @brief Destructor Function.
		*/
		virtual ~span() = default;

		/**
		* @brief Start page id.
		*/
		size_t m_PageId;

		/**
		* @brief How much page it managed.
		*/
		size_t m_NPages;

		/**
		* @brief next span.
		*/
		span* m_Next;

		/**
		* @brief previous span.
		*/
		span* m_Prev;

		/**
		* @brief current pointer.
		*/
		void* m_FreeList;

		/**
		* @brief In used memory block count.
		*/
		size_t m_UseCount;
		
		/**
		* @brief True if in use.
		*/
		bool m_IsUse;

		/**
		* @brief page block size.
		*/
		size_t m_BlockSize;
	};

	/**
	* @brief Bidirectional cyclic linked list for span.
	*/
	class span_list
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		span_list();

		/**
		* @brief Destructor Function.
		*/
		virtual ~span_list();

		/**
		* @brief Get begin pointer.
		* @return Returns begin pointer.
		*/
		span* Begin() const;

		/**
		* @brief Get end pointer.
		* @return Returns end pointer.
		*/
		span* End() const;

		/**
		* @brief Push a span to this list.
		* @param[in] s span.
		*/
		void PushFront(span* s);

		/**
		* @brief Pop a span from front.
		* @return Returns front span.
		*/
		span* PopFront();

		/**
		* @brief Empty if Begin equals to End.
		* @return Returns true if empty.
		*/
		bool Empty();

		/**
		* @brief Insert a span before in given position.
		* @param[in] pos inserted position.
		* @param[in] ptr inserted span.
		*/
		static void Insert(span* pos, span* ptr);

		/**
		* @brief Erase a span form this list.
		* @param[in] pos erased position.
		*/
		void Erase(span* pos) const;

		/**
		* @brief Get mutex.
		* @return Returns mutex.
		*/
		std::mutex& GetMutex() { return m_Mutex; }

	private:

		/**
		* @brief list entry point.
		*/
		span* m_Head;

		/**
		* @brief mutex for thread safe.
		*/
		std::mutex m_Mutex;
	};

}