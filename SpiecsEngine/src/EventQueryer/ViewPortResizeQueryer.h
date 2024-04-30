#pragma once
#include "Core/Core.h"

namespace Spiecs {

	class ImguiViewport;

	class ViewPortResizeQueryer
	{
	public:
		ViewPortResizeQueryer() {};
		virtual ~ViewPortResizeQueryer() {};

		void QueryEvent();

	private:
		std::shared_ptr<ImguiViewport> m_ViewPort;
	};

}