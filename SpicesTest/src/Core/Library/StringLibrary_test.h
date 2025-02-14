/**
* @file StringLibrary_test.h.
* @brief The StringLibrary_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Library/StringLibrary.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::StringLibrary::SplitString.
    */
    TEST(StringLibraryTest, SplitString) {

        SPICESTEST_PROFILE_FUNCTION();

        std::string s = "Hello,my frient,this is a good day.";
        auto result = Spices::StringLibrary::SplitString(s, ',');

        EXPECT_EQ(result.size(), 3);

        EXPECT_STREQ(result[0].c_str(), "Hello");
        EXPECT_STREQ(result[1].c_str(), "my frient");
        EXPECT_STREQ(result[2].c_str(), "this is a good day.");
    }

    /**
    * @brief Testing Spices::StringLibrary::StringsEqual.
    */
    TEST(StringLibraryTest, StringsEqual) {

        SPICESTEST_PROFILE_FUNCTION();

        EXPECT_EQ(Spices::StringLibrary::StringsEqual("Hello", "Hello"), true);
        EXPECT_EQ(Spices::StringLibrary::StringsEqual("Hello1", "Hello1"), true);
        EXPECT_NE(Spices::StringLibrary::StringsEqual("Hello1", "Hello2"), true);
    }
}