#pragma once
#include "Core/Core.h"
#include "Render/FrameInfo.h"

#include "Slate/Imgui/ImguiConsole.h"
#include "Slate/Imgui/ImguiContentBrowser.h"
#include "Slate/Imgui/ImguiProperty.h"
#include "Slate/Imgui/ImguiStage.h"
#include "Slate/Imgui/ImguiViewport.h"

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
		std::unique_ptr<ImguiViewport> m_ViewPort;
		std::unique_ptr<ImguiStage> m_Stage;
		std::unique_ptr<ImguiProperty> m_Property;
		std::unique_ptr<ImguiContentBrowser> m_ContentBrowser;
		std::unique_ptr<ImguiConsole> m_Console;

	};
}