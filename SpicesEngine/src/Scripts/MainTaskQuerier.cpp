/**
* @file MainTaskQuerier.cpp.
* @brief The MainTaskQuerier & NativeScriptRegister Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "MainTaskQuerier.h"
#include "Core/Thread/ThreadModel.h"

namespace Spices {

	void MainTaskQuerier::OnTick(TimeStep& ts)
	{
		SPICES_PROFILE_ZONE;

		auto& taskQueue = ThreadModel::Get()->GetMainTaskQueue();

		while (!taskQueue.IsEmpty())
		{
			auto task = taskQueue.Pop();
			task();
		}
	}
}