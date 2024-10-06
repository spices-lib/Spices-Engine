#include "Pchheader.h"
#include "Transcoder.h"
#include "Core/Timer/ScopeTimer.h"
#include "Render/Vulkan/VulkanDevice.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

#include <../external/basisu/transcoder/basisu_transcoder.h>
#include <ktx.h>
#include <../lib/vkformat_enum.h>
#include <stb_image.h>

namespace Spices {

#define KTX_CHECK(expr)                                                                                       \
	{                                                                                                         \
		if (expr == KTX_SUCCESS) {}                                                                           \
		else                                                                                                  \
		{                                                                                                     \
			std::stringstream ss;                                                                             \
			ss << "Assert Failed \n    At File: " << __FILE__ << " \n   At Line: " << __LINE__ << "\n   ";    \
			SPICES_CORE_ERROR(ss.str());                                                                      \
		}                                                                                                     \
	}

	void Transcoder::Init()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief The encoder already initializes the transcoder, but if you haven't initialized the encoder you MUST call this function to initialize the transcoder.
		*/
		basist::basisu_transcoder_init();
	}

	bool Transcoder::SaveSrcToKTX(const std::string& filePath, const unsigned char* data, int width, int height)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a ktxTextureCreateInfo.
		*/
		ktxTextureCreateInfo               createInfo{};
		createInfo.vkFormat              = VK_FORMAT_R8G8B8A8_UNORM;
		createInfo.baseWidth             = width;
		createInfo.baseHeight            = height;
		createInfo.baseDepth             = 1;
		createInfo.numDimensions         = 2;
		createInfo.numLevels             = 1;          // Must be 1 if enable generateMipmaps.
		createInfo.numLayers             = 1;
		createInfo.numFaces              = 1;
		createInfo.isArray               = KTX_FALSE;
		createInfo.generateMipmaps       = KTX_FALSE;  // Must be false before compress.

		ktxTexture2* texture;

		// Call ktxTexture1_Create to create a KTX texture.
		KTX_CHECK(ktxTexture2_Create(&createInfo, KTX_TEXTURE_CREATE_ALLOC_STORAGE, &texture))
		KTX_CHECK(ktxTexture_SetImageFromMemory(ktxTexture(texture), 0, 0, 0, data, width * height * 4))

		ktxBasisParams                     params{0};
		params.structSize                = sizeof(params);
		params.uastc                     = KTX_FALSE;
		params.verbose                   = KTX_FALSE;
		params.noSSE                     = KTX_TRUE;
		params.threadCount               = std::thread::hardware_concurrency();
		params.compressionLevel          = KTX_ETC1S_DEFAULT_COMPRESSION_LEVEL;
		
		/**
		* @brief Set other BasisLZ / ETC1S or UASTC params to change default quality settings.
		*/
		KTX_CHECK(ktxTexture2_CompressBasisEx(texture, &params))

		/**
		* @brief Write to disk.
		*/
		std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());
		ktxTexture_WriteToNamedFile(ktxTexture(texture), filePath.c_str());
		ktxTexture_Destroy(ktxTexture(texture));

		/**
		* @brief Load ktx file and set generateMipmaps to true.
		*/
		KTX_CHECK(ktxTexture2_CreateFromNamedFile(filePath.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture))
		texture->generateMipmaps = KTX_TRUE;

		/**
		* @brief ReWrite angin.
		*/
		ktxTexture_WriteToNamedFile(ktxTexture(texture), filePath.c_str());
		ktxTexture_Destroy(ktxTexture(texture));

		return true;
	}

	bool Transcoder::LoadFromKTX(const std::string& filePath, ktxVulkanTexture* vkTexture)
	{
		SPICES_PROFILE_ZONE;

		ktxTexture2* texture;
		KTX_CHECK(ktxTexture2_CreateFromNamedFile(filePath.c_str(), KTX_TEXTURE_CREATE_NO_FLAGS, &texture))

		if (ktxTexture2_NeedsTranscoding(texture))
		{
			ktx_transcode_fmt_e tf;

			VkPhysicalDeviceFeatures feature = VulkanDevice::GetDeviceFeatures();

			khr_df_model_e colorModel = ktxTexture2_GetColorModel_e(texture);

			if (colorModel == KHR_DF_MODEL_UASTC && feature.textureCompressionASTC_LDR) 
			{
				tf = KTX_TTF_ASTC_4x4_RGBA;
			}
			else if (colorModel == KHR_DF_MODEL_ETC1S && feature.textureCompressionETC2) 
			{
				tf = KTX_TTF_ETC;
			}
			else if (feature.textureCompressionASTC_LDR) 
			{
				tf = KTX_TTF_ASTC_4x4_RGBA;
			}
			else if (feature.textureCompressionETC2)
			{
				tf = KTX_TTF_ETC2_RGBA;
			}
			else if (feature.textureCompressionBC)
			{
				tf = KTX_TTF_BC3_RGBA;
			}
			else 
			{
				SPICES_CORE_ERROR("Vulkan implementation does not support any available transcode target.");
			}

			KTX_CHECK(ktxTexture2_TranscodeBasis(texture, tf, 0))
		}

		ktxVulkanDeviceInfo vdi{};
		VulkanState& state = VulkanRenderBackend::GetState();

		KTX_CHECK(ktxVulkanDeviceInfo_Construct(
			&vdi                       , 
			state.m_PhysicalDevice     , 
			state.m_Device             ,
			state.m_GraphicQueue       , 
			state.m_GraphicCommandPool , 
			nullptr
		))

		KTX_CHECK(ktxTexture2_VkUploadEx(
			texture                                  , 
			&vdi                                     , 
			vkTexture                                ,
			VK_IMAGE_TILING_OPTIMAL                  ,
			VK_IMAGE_USAGE_SAMPLED_BIT               ,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		))

#if 0   // Get Texture MetaData.

		char* pValue;
		uint32_t valueLen;
		KTX_CHECK(ktxHashList_FindValue(
			&texture->kvDataHead    ,
			KTX_ORIENTATION_KEY     ,
			&valueLen               ,
			(void**)&pValue
		))
		{
			char s, t;

			if (sscanf(pValue, KTX_ORIENTATION2_FMT, &s, &t) == 2)
			{

			}
		}

#endif

		ktxTexture_Destroy(ktxTexture(texture));
		ktxVulkanDeviceInfo_Destruct(&vdi);

		return true;
	}
}