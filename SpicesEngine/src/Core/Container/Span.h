#pragma once

namespace Spices {

	class Span
	{
	public:

		PageID m_PageId = 0;
		size_t n = 0;

		Span* m_Next = nullptr;
		Span* m_Prev = nullptr;

		void* m_List = nullptr;
		size_t m_UseCount = 0;
	};

}