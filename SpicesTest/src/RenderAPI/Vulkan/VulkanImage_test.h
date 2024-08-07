/**
* @file VulkanImage_test.h.
* @brief The VulkanImage_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Render/Vulkan/VulkanImage.h>
#include "RenderAPI/Vulkan/VulkanRenderBackend_test.h"

#include <memory>

namespace SpicesTest {

	/**
	* @brief Unit Test for VulkanImage.
	*/
	class VulkanImage_test : public VulkanRenderBackend_test
	{
	protected:

		/**
		* @brief The interface is inherited from testing::Test.
		* Registry on Initialize.
		*/
		virtual void SetUp() override {
			VulkanRenderBackend_test::SetUp();

			m_VulkanImage = std::make_unique<Spices::VulkanImage>(
				m_RenderBackend->GetState(),
				"TestImage",
				VK_IMAGE_TYPE_2D,
				m_TextureSize,
				m_TextureSize,
				1,
				VK_SAMPLE_COUNT_1_BIT,
				VK_FORMAT_R32G32B32A32_SFLOAT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |    // Can be used for ColorAttachment.
				VK_IMAGE_USAGE_SAMPLED_BIT          |    // Can be used for ShaderRead.
				VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |    // Can be used for InputAttachment.
				VK_IMAGE_USAGE_TRANSFER_SRC_BIT     |    // Can be used for TransferSrc.
				VK_IMAGE_USAGE_TRANSFER_DST_BIT,         // Can be used for TransferDst.
				0,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				1
			);
		}

		/**
		* @brief The interface is inherited from testing::Test.
		* Registry on ShutDown.
		*/
		virtual void TearDown() override {
			m_VulkanImage = nullptr;
			VulkanRenderBackend_test::TearDown();
		}

	protected:

		/**
		* @brief Texture's width and height
		*/
		const int m_TextureSize = 100;

		/**
		* @brief The unique pointer of VulkanImage.
		*/
		std::unique_ptr<Spices::VulkanImage> m_VulkanImage;
	};

	/**
	* @brief Testing VulkanImage Initialize.
	*/
	TEST_F(VulkanImage_test, Initialize) {

		EXPECT_NE(m_VulkanImage, nullptr);    /* @see Spices::VulkanImage. */
	}

	/**
	* @brief Testing VulkanImage TransferDataWithVkBuffer.
	*/
	TEST_F(VulkanImage_test, TransferDataWithVkBuffer) {

		/**
		* @brief Init with ArrayData.
		*/
		std::vector<float> dataArray(m_TextureSize * m_TextureSize * 4);
		for (int i = 0; i < m_TextureSize * m_TextureSize * 4; i++)
		{
			dataArray[i] = i + 0.0f;
		}

		/**
		* @brief Instance a staginBuffer for transfer data from array to image.
		*/
		Spices::VulkanBuffer stagingBuffer(
			m_RenderBackend->GetState(), 
			sizeof(dataArray),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		/**
		* @brief Copy the data from array to staginBuffer. 
		*/
		void* data;
		vkMapMemory(
			m_RenderBackend->GetState().m_Device, 
			stagingBuffer.GetMemory(), 
			0, 
			sizeof(dataArray),
			0, 
			&data
		);
		memcpy(data, dataArray.data(), sizeof(dataArray));
		vkUnmapMemory(m_RenderBackend->GetState().m_Device, stagingBuffer.GetMemory());
	
		/**
		* @brief Transform Image Layout from origin to Transfer_dst.
		*/
		m_VulkanImage->TransitionImageLayout(VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		/**
		* @brief Copy the data from staginBuffer to Image.
		*/
		m_VulkanImage->CopyBufferToImage(stagingBuffer.Get(), m_VulkanImage->GetImage(), static_cast<uint32_t>(m_TextureSize), static_cast<uint32_t>(m_TextureSize));

		/**
		* @brief Iter all texel and test is get the data correct.
		*/
		for (int i = 0; i < m_TextureSize; i++)
		{
			for (int j = 0; j < m_TextureSize; j++)
			{
				/**
				* @brief Copy the data from Image to array.
				*/
				std::array<float, 4> outDta;
				m_VulkanImage->CopyImageTexelToBuffer(i, j, reinterpret_cast<void*>(outDta.data()));

				EXPECT_EQ(outDta[0], 4.0f * m_TextureSize * j + 4.0f * i + 0.0f);
				EXPECT_EQ(outDta[1], 4.0f * m_TextureSize * j + 4.0f * i + 1.0f);
				EXPECT_EQ(outDta[2], 4.0f * m_TextureSize * j + 4.0f * i + 2.0f);
				EXPECT_EQ(outDta[3], 4.0f * m_TextureSize * j + 4.0f * i + 3.0f);
			}
		}
	}
}