#include "Pchheader.h"
#include "NativeScript.h"

namespace Spiecs {

	void NativeScriptRegister::OnUpdate(TimeStep& ts)
	{
		for (int i = 0; i < m_NativeScripts.size(); i++)
		{
			m_NativeScripts[i]->OnTick(ts);
		}
	}

	void NativeScriptRegister::OnEvent(Event& event)
	{
		for (int i = 0; i < m_NativeScripts.size(); i++)
		{
			m_NativeScripts[i]->OnEvent(event);
		}
	}
}