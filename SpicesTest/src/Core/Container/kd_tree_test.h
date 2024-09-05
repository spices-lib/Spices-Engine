/**
* @file kd_tree_test.h.
* @brief The kd_tree_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/kd_tree.h>

namespace SpicesTest {

	/**
	* @brief The interface is inherited from testing::Test.
	* Registy on Initialize.
	*/
	class kd_tree_test : public testing::Test
	{
	protected:

		/**
		* @brief Testing class initialize function.
		*/
		void SetUp() override {
		
			/**
			* @brief Insert points into the kd_tree.
			*/
			m_KDTree.insert({ 3, 6 });
			m_KDTree.insert({ 2, 2 });
			m_KDTree.insert({ 4, 7 });
			m_KDTree.insert({ 1, 3 });
			m_KDTree.insert({ 2, 4 });
			m_KDTree.insert({ 5, 4 });
			m_KDTree.insert({ 7, 2 });

			m_KDTree.print();
		}

		/**
		* @brief Testing class TearDown function.
		*/
		void TearDown() override {}

		/**
		* @brief Create a KDTree with 2 dimensions.
		*/
		scl::kd_tree<2> m_KDTree;
	};

	/**
	* @brief Testing if Insert successfully.
	*/
	TEST_F(kd_tree_test, Insert) {

		m_KDTree.insert({ 3, 6 + 1 });
		m_KDTree.insert({ 2, 2 + 1 });
		m_KDTree.insert({ 4, 7 + 1 });
		m_KDTree.insert({ 1, 3 + 1 });
		m_KDTree.insert({ 2, 4 + 1 });
		m_KDTree.insert({ 5, 4 + 1 });
		m_KDTree.insert({ 7, 2 + 1 });

		m_KDTree.print();
	}

	/**
	* @brief Testing if Search successfully.
	*/
	TEST_F(kd_tree_test, Search) {

		/**
		* @brief Search for specific points in the kd_tree.
		*/
		EXPECT_EQ(m_KDTree.search({ 3, 6 }), true);
		EXPECT_EQ(m_KDTree.search({ 2, 2 }), true);
		EXPECT_EQ(m_KDTree.search({ 4, 7 }), true);
		EXPECT_EQ(m_KDTree.search({ 1, 3 }), true);
		EXPECT_EQ(m_KDTree.search({ 2, 4 }), true);
		EXPECT_EQ(m_KDTree.search({ 5, 4 }), true);
		EXPECT_EQ(m_KDTree.search({ 7, 2 }), true);

		/**
		* @brief Search for specific points not in the kd_tree.
		*/
		EXPECT_EQ(m_KDTree.search({ 3 + 1, 6 }), false);
		EXPECT_EQ(m_KDTree.search({ 2 + 1, 2 }), false);
		EXPECT_EQ(m_KDTree.search({ 4 + 1, 7 }), false);
		EXPECT_EQ(m_KDTree.search({ 1 + 1, 3 }), false);
		EXPECT_EQ(m_KDTree.search({ 2 + 1, 4 }), false);
		EXPECT_EQ(m_KDTree.search({ 5 + 1, 4 }), false);
		EXPECT_EQ(m_KDTree.search({ 7 + 1, 2 }), false);
	}

	/**
	* @brief Testing if NearestNeighbourSearch successfully.
	*/
	TEST_F(kd_tree_test, NearestNeighbourSearch) {

		/**
		* @brief Search for specific points.
		*/
		scl::kd_tree<2>::item val = { 1, 3 };

		EXPECT_EQ(m_KDTree.nearest_neighbour_search({ 0, 0 }, { 3, 3 }), val);
	}

	/**
	* @brief Testing if RangeSearch successfully.
	*/
	TEST_F(kd_tree_test, RangeSearch) {

		/**
		* @brief Search for specific points.
		*/
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 1, 1 }).size(), 0);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 2, 2 }).size(), 1);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 3, 3 }).size(), 2);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 4, 4 }).size(), 3);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 5, 5 }).size(), 4);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 6, 6 }).size(), 5);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 7, 7 }).size(), 7);

		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 1, 100 }).size(), 1);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 2, 100 }).size(), 3);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 3, 100 }).size(), 4);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 4, 100 }).size(), 5);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 5, 100 }).size(), 6);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 6, 100 }).size(), 6);
		EXPECT_EQ(m_KDTree.range_search({ 0, 0 }, { 7, 100 }).size(), 7);
	}
}