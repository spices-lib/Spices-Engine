#pragma once
#include "Core/Core.h"
#include "Render/FrameInfo.h"

#include "Imgui/ImguiSlateBackend.h"

#include <memory>

namespace Spiecs {

	class SlateFrontend
	{
	public:
		SlateFrontend();
		virtual ~SlateFrontend() {};

		SlateFrontend(const SlateFrontend&) = delete;
		SlateFrontend& operator=(const SlateFrontend&) = delete;

		void BeginFrame(FrameInfo& frameInfo);
		void EndFrame(FrameInfo& frameInfo);

		void DrawTest(FrameInfo& frameInfo);

	private:
		std::unique_ptr<ImguiSlateBackend> m_SlateBackend;
	};
}