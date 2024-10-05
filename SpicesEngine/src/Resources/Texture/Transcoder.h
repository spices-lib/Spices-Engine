#pragma once
#include "Core/Core.h"

#include <transcoder/basisu_transcoder.h>

namespace Spices {

	class Transcoder
	{
	public:

		Transcoder() = default;
		virtual ~Transcoder() = default;

		static void Init();

	private:

	};

}