#pragma once
#include <gmock/gmock.h>
#include <Core/Container/linked_unordered_map.h>

namespace SpiecsTest {

	/**
	* @brief Unit Test for linked_unordered_map
	*/
	class linked_unordered_map_test : public testing::Test
	{
	protected:

		/**
		* @brief Testing class initialize funtion.
		*/
		void SetUp() override {

			c0.insert("aaa", "aaa");
			c0.insert("bbb", "bbb");
			c0.insert("ccc", "ccc");

			c1.insert(1, "1");
			c1.insert(2, "2");
			c1.insert(3, "3");

			c2.insert(1.0f, "1.0");
			c2.insert(2.0f, "2.0");
			c2.insert(3.0f, "3.0");
		}

		// void TearDown() override {}

		scl::linked_unordered_map<std::string, std::string> c0;
		scl::linked_unordered_map<int, std::string> c1;
		scl::linked_unordered_map<float, std::string> c2;
	};

	/**
	* @brief Testing if initialize successfully.
	*/
	TEST_F(linked_unordered_map_test, Initialize) {
		EXPECT_EQ(c0.size(), 3);
		EXPECT_EQ(c1.size(), 3);
		EXPECT_EQ(c2.size(), 3);
	}

	/**
	* @brief Testing if Remove element successfully.
	*/
	TEST_F(linked_unordered_map_test, RemoveElement) {

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
	}

	/**
	* @brief Testing if Find element successfully.
	*/
	TEST_F(linked_unordered_map_test, AddElement) {

		/**
		* @brief Testing if find a exist element successfully.
		*/
		auto v = c0.find("aaa");
		EXPECT_EQ(v, "aaa");

		/**
		* @brief Testing if find a not exist element successfully.
		*/
		auto v = c1.find(10);
		EXPECT_EQ(v, nullptr);
	}

	/**
	* @brief Testing if Add element successfully.
	*/
	TEST_F(linked_unordered_map_test, AddElement) {

		/**
		* @brief Testing if insert a new element successfully.
		*/
		c0.insert("ddd", "ddd");
		EXPECT_EQ(c0.size(), 4);

		/**
		* @brief Testing if insert a repeat element successfully.
		*/
		c1.insert(1, "1");
		EXPECT_EQ(c1.size(), 3);

		auto v = c2.find(1.0f);
		EXPECT_EQ(v, "1.0");
		c2.insert(1.0f, "10.0");
		v = c2.find(1.0f);
		EXPECT_EQ(v, "10.0");
	}
}