/**
* @file ClassLibrary_test.h.
* @brief The ClassLibrary_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Delegate/DelegateBasic.h>

namespace SpicesTest {

	/**
	* @brief Test Function Class.
	*/
	class DelegateFuncTest
	{
	public:

		/**
		* @brief Basic Override Class Function.
		* @return Returns true.
		*/
		void Test() 
		{
			std::cout << "Hello  DelegateFuncTest::Test" << std::endl;
		}

		/**
		* @brief Basic Override Class Function.
		* @param[in] f In float.
		*/
		void Test(float f) 
		{
			std::cout << "Hello  DelegateFuncTest::Test(float)" << std::endl;
		}

		/**
		* @brief Basic Class Function.
		* @param[in] a In a.
		* @param[in] b In b.
		*/
		void Test0(int a, int b) 
		{
			std::cout << "Hello  DelegateFuncTest::Test0(int, int)" << " " << a << " " << b << std::endl;
		}

		/**
		* @brief Static Class Function.
		*/
		static void Test1() 
		{
			std::cout << "Hello  DelegateFuncTest::Test1" << std::endl;
		}
	};

	/**
	* @brief Template Function.
	* @tparam RType Return type.
	* @tparam Args any type.
	*/
	template<typename ...Args>
	void DelegateTestT(Args ...args) 
	{
		std::cout << "Hello  DelegateTestT(args...)" << std::endl;
	}

    /**
    * @brief Instance Delegate Class.
    */
    DELEGATE_NONE_PARAM(Test0)
    DELEGATE_ONE_PARAM(Test1, float)
    DELEGATE_TWO_PARAM(Test2, int, int)
    DELEGATE_THREE_PARAM(Test3, glm::vec2, glm::vec2, glm::vec2)
    DELEGATE_FOUR_PARAM(Test4, glm::vec3, glm::vec3, glm::vec3, glm::vec3)
    DELEGATE_FIVE_PARAM(Test5, glm::vec4, glm::vec4, glm::vec4, glm::vec4, glm::vec4)
    DELEGATE_SIX_PARAM(Test6, bool, bool, bool, bool, bool, bool)
    DELEGATE_SEVEN_PARAM(Test7, bool, bool, bool, bool, bool, bool, bool)
    DELEGATE_EIGHT_PARAM(Test8, bool, bool, bool, bool, bool, bool, bool, bool)
    DELEGATE_NINE_PARAM(Test9, bool, bool, bool, bool, bool, bool, bool, bool, bool)

	/**
	* @brief Unit Test for Delegate
	*/
	class Delegate_test : public testing::Test
	{
	protected:

		/**
		* @brief The interface is inherited from testing::Test.
		* Registry on Initialize.
		*/
		void SetUp() override {
			Spices::Log::Init();
		}

		/**
		* @brief Testing class TearDown function.
		*/
		void TearDown() override {
			Spices::Log::ShutDown();
		}

		DelegateTest0 test0;
		DelegateTest1 test1;
		DelegateTest2 test2;
		DelegateTest3 test3;
		DelegateTest4 test4;
		DelegateTest5 test5;
		DelegateTest6 test6;
		DelegateTest7 test7;
		DelegateTest8 test8;
		DelegateTest9 test9;
	};

	/**
	* @brief Testing if bind successfully.
	*/
	TEST_F(Delegate_test, Bind) {
		DelegateFuncTest funcTestClass;

		test0.Bind(std::bind((void(DelegateFuncTest::*)())&DelegateFuncTest::Test, &funcTestClass));                                /* @brief Override Class Function. */
		test1.Bind(std::bind((void(DelegateFuncTest::*)(float))&DelegateFuncTest::Test, &funcTestClass, std::placeholders::_1));    /* @brief Override Class Function. */
		test2.Bind(std::bind(&DelegateFuncTest::Test0, &funcTestClass, std::placeholders::_1, std::placeholders::_2));              /* @brief Class Function.          */
		test0.Bind(std::bind(&DelegateFuncTest::Test1));                                                                            /* @brief Static Class Function.   */

		test0.Bind([&]() { return funcTestClass.Test(); });                                                                         /* @brief Lambda Override Class Function. */
		test1.Bind([&](float f) { return funcTestClass.Test(f); });																    /* @brief Lambda Override Class Function. */
		test2.Bind([&](int a, int b) { return funcTestClass.Test0(a, b); });														/* @brief Lambda Class Function.          */
		test0.Bind([&]() { return DelegateFuncTest::Test1(); });																    /* @brief Lambda Static Class Function.   */
		test0.Bind([&]() { return DelegateTestT(); });										                                        /* @brief Lambda Template Function.       */

		test1.Bind([](bool val) { return val; });                                                                                   /* @brief Lambda Function.         */

		/**
		* @brief Test Size is correct.
		*/
		EXPECT_EQ(test0.size(), 5);
		EXPECT_EQ(test1.size(), 3);
		EXPECT_EQ(test2.size(), 2);
	}

	/**
	* @brief Testing if unbind successfully.
	*/
	TEST_F(Delegate_test, UnBind) {
		DelegateFuncTest funcTestClass;

		test0.Bind(std::bind((void(DelegateFuncTest::*)())&DelegateFuncTest::Test, &funcTestClass));               
		test0.Bind(std::bind(&DelegateFuncTest::Test1));                                                           
		test0.Bind([&]() { return funcTestClass.Test(); });                                                        
		test0.Bind([&]() { return DelegateFuncTest::Test1(); });												
		test0.Bind([&]() { return DelegateTestT(); });										                                           

		EXPECT_EQ(test0.size(), 5);

		test0.UnBind(std::bind((void(DelegateFuncTest::*)()) & DelegateFuncTest::Test, &funcTestClass));
		test0.UnBind(std::bind(&DelegateFuncTest::Test1));
		test0.UnBind([&]() { return funcTestClass.Test(); });
		test0.UnBind([&]() { return DelegateFuncTest::Test1(); });
		test0.UnBind([&]() { return DelegateTestT(); });

		EXPECT_EQ(test0.size(), 3);
	}

	/**
	* @brief Testing if broadcast successfully.
	*/
	TEST_F(Delegate_test, Broadcast) {
		DelegateFuncTest funcTestClass;

		test2.Bind(std::bind(&DelegateFuncTest::Test0, &funcTestClass, std::placeholders::_1, std::placeholders::_2));
		test2.Broadcast(1, 10);

		EXPECT_EQ(test2.size(), 1);
	}
}
