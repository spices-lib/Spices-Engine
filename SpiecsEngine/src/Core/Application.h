#pragma once
#include "Core.h"
#include "World/World/EditorWorld.h"
#include "World/World/RuntimeWorld.h"

#include <memory>

namespace Spiecs {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void Run();
	private:
		
	};
}