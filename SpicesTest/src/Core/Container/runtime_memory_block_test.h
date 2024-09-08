/**
* @file runtime_memory_block_test.h.
* @brief The runtime_memory_block_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/runtime_memory_block.h>
#include "Instrumentor.h"

namespace SpicesTest {

	/**
	* @brief Unit Test for runtime_memory_block
	*/
	class runtime_memory_block_test : public testing::Test
	{
	protected:

		/**
		* @brief The interface is inherited from testing::Test.
		* Registry on Initialize.
		*/
		void SetUp() override {
			m0.add_element("1", "float");
			m0.add_element("2", "float2");
			m0.add_element("3", "float3");
			m0.add_element("4", "float4");
		}

		/**
		* @brief Testing class TearDown function.
		*/
		void TearDown() override {}

		scl::runtime_memory_block m0;        /* @brief Test Item. */
	};

	/**
	* @brief Testing if initialize successfully.
	*/
	TEST_F(runtime_memory_block_test, Initialize) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Testing initialized container's size.
		*/
		EXPECT_EQ(m0.size(), 4);

		/**
		* @brief Testing initialized container's bytes.
		*/
		EXPECT_EQ(m0.get_bytes(), 40);

		/**
		* @brief Testing initialized container's address.
		*/
		EXPECT_EQ(m0.get_addr(), nullptr);

		/**
		* @brief Testing initialized container's item_location.
		*/
		EXPECT_EQ(m0.item_location("1"), 0);
		EXPECT_EQ(m0.item_location("2"), 4);
		EXPECT_EQ(m0.item_location("3"), 12);
		EXPECT_EQ(m0.item_location("4"), 24);
	}

	/**
	* @brief Testing if Add element successfully.
	*/
	TEST_F(runtime_memory_block_test, AddElement) {

		SPICESTEST_PROFILE_FUNCTION();

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
		EXPECT_EQ(m0.get_bytes(), 52);

		/**
		* @brief Testing initialized container's address.
		*/
		EXPECT_EQ(m0.get_addr(), nullptr);

		/**
		* @brief Testing initialized container's item_location.
		*/
		EXPECT_EQ(m0.item_location("5"), 40);
		EXPECT_EQ(m0.item_location("6"), 44);
	}

	/**
	* @brief Testing if Build successfully.
	*/
	TEST_F(runtime_memory_block_test, Build) {

		SPICESTEST_PROFILE_FUNCTION();

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

		SPICESTEST_PROFILE_FUNCTION();

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

		SPICESTEST_PROFILE_FUNCTION();

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

	/**
	* @brief Testing if ItemLocation test successfully.
	*/
	TEST_F(runtime_memory_block_test, ItemLocation) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief item location in valid item.
		*/
		EXPECT_EQ(m0.item_location("1"), 0 );
		EXPECT_EQ(m0.item_location("2"), 4 );
		EXPECT_EQ(m0.item_location("3"), 12);
		EXPECT_EQ(m0.item_location("4"), 24);

		/**
		* @brief item location in unvalid item.
		*/
		EXPECT_EQ(m0.item_location("5"), UINT32_MAX);
		EXPECT_EQ(m0.item_location("6"), UINT32_MAX);
		EXPECT_EQ(m0.item_location("7"), UINT32_MAX);
		EXPECT_EQ(m0.item_location("8"), UINT32_MAX);
	}
}