/**
* @file FreeList.h.
* @brief The free_list Class Definitions.
* @author tcmalloc.
*/

#pragma once

namespace scl {

	/**
	* @brief Free list for memory pool.
	* @attention pushed object must large that 8B, for we want store a pointer in there.
	*/
	class free_list
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		free_list()
			: m_EndPointer(nullptr)
			, m_Freelist(nullptr)
			, m_NBlocksAppliedFor(1)
			, m_Size(0)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~free_list() = default;

		/**
		* @brief Recycle a object memory to this free list.
		* @param[in] obj pushed object.
		*/
		void Push(void* obj);

		/**
		* @brief Pop a unused memory block from this free list.
		* @return Returns unused memory block.
		*/
		void* Pop();

		/**
		* @brief Determine if this list is empty.
		* @return Returns true if is empty.
		*/
		bool Empty() const { return m_Freelist == nullptr; }

		/**
		* @brief Get m_NBlocksAppliedFor reference.
		*/
		const size_t ApplyforNBlocks() const { return m_NBlocksAppliedFor; }

		/**
		* @brief Increase count of blocks in next apply for.
		*/
		void IncreaseInNextApplyFor() { m_NBlocksAppliedFor++; }

		/**
		* @brief Get this freelist spare size.
		* @return Returns spare size.
		*/
		const size_t Size() const { return m_Size; }

		/**
		* @brief Push other free list to this free list.
		* @param[in] start other free list start pointer.
		* @param[in] end other free list end pointer.
		* @param[in] size other free list number.
		*/
		void PushRange(void* start, void* end, size_t size);

		/**
		* @brief Pop unused memory blocks from this free list.
		* @param[in,out] start objects start pointer.
		* @param[in,out] end objects end pointer.
		* @param[in] size objects number.
		*/
		void PopRange(void*& start, void*& end, size_t size);

		/**
		* @brief Get current pointer.
		* @return Returns current pointer.
		*/
		const void* Begin() const { return m_Freelist; }

		/**
		* @brief Get current pointer (no const & reference version).
		* @return Returns current pointer.
		*/
		void*& Begin() { return m_Freelist; }

		/**
		* @brief Get end pointer (no const & reference version).
		* @return Returns end pointer.
		*/
		void*& End() { return m_EndPointer; }
		
	private:

		/**
		* @brief end pointer;
		*/
		void* m_EndPointer;

		/**
		* @brief current pointer.
		*/
		void* m_Freelist;

		/**
		* @brief count of blocks applied for in next time.
		*/
		size_t m_NBlocksAppliedFor;

		/**
		* @brief freelist spare size.
		*/
		size_t m_Size;
	};
}