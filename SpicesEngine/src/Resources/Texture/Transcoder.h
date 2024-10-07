/**
* @file Transcoder.h.
* @brief The Transcoder Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include <ktxvulkan.h>

namespace Spices {

	/**
	* @brief Transcoder of Texture Container format and Tranbsform fromat and GPU cmpress format.
	*/
	class Transcoder
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Transcoder() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~Transcoder() = default;

		/**
		* @brief Init basisu.
		*/
		static void Init();

		/**
		* @brief Create a ktx2 Texture.
		* @param[in] width .
		* @param[in] height .
		* @return Returns ktxTexture2* .
		*/
		static ktxTexture2* CreateKTX2Texture(int& width, int& height);

		/**
		* @brief Wrte Data to ktxtexture.
		* @param[in] texture ktxtexture.
		* @param[in] mipLevel .
		* @param[in] data .
		* @param[in] size data size.
		* @return Returns true if succeed.
		*/
		static bool WriteData(ktxTexture2* texture, uint32_t mipLevel, const unsigned char* data, uint32_t size);

		/**
		* @brief Save a src image data to a ktx file.
		* @param[in] texture ktxtexture.
		* @param[in] filePath saved ktx file path.
		* @return Returns true if finish all task.
		*/
		static bool SaveToDisk(ktxTexture2* texture, const std::string& filePath);

		/**
		* @brief Load a ktx file.
		* @param[in] filePath ktx file path.
		* @param[in,out] texture ktx file.
		* @return Returns true if finish all task.
		*/
		static bool LoadFromKTX(const std::string& filePath, ktxTexture2*& texture);

		/**
		* @brief Destroy a ktx file.
		* @param[in] ktx file.
		* @return Returns true if succeed.
		*/
		static bool DestroyktxTexture2(ktxTexture2* texture);

		/**
		* @brief Get Mipmap data offset.
		* @param[in] texture ktx file.
		* @param[in] mipLevel .
		* @return Returns Mipmap data offset.
		*/
		static ktx_size_t GetMipmapOffset(ktxTexture2* texture, uint32_t mipLevel);

	private:

		/**
		* @brief Check if the device supports sampling and transfers for the selected image.
		* @param[in] format.
		* @return Retuens true if succeed.
		*/
		static bool FormatSupported(VkFormat format);

		/**
		* @brief Get available gpu compress format.
		* @return Returns available gpu compress format.
		*/
		static ktx_transcode_fmt_e GetAvailableTargetFormats();
	};

}