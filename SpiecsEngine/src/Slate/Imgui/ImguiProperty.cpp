#include "Pchheader.h"
#include "ImguiProperty.h"

namespace Spiecs {

	void ImguiProperty::OnRender()
	{
		Begin();
		ImGui::ShowDemoWindow();
		End();
	}
}