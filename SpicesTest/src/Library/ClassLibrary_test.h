/**
* @file ClassLibrary_test.h.
* @brief The ClassLibrary_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Library/ClassLibrary.h>

namespace SpicesTest {

    /**
    * @brief Declear Empty Class.
    */
    class TestClass0 {};
    class TestClass1 {};

    /**
    * @brief Testing Spices::ClassLibrary::GetClassString.
    */
    TEST(ClassLibraryTest, GetClassString) {
        EXPECT_EQ(Spices::ClassLibrary::GetClassString(typeid(TestClass0)), "TestClass0");
        EXPECT_EQ(Spices::ClassLibrary::GetClassString(typeid(TestClass1)), "TestClass1");
    }
}