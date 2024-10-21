/**
* @file LinkedUnorderedMap_test.h.
* @brief The LinkedUnorderedMap_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/LinkedUnorderedMap.h>
#include "Instrumentor.h"

namespace SpicesTest {

	/**
	* @brief The interface is inherited from testing::Test.
	* Registy on Initialize.
	*/
	class linked_unordered_map_test : public testing::Test
	{
	protected:

		/**
		* @brief Testing class initialize function.
		*/
		void SetUp() override {

			c0.push_back("aaa", "aaa");
			c0.push_back("bbb", "bbb");
			c0.push_back("ccc", "ccc");

			c1.push_back(1, "1");
			c1.push_back(2, "2");
			c1.push_back(3, "3");

			c2.push_back(1.0f, "1.0");
			c2.push_back(2.0f, "2.0");
			c2.push_back(3.0f, "3.0");
		}

		/**
		* @brief Testing class TearDown function.
		*/
		void TearDown() override {}

		scl::linked_unordered_map<std::string, std::string> c0;    /* @brief Test Item. */
		scl::linked_unordered_map<int, std::string> c1;            /* @brief Test Item. */
		scl::linked_unordered_map<float, std::string> c2;          /* @brief Test Item. */
	};

	/**
	* @brief Testing if initialize successfully.
	*/
	TEST_F(linked_unordered_map_test, Initialize) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Testing initialized container's size.
		*/
		EXPECT_EQ(c0.size(), 3);
		EXPECT_EQ(c1.size(), 3);
		EXPECT_EQ(c2.size(), 3);

		/**
		* @brief Testing initialized container's euqlity.
		*/
		EXPECT_EQ(c0.has_equal_size(), true);
		EXPECT_EQ(c1.has_equal_size(), true);
		EXPECT_EQ(c2.has_equal_size(), true);
	}

	/**
	* @brief Testing if Remove element successfully.
	*/
	TEST_F(linked_unordered_map_test, RemoveElement) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Testing if clear successfully.
		*/
		c0.clear();
		EXPECT_EQ(c0.size(), 0);
		
		/**
		* @brief Testing if erase successfully.
		*/
		c1.erase(1);
		EXPECT_EQ(c1.size(), 2);
		
		c2.erase(2.0f);
		c2.erase(3.0f);
		EXPECT_EQ(c2.size(), 1);

		/**
		* @brief Testing if erase the element not in this container,
		*/
		c1.erase(10);
		EXPECT_EQ(c1.size(), 2);

		/**
		* @brief Testing container's euqlity after remove element.
		*/
		EXPECT_EQ(c0.has_equal_size(), true);
		EXPECT_EQ(c1.has_equal_size(), true);
		EXPECT_EQ(c2.has_equal_size(), true);
	}

	/**
	* @brief Testing if Find element successfully.
	*/
	TEST_F(linked_unordered_map_test, FindElement) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Testing if find a exist element successfully.
		*/
		auto v = c0.find_value("aaa");
		EXPECT_EQ(*v, "aaa");

		/**
		* @brief Testing if find a not exist element successfully.
		*/
		bool ishasvalue = c1.has_key(10);
		EXPECT_EQ(ishasvalue, false);

		/**
		* @brief Testing if find a not exist element successfully.
		*/
		v = c2.find_value(10.0);
		EXPECT_EQ(v, nullptr);

		/**
		* @brief Testing container's euqlity after find element.
		*/
		EXPECT_EQ(c0.has_equal_size(), true);
		EXPECT_EQ(c1.has_equal_size(), true);
		EXPECT_EQ(c2.has_equal_size(), true);
	}

	/**
	* @brief Testing if Add element successfully.
	*/
	TEST_F(linked_unordered_map_test, AddElement) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Testing if insert a new element successfully.
		*/
		c0.push_back("ddd", "ddd");
		EXPECT_EQ(c0.size(), 4);

		/**
		* @brief Testing if insert a repeat element successfully.
		*/
		c1.push_back(1, "1");
		EXPECT_EQ(c1.size(), 3);

		/**
		* @brief Testing if the insert element written successfully.
		*/
		auto v = c2.find_value(1.0f);
		EXPECT_EQ(*v, "1.0");
		c2.push_back(1.0f, "10.0");
		v = c2.find_value(1.0f);
		EXPECT_EQ(*v, "10.0");

		/**
		* @brief Testing container's euqlity after add element.
		*/
		EXPECT_EQ(c0.has_equal_size(), true);
		EXPECT_EQ(c1.has_equal_size(), true);
		EXPECT_EQ(c2.has_equal_size(), true);
	}

	/**
	* @brief Testing if Iter in correct order successfully.
	*/
	TEST_F(linked_unordered_map_test, IteratorOrder) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Testing the initialized container's order.
		*/
		std::vector<std::string> iterOrder0;
		c0.for_each([&](const std::string& k, const std::string& v) {
			iterOrder0.push_back(k);
			return false;
		});
		EXPECT_EQ(iterOrder0[0], "aaa");
		EXPECT_EQ(iterOrder0[1], "bbb");
		EXPECT_EQ(iterOrder0[2], "ccc");

		/**
		* @brief Testing erase's influence in for_each order.
		*/
		std::vector<int> iterOrder1;
		c1.erase(2);
		c1.for_each([&](const int& k, const std::string& v) {
			iterOrder1.push_back(k);
			return false;
		});
		EXPECT_EQ(iterOrder1[0], 1);
		EXPECT_EQ(iterOrder1[1], 3);

		/**
		* @brief Testing insert's influence in for_each order.
		*/
		std::vector<float> iterOrder2;
		c2.push_back(0.5f, "0.5");
		c2.for_each([&](const float& k, const std::string& v) {
			iterOrder2.push_back(k);
			return false;
		});
		EXPECT_EQ(iterOrder2[0], 1.0f);
		EXPECT_EQ(iterOrder2[1], 2.0f);
		EXPECT_EQ(iterOrder2[2], 3.0f);
		EXPECT_EQ(iterOrder2[3], 0.5f);

		/**
		* @brief Testing container's euqlity after iter.
		*/
		EXPECT_EQ(c0.has_equal_size(), true);
		EXPECT_EQ(c1.has_equal_size(), true);
		EXPECT_EQ(c2.has_equal_size(), true);
	}

	/**
	* @brief Testing if No Linear Finding is successfully.
	*/
	TEST_F(linked_unordered_map_test, NoLinearFind) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Testing prev_value is getted successfully.
		*/
		EXPECT_EQ(c0.prev_value("aaa"), nullptr);
		EXPECT_EQ(*c0.prev_value("bbb"), "aaa");
		EXPECT_EQ(*c0.prev_value("ccc"), "bbb");

		/**
		* @brief Testing next_value is getted successfully.
		*/
		EXPECT_EQ(*c0.next_value("aaa"), "bbb");
		EXPECT_EQ(*c0.next_value("bbb"), "ccc");
		EXPECT_EQ(c0.next_value("ccc"),  nullptr);

		/**
		* @breif Testing first value is getted successfully.
		*/
		scl::linked_unordered_map<std::string, std::string> c3;

		EXPECT_EQ(*c0.first(), "aaa");
		EXPECT_EQ(*c1.first(), "1");
		EXPECT_EQ(*c2.first(), "1.0");
		EXPECT_EQ(c3.first(), nullptr);

		/**
		* @breif Testing end value is getted successfully.
		*/
		EXPECT_EQ(*c0.end(), "ccc");
		EXPECT_EQ(*c1.end(), "3");
		EXPECT_EQ(*c2.end(), "3.0");
		EXPECT_EQ(c3.end(), nullptr);

		/**
		* @breif Testing end key is getted successfully.
		*/
		EXPECT_EQ(*c0.end_k(), "ccc");
		EXPECT_EQ(*c1.end_k(), 3);
		EXPECT_EQ(*c2.end_k(), 3.0);
		EXPECT_EQ(c3.end_k(), nullptr);

		/**
		* @brief Testing container's euqlity after NoLinearFind.
		*/
		EXPECT_EQ(c0.has_equal_size(), true);
		EXPECT_EQ(c1.has_equal_size(), true);
		EXPECT_EQ(c2.has_equal_size(), true);
		EXPECT_EQ(c3.has_equal_size(), true);
	}
}