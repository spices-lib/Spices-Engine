#pragma once
#include "Core/Core.h"
#include "Render/FrameInfo.h"

namespace Spiecs {

	class ImguiSlateBackend
	{
	public:
		ImguiSlateBackend();
		virtual ~ImguiSlateBackend() {};

		ImguiSlateBackend(const ImguiSlateBackend&) = delete;
		ImguiSlateBackend& operator=(const ImguiSlateBackend&) = delete;

		void BeginFrame(FrameInfo& frameInfo);
		void EndFrame(FrameInfo& frameInfo);

		void DrawTest(FrameInfo& frameInfo);

	private:
	};
}