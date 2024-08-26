/**
* @file directed_acyclic_graph_test.h.
* @brief The directed_acyclic_graph_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/directed_acyclic_graph.h>

namespace SpicesTest {

	/**
	* @brief Test Function Class.
	*/
	class directed_acyclic_graphFuncTest
	{
	public:

		/**
		* @brief Basic Override Class Function.
		* @return Returns true.
		*/
		void Test()
		{
			std::cout << "Hello directed_acyclic_graphFuncTest::Test  2" << std::endl;
		}

		/**
		* @brief Basic Override Class Function.
		* @param[in] f In float.
		*/
		void Test(float f)
		{
			std::cout << "Hello directed_acyclic_graphFuncTest::Test(float)  3" << std::endl;
		}

		/**
		* @brief Static Class Function.
		*/
		static void Test1()
		{
			std::cout << "Hello directed_acyclic_graphFuncTest::Test1  1 " << std::endl;
		}
	};

	/**
	* @brief The interface is inherited from testing::Test.
	* Registy on Initialize.
	*/
	class directed_acyclic_graph_test : public testing::Test
	{
	protected:

		/**
		* @brief Testing class initialize function.
		*/
		void SetUp() override {}

		/**
		* @brief Testing class TearDown function.
		*/
		void TearDown() override {}

		std::vector<scl::directed_acyclic_node> m_Nodes;

		scl::directed_acyclic_graph m_DAG;
	};

	/**
	* @brief Testing if Addnode successfully.
	*/
	TEST_F(directed_acyclic_graph_test, Addnode) {
		directed_acyclic_graphFuncTest funcTestClass;

		m_Nodes.push_back({ "A", {"C"}, std::bind((void(directed_acyclic_graphFuncTest::*)()) & directed_acyclic_graphFuncTest::Test, &funcTestClass)});               /* @brief Override Class Function. */
		m_Nodes.push_back({ "B", {"A"}, std::bind((void(directed_acyclic_graphFuncTest::*)(float)) & directed_acyclic_graphFuncTest::Test, &funcTestClass, 1.0f)});    /* @brief Override Class Function. */
		m_Nodes.push_back({ "C", {}, std::bind(&directed_acyclic_graphFuncTest::Test1)});                                                                              /* @brief Static Class Function.   */

		for (int i = 0; i < m_Nodes.size(); i++)
		{
			m_DAG.add_node(&m_Nodes[i]);
		}

		/**
		* @brief Testing initialized container's size.
		*/
		EXPECT_EQ(m_Nodes.size(), 3);
		EXPECT_EQ(m_DAG.size(), 3);
	}

	/**
	* @brief Testing if Execute successfully.
	*/
	TEST_F(directed_acyclic_graph_test, Execute) {
		directed_acyclic_graphFuncTest funcTestClass;

		m_Nodes.push_back({ "A", {"C"}, std::bind((void(directed_acyclic_graphFuncTest::*)()) & directed_acyclic_graphFuncTest::Test, &funcTestClass) });               /* @brief Override Class Function. */
		m_Nodes.push_back({ "B", {"A"}, std::bind((void(directed_acyclic_graphFuncTest::*)(float)) & directed_acyclic_graphFuncTest::Test, &funcTestClass, 1.0f) });    /* @brief Override Class Function. */
		m_Nodes.push_back({ "C", {}, std::bind(&directed_acyclic_graphFuncTest::Test1) });                                                                              /* @brief Static Class Function.   */

		for (int i = 0; i < m_Nodes.size(); i++)
		{
			m_DAG.add_node(&m_Nodes[i]);
		}

		m_DAG.execute();

		/**
		* @brief Testing initialized container's size.
		*/
		EXPECT_EQ(m_Nodes.size(), 3);
	}
}