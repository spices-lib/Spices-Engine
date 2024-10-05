#include "Pchheader.h"
#include "Transcoder.h"

namespace Spices {

	void Transcoder::Init()
	{
		/**
		* @brief The encoder already initializes the transcoder, but if you haven't initialized the encoder you MUST call this function to initialize the transcoder.
		*/
		basist::basisu_transcoder_init();


	}
}