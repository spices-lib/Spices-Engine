#include "Pchheader.h"
#include "ImguiProperty.h"

namespace Spiecs {

	void ImguiProperty::OnRender()
	{
		ImGui::Begin(m_PanelName.c_str());

		ImGui::End();
	}
}