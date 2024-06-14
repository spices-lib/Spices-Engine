/**
* @file ClassLibrary_test.h.
* @brief The ClassLibrary_test Definitions.
* @author Spiecs.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Library/ClassLibrary.h>

namespace SpiecsTest {

    /**
    * @brief Declear Empty Class.
    */
    class TestClass0 {};
    class TestClass1 {};

    /**
    * @brief Testing Spiecs::ClassLibrary::GetClassString.
    */
    TEST(ClassLibraryTest, GetClassString) {
        EXPECT_EQ(Spiecs::ClassLibrary::GetClassString(typeid(TestClass0)), "TestClass0");
        EXPECT_EQ(Spiecs::ClassLibrary::GetClassString(typeid(TestClass1)), "TestClass1");
    }
}