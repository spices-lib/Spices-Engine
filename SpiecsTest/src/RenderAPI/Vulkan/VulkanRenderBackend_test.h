#pragma once
#include <gmock/gmock.h>
#include <Render/Vulkan/VulkanRenderBackend.h>

namespace SpiecsTest {

	class VulkanRenderBackend_test : public testing::Test
	{

	protected:

		virtual void SetUp() override {
			Spiecs::Log::Init();

			m_RenderBackend = std::make_unique<Spiecs::VulkanRenderBackend>();
		}

		virtual void TearDown() override {
			m_RenderBackend = nullptr;
		};

	protected:

		std::unique_ptr<Spiecs::VulkanRenderBackend> m_RenderBackend;
	};

	TEST_F(VulkanRenderBackend_test, Initialize) {

		EXPECT_NE(m_RenderBackend, nullptr);
	}
}