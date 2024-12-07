#pragma once
#include "Core/Core.h"

namespace Spices {

	enum class StyleLayout
	{
		Default = 0,
		Animation = 1,
		AnimationGraph = 2,
		Paint = 3,
		Rendering = 4,
		VisualScripting = 5,
		PhysicsAuthoring = 6,
	};

	class SlateStyleLayout
	{
	public:

		static std::shared_ptr<SlateStyleLayout> Get();

		void SetLayout(StyleLayout layout);

		void StoreLayout();
		void LoadLayout();

		void StoreLayoutInMemory();
		void LoadLayoutInMemoru();

		void QuickStoreLayout();
		void QuickLoadLayout();

	private:

		static std::shared_ptr<SlateStyleLayout> m_Layout;

		char* m_LayoutCache;
		size_t m_LayoutCacheBytes;
	};
}