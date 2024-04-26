#include "Pchheader.h"
#include "SlateRegister.h"

namespace Spiecs {

	void SlateRegister::OnUpdate(TimeStep& ts)
	{
		for (auto& slate : m_Slates)
		{
			slate->OnUpdate(ts);
		}
	}
	void SlateRegister::OnRender()
	{
		for (auto& slate : m_Slates)
		{
			slate->OnRender();
		}
	}
}