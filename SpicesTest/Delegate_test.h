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
		void SetUp() override {}

		/**
		* @brief Testing class TearDown function.
		*/
		void TearDown() override {}

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

	}
}
