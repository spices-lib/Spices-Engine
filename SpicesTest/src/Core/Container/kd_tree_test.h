/**
* @file kd_tree_test.h.
* @brief The kd_tree_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/kd_tree.h>
#include <Core/Thread/ThreadPool.h>
#include <random>
#include "Instrumentor.h"

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
			
			SPICESTEST_PROFILE_FUNCTION();

			/**
			* @brief ThreadPool for build KDTree.
			*/
			std::vector<scl::kd_tree<2>::item> points;
			points.push_back({ 3.0, 6.0 });
			points.push_back({ 2.0, 2.0 });
			points.push_back({ 4.0, 7.0 });
			points.push_back({ 1.0, 3.0 });
			points.push_back({ 2.0, 4.0 });
			points.push_back({ 5.0, 4.0 });
			points.push_back({ 7.0, 2.0 });

			/**
			* @brief Insert points into the kd_tree.
			*/
			m_KDTree.insert(points);
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

		SPICESTEST_PROFILE_FUNCTION();

		EXPECT_EQ(m_KDTree.size(), 7);
		m_KDTree.print();
	}

	/**
	* @brief Testing if Search successfully.
	*/
	TEST_F(kd_tree_test, Search) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Search for specific points in the kd_tree.
		*/
		EXPECT_EQ(m_KDTree.search({ 3.0, 6.0 }), true);
		EXPECT_EQ(m_KDTree.search({ 2.0, 2.0 }), true);
		EXPECT_EQ(m_KDTree.search({ 4.0, 7.0 }), true);
		EXPECT_EQ(m_KDTree.search({ 1.0, 3.0 }), true);
		EXPECT_EQ(m_KDTree.search({ 2.0, 4.0 }), true);
		EXPECT_EQ(m_KDTree.search({ 5.0, 4.0 }), true);
		EXPECT_EQ(m_KDTree.search({ 7.0, 2.0 }), true);

		/**
		* @brief Search for specific points not in the kd_tree.
		*/
		EXPECT_EQ(m_KDTree.search({ 3.0 + 1, 6.0 }), false);
		EXPECT_EQ(m_KDTree.search({ 2.0 + 1, 2.0 }), false);
		EXPECT_EQ(m_KDTree.search({ 4.0 + 1, 7.0 }), false);
		EXPECT_EQ(m_KDTree.search({ 1.0 + 1, 3.0 }), false);
		EXPECT_EQ(m_KDTree.search({ 2.0 + 1, 4.0 }), false);
		EXPECT_EQ(m_KDTree.search({ 5.0 + 1, 4.0 }), false);
		EXPECT_EQ(m_KDTree.search({ 7.0 + 1, 2.0 }), false);
	}

	/**
	* @brief Testing if NearestNeighbourSearch successfully.
	*/
	TEST_F(kd_tree_test, NearestNeighbourSearch) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Search for specific points.
		*/
		scl::kd_tree<2>::item val = { 2, 2 };

		EXPECT_EQ(m_KDTree.nearest_neighbour_search({ 2.0, 0.0 }, { 3.0, 3.0 }), val);

		const int nPoints = 10000000;
		scl::kd_tree<3> modelKDTree;
		std::vector<scl::kd_tree<3>::item> points;
		scl::kd_tree<3>::item findVal = { 50.2f, 87.3f, 12.6f };

		{
			SPICESTEST_PROFILE_SCOPE("Create Points Collection.");

			points.resize(nPoints);
			for (int i = 0; i < nPoints; i++)
			{
				points[i][0] = std::rand() / float(RAND_MAX) * 100.0f;
				points[i][1] = std::rand() / float(RAND_MAX) * 100.0f;
				points[i][2] = std::rand() / float(RAND_MAX) * 100.0f;
			}
		}

		{
			SPICESTEST_PROFILE_SCOPE("Insert to KDTree");

			Spices::ThreadPool threadPool;
			threadPool.SetMode(Spices::PoolMode::MODE_FIXED);
			threadPool.Start(10);
			modelKDTree.insert_async(points, &threadPool);
			threadPool.Wait();
			EXPECT_EQ(modelKDTree.size(), nPoints);
		}

		{
			SPICESTEST_PROFILE_SCOPE("Search in KDTree");

			modelKDTree.nearest_neighbour_search(findVal, { 0.1f, 0.1f, 0.1f });
		}

		{
			SPICESTEST_PROFILE_SCOPE("Search in Loop");

			scl::kd_tree<3>::item nearPt = { 1E11 };
			for (int i = 0; i < nPoints; i++)
			{
				if (points[i][0] < nearPt[0] && points[i][1] < nearPt[1] && points[i][2] < nearPt[2])
				{
					nearPt = points[i];
				}
			}
		}
	}

	/**
	* @brief Testing if RangeSearch successfully.
	*/
	TEST_F(kd_tree_test, RangeSearch) {

		SPICESTEST_PROFILE_FUNCTION();

		/**
		* @brief Search for specific points.
		*/
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 1.0, 1.0 }).size(), 0);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 2.0, 2.0 }).size(), 1);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 3.0, 3.0 }).size(), 2);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 4.0, 4.0 }).size(), 3);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 5.0, 5.0 }).size(), 4);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 6.0, 6.0 }).size(), 5);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 7.0, 7.0 }).size(), 7);

		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 1.0, 100.0 }).size(), 1);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 2.0, 100.0 }).size(), 3);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 3.0, 100.0 }).size(), 4);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 4.0, 100.0 }).size(), 5);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 5.0, 100.0 }).size(), 6);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 6.0, 100.0 }).size(), 6);
		EXPECT_EQ(m_KDTree.range_search({ 0.0, 0.0 }, { 7.0, 100.0 }).size(), 7);
	}
}