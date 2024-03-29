#pragma once
#include "Core.h"

namespace Spiecs {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void Run();
	};
}