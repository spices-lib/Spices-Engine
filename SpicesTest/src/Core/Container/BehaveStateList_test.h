/**
* @file BehaveStateList_test.h.
* @brief The BehaveStateList_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/BehaveStateList.h>
#include "Instrumentor.h"

namespace SpicesTest {

	/**
	* @brief Testing scl::behave_state_list.
	*/
	TEST(behave_state_list_test, All) {

		SPICESTEST_PROFILE_FUNCTION();

		scl::behave_state_list<void> list;

		{
			const auto node = list.AddNode();

			node->PushBehave("B", []() {
				std::cout << "BO" << std::endl;
			});
		}

		{
			const auto node = list.AddNode();

			node->PushBehave("B", []() {
				std::cout << "B1" << std::endl;
			});
		}

		{
			const auto node = list.AddNode();

			node->PushBehave("B", []() {
				std::cout << "B2" << std::endl;
			});
		}

		for (int i = 0; i < 3; i++)
		{
			const auto node = list.GetState();

			node->ExecuteBehave("B");

			list.IncreateState();
		}
		
	}
}