#pragma once
#include <gmock/gmock.h>
#include <Core/Container/runtime_memory_block.h>

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
	}
}