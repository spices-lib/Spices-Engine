/**
* @file Transcoder.cpp.
* @brief The Transcoder Class Implementation.
* @author Spices.
*/

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

	ktxTexture2* Transcoder::CreateKTX2Texture(int& width, int& height)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a ktxTextureCreateInfo.
		*/
		ktxTextureCreateInfo               createInfo;
		createInfo.vkFormat              = VK_FORMAT_R8G8B8A8_UNORM;
		createInfo.baseWidth             = width;
		createInfo.baseHeight            = height;
		createInfo.baseDepth             = 1;
		createInfo.numDimensions         = 2;
		createInfo.numLevels             = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;   // Must be 1 if enable generateMipmaps.
		createInfo.numLayers             = 1;
		createInfo.numFaces              = 1;
		createInfo.isArray               = KTX_FALSE;
		createInfo.generateMipmaps       = KTX_FALSE;  // Must be false before compress.

		ktxTexture2* texture;

		// Call ktxTexture1_Create to create a KTX texture.
		KTX_CHECK(ktxTexture2_Create(&createInfo, KTX_TEXTURE_CREATE_ALLOC_STORAGE, &texture))

		return texture;
	}

	bool Transcoder::WriteData(ktxTexture2* texture, uint32_t mipLevel, const unsigned char* data, uint32_t size)
	{
		SPICES_PROFILE_ZONE;

		KTX_CHECK(ktxTexture_SetImageFromMemory(ktxTexture(texture), mipLevel, 0, 0, data, size))

		return true;
	}

	bool Transcoder::SaveToDisk(ktxTexture2* texture, const std::string& filePath)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a ktxBasisParams.
		* etc1s: unlosslessly compressed, host memory << device memory.
		* uastc: losslessly compressed, host memory == device memory.
		*/
		ktxBasisParams                     params{0};
		params.structSize                = sizeof(params);
		params.uastc                     = KTX_TRUE;
		params.verbose                   = KTX_FALSE;
		params.noSSE                     = KTX_TRUE;

		// etc1s
		params.threadCount               = std::thread::hardware_concurrency();
		params.compressionLevel          = 0;
		params.qualityLevel              = 128;
		
		// uastc
		params.uastcFlags                = KTX_PACK_UASTC_LEVEL_FASTEST;
		params.uastcRDO                  = KTX_TRUE;
		params.uastcRDONoMultithreading  = KTX_TRUE;

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

#if 0   // Disable auto mipmaps, we will manually generate mipmaps.

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

#endif

		return true;
	}

	bool Transcoder::LoadFromKTX(const std::string& filePath, ktxTexture2*& texture)
	{
		SPICES_PROFILE_ZONE;

		KTX_CHECK(ktxTexture2_CreateFromNamedFile(filePath.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture))

		if (ktxTexture2_NeedsTranscoding(texture))
		{
			ktx_transcode_fmt_e tf = GetAvailableTargetFormats();

			khr_df_model_e colorModel = ktxTexture2_GetColorModel_e(texture);

			KTX_CHECK(ktxTexture2_TranscodeBasis(texture, tf, 0))
		}
		
#if 0

		/**
		* @brief Those method use bitimage to create mipmap, which is not suuported by compress format.
		*/
		ktxVulkanDeviceInfo vdi{};
		VulkanState& state = VulkanRenderBackend::GetState();

		KTX_CHECK(ktxVulkanDeviceInfo_Construct(
			&vdi,
			state.m_PhysicalDevice,
			state.m_Device,
			state.m_GraphicQueue,
			state.m_GraphicCommandPool,
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

		/**
		* @brief Get Texture MetaData.
		*/
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

		ktxTexture_Destroy(ktxTexture(texture));
		ktxVulkanDeviceInfo_Destruct(&vdi);

#endif

		return true;
	}

	bool Transcoder::DestroyktxTexture2(ktxTexture2* texture)
	{
		SPICES_PROFILE_ZONE;

		ktxTexture_Destroy(ktxTexture(texture));

		return true;
	}

	ktx_size_t Transcoder::GetMipmapOffset(ktxTexture2* texture, uint32_t mipLevel)
	{
		SPICES_PROFILE_ZONE;

		ktx_size_t offset;
		KTX_CHECK(ktxTexture_GetImageOffset((ktxTexture*)texture, mipLevel, 0, 0, &offset))

		return offset;
	}

	bool Transcoder::FormatSupported(VkFormat format)
	{
		SPICES_PROFILE_ZONE;

		VkFormatProperties properties{};

		VulkanState& state = VulkanRenderBackend::GetState();
		vkGetPhysicalDeviceFormatProperties(state.m_PhysicalDevice, format, &properties);


		VkFormatFeatureFlags  neededFeatures
			= VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT;


		return ((properties.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_DST_BIT) && 
			    (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT));
	}

	ktx_transcode_fmt_e Transcoder::GetAvailableTargetFormats()
	{
		SPICES_PROFILE_ZONE;

		VkPhysicalDeviceFeatures feature = VulkanDevice::GetDeviceFeatures();

		// Block compression
		if (feature.textureCompressionBC)
		{
			// BC7 is the preferred block compression if available
			if (FormatSupported(VK_FORMAT_BC7_SRGB_BLOCK))
			{
				return KTX_TTF_BC7_RGBA;
			}

			if (FormatSupported(VK_FORMAT_BC3_SRGB_BLOCK))
			{
				return KTX_TTF_BC3_RGBA;
			}
		}

		// Adaptive scalable texture compression
		if (feature.textureCompressionASTC_LDR)
		{
			if (FormatSupported(VK_FORMAT_ASTC_4x4_SRGB_BLOCK))
			{
				return KTX_TTF_ASTC_4x4_RGBA;
			}
		}

		// Ericsson texture compression
		if (feature.textureCompressionETC2)
		{
			if (FormatSupported(VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK))
			{
				return KTX_TTF_ETC2_RGBA;
			}
		}

		// PowerVR texture compression support needs to be checked via an extension
		/*if (get_device().is_extension_supported(VK_IMG_FORMAT_PVRTC_EXTENSION_NAME))
		{
			if (FormatSupported(VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG))
			{
				return KTX_TTF_PVRTC1_4_RGBA;
			}
		}*/

		// Always add uncompressed RGBA as a valid target
		return KTX_TTF_RGBA32;
	}
}