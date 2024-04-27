#include "Pchheader.h"
#include "ImguiConsole.h"

namespace Spiecs {



	void ImguiConsole::OnRender()
	{
		ImGui::Begin(m_PanelName.c_str());
		ImGui::End();
	}

}