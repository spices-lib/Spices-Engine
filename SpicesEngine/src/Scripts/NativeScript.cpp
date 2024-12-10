/**
* @file NativeScript.cpp.
* @brief The NativeScript & NativeScriptRegister Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "NativeScript.h"

namespace Spices {

	void NativeScriptRegister::OnUpdate(TimeStep& ts)
	{
		SPICES_PROFILE_ZONE;
		
		for (int i = 0; i < m_NativeScripts.size(); i++)
		{
			m_NativeScripts[i]->OnTick(ts);
		}
	}

	void NativeScriptRegister::OnEvent(Event& event)
	{
		SPICES_PROFILE_ZONE;
		
		for (int i = 0; i < m_NativeScripts.size(); i++)
		{
			m_NativeScripts[i]->OnEvent(event);
		}
	}
}