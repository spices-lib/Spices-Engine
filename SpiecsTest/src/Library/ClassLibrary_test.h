#pragma once
#include <gmock/gmock.h>
#include <Core/Library/ClassLibrary.h>

namespace SpiecsTest {

    class TestClass0 {};
    class TestClass1 {};

    TEST(ClassLibraryTest, GetClassString) {
        EXPECT_EQ(Spiecs::ClassLibrary::GetClassString(typeid(TestClass0)), "TestClass0");
        EXPECT_EQ(Spiecs::ClassLibrary::GetClassString(typeid(TestClass1)), "TestClass1");
    }
}