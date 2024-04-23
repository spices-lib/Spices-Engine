/**
* @file SlateSystem.cpp.
* @brief The SlateSystem Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SlateSystem.h"

namespace Spiecs {

	void SlateSystem::OnSystemInitialize()
	{
		/**
		* @brief Init a SlateFrontend.
		*/
		m_SlateFrontend = std::make_unique<SlateFrontend>();
	}

	void SlateSystem::OnSystemShutDown()
	{

	}

	void SlateSystem::OnSystemUpdate(TimeStep& ts)
	{
		m_SlateFrontend->BeginFrame(FrameInfo::Get());

		m_SlateFrontend->DrawTest(FrameInfo::Get());

		m_SlateFrontend->EndFrame(FrameInfo::Get());
	}
}