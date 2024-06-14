#pragma once
#include <gmock/gmock.h>
#include <Render/Vulkan/VulkanImage.h>

namespace SpiecsTest {

	/**
	* @brief Unit Test for runtime_memory_block
	*/
	class runtime_memory_block_test : public testing::Test
	{
	protected:

		/**
		* @brief Testing class initialize funtion.
		*/
		void SetUp() override {
			m0.add_element("1", "float");
			m0.add_element("2", "float2");
			m0.add_element("3", "float3");
			m0.add_element("4", "float4");
		}

		// void TearDown() override {}

		scl::runtime_memory_block m0;
	};

	/**
	* @brief Testing if initialize successfully.
	*/
	TEST_F(runtime_memory_block_test, Initialize) {

		/**
		* @brief Testing initialized container's size.
		*/
		EXPECT_EQ(m0.size(), 4);

		/**
		* @brief Testing initialized container's bytes.
		*/
		EXPECT_EQ(m0.get_bytes(), 44);

		/**
		* @brief Testing initialized container's address.
		*/
		EXPECT_EQ(m0.get_addr(), nullptr);
	}

	/**
	* @brief Testing if Add element successfully.
	*/
	TEST_F(runtime_memory_block_test, AddElement) {

		/**
		* @brief Testing initialized container's size.
		*/
		m0.add_element("5", "float");
		m0.add_element("6", "float2");

		/**
		* @brief Testing initialized container's size.
		*/
		EXPECT_EQ(m0.size(), 6);

		/**
		* @brief Testing initialized container's bytes.
		*/
		EXPECT_EQ(m0.get_bytes(), 56);

		/**
		* @brief Testing initialized container's address.
		*/
		EXPECT_EQ(m0.get_addr(), nullptr);
	}

	/**
	* @brief Testing if Build successfully.
	*/
	TEST_F(runtime_memory_block_test, Build) {

		/**
		* @brief Testing Build container.
		*/
		scl::runtime_memory_block m1;

		EXPECT_EQ(m0.get_addr(), nullptr);
		EXPECT_EQ(m1.get_addr(), nullptr);

		m1.add_element("1", "float3");

		EXPECT_EQ(m1.get_addr(), nullptr);

		m0.build();
		m1.build();

		EXPECT_NE(m0.get_addr(), nullptr);
		EXPECT_NE(m1.get_addr(), nullptr);
	}

	/**
	* @brief Testing if Explain element successfully.
	*/
	TEST_F(runtime_memory_block_test, ExplainElement) {

		/**
		* @brief Build before explain.
		*/
		m0.build();

		/**
		* @brief Testing if build succeed.
		*/
		EXPECT_NE(m0.get_value<float>("1"), 1.0f);
		EXPECT_NE(m0.get_value<glm::vec2>("2"), glm::vec2(2.0f));
		EXPECT_NE(m0.get_value<glm::vec3>("3"), glm::vec3(3.0f));
		EXPECT_NE(m0.get_value<glm::vec4>("4"), glm::vec4(4.0f));

		/**
		* @brief Testing if float value explained succees.
		*/
		m0.explain_element<float>("1", 1.0f);
		EXPECT_EQ(m0.get_value<float>("1"), 1.0f);

		/**
		* @brief Testing if float2 value explained succees.
		*/
		m0.explain_element<glm::vec2>("2", glm::vec2(2.0f));
		EXPECT_EQ(m0.get_value<glm::vec2>("2"), glm::vec2(2.0f));

		/**
		* @brief Testing if float3 value explained succees.
		*/
		m0.explain_element<glm::vec3>("3", glm::vec3(3.0f));
		EXPECT_EQ(m0.get_value<glm::vec3>("3"), glm::vec3(3.0f));

		/**
		* @brief Testing if float4 value explained succees.
		*/
		m0.explain_element<glm::vec4>("4", glm::vec4(4.0f));
		EXPECT_EQ(m0.get_value<glm::vec4>("4"), glm::vec4(4.0f));
	}

	/**
	* @brief Testing if Destruct successfully.
	*/
	TEST_F(runtime_memory_block_test, Destruct) {

		/**
		* @brief New a object.
		*/
		scl::runtime_memory_block* m1 = new scl::runtime_memory_block();

		/**
		* @brief Testing if init succeed.
		*/
		m1->add_element("1", "float3");
		m1->build();
		m1->explain_element<glm::vec3>("1", glm::vec3(3.0f));

		EXPECT_EQ(m1->get_value<glm::vec3>("1"), glm::vec3(3.0f));

		/**
		* @brief Test failed.
		* @todo More effective test.
		*/
		void* addr = m1->get_addr();
		EXPECT_NE(addr, nullptr);

		delete m1;
		EXPECT_NE(addr, nullptr);
	}
}