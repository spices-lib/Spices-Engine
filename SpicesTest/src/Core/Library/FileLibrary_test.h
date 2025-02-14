/**
* @file FileLibrary_test.h.
* @brief The FileLibrary_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Library/FileLibrary.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::FileLibrary::Exists.
    */
    TEST(FileLibraryTest, Exists) {

        SPICESTEST_PROFILE_FUNCTION();

        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Exists("premake5.lua"), true);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Exists("vendor/googletest/premake5.lua"), true);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Exists("src/main.cpp"), true);

        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Exists("src/main1.cpp"), false);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Exists("src/main2.cpp"), false);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Exists("src/main3.cpp"), false);
    }

    /**
    * @brief Testing Spices::FileLibrary::Open.
    */
    TEST(FileLibraryTest, Open) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("premake5.lua", Spices::FileModes::FILE_MODE_READ, false, &handle), true);

        EXPECT_EQ(handle.is_valid, true);
        EXPECT_NE(handle.handle, nullptr);
    }

    /**
    * @brief Testing Spices::FileLibrary::Close.
    */
    TEST(FileLibraryTest, Close) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("premake5.lua", Spices::FileModes::FILE_MODE_READ, false, &handle), true);

        EXPECT_EQ(handle.is_valid, true);
        EXPECT_NE(handle.handle, nullptr);

        Spices::FileLibrary::FileLibrary_Close(&handle);
    }

    /**
    * @brief Testing Spices::FileLibrary::Size.
    */
    TEST(FileLibraryTest, Size) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("premake5.lua", Spices::FileModes::FILE_MODE_READ, false, &handle), true);

        EXPECT_EQ(handle.is_valid, true);
        EXPECT_NE(handle.handle, nullptr);

        uint64_t size;
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Size(&handle, &size), true);
        EXPECT_EQ(size, 5767);

        Spices::FileLibrary::FileLibrary_Close(&handle);
    }

    /**
    * @brief Testing Spices::FileLibrary::Read.
    */
    TEST(FileLibraryTest, Read) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("premake5.lua", Spices::FileModes::FILE_MODE_READ, false, &handle), true);

        EXPECT_EQ(handle.is_valid, true);
        EXPECT_NE(handle.handle, nullptr);

        // First Line.
        {
            char data[29];
            uint64_t bytes;
            EXPECT_EQ(Spices::FileLibrary::FileLibrary_Read(&handle, 29, data, &bytes), true);
            EXPECT_EQ(bytes, 29);

            std::string firstLineRead(data, bytes);
            std::string firstLine = "-- @file SpicesTest Premake.\n";

            EXPECT_STREQ(firstLineRead.c_str(), firstLine.c_str());
        }

        // Second Line.
        {
            char data[61];
            uint64_t bytes;
            EXPECT_EQ(Spices::FileLibrary::FileLibrary_Read(&handle, 61, data, &bytes), true);
            EXPECT_EQ(bytes, 61);

            std::string firstLineRead(data, bytes);
            std::string firstLine = "-- @brief Defines details of the UintTest Solution Building.\n";

            EXPECT_STREQ(firstLineRead.c_str(), firstLine.c_str());
        }

        // Third Line.
        {
            char data[32];
            uint64_t bytes;
            EXPECT_EQ(Spices::FileLibrary::FileLibrary_Read(&handle, 32, data, &bytes), true);
            EXPECT_EQ(bytes, 32);

            std::string firstLineRead(data, bytes);
            std::string firstLine = "-- @author The Cherno & Spices.\n";

            EXPECT_STREQ(firstLineRead.c_str(), firstLine.c_str());
        }

        Spices::FileLibrary::FileLibrary_Close(&handle);
    }

    /**
    * @brief Testing Spices::FileLibrary::Read_Line.
    */
    TEST(FileLibraryTest, Read_Line) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("premake5.lua", Spices::FileModes::FILE_MODE_READ, false, &handle), true);

        EXPECT_EQ(handle.is_valid, true);
        EXPECT_NE(handle.handle, nullptr);

        // First Line.
        {
            char data[100];
            uint64_t bytes;
            EXPECT_EQ(Spices::FileLibrary::FileLibrary_Read_Line(&handle, 1000, data, &bytes), true);
            EXPECT_EQ(bytes, 29);

            std::string firstLineRead(data, bytes);
            std::string firstLine = "-- @file SpicesTest Premake.\n";

            EXPECT_STREQ(firstLineRead.c_str(), firstLine.c_str());
        }

        // Second Line.
        {
            char data[100];
            uint64_t bytes;
            EXPECT_EQ(Spices::FileLibrary::FileLibrary_Read_Line(&handle, 1000, data, &bytes), true);
            EXPECT_EQ(bytes, 61);

            std::string firstLineRead(data, bytes);
            std::string firstLine = "-- @brief Defines details of the UintTest Solution Building.\n";

            EXPECT_STREQ(firstLineRead.c_str(), firstLine.c_str());
        }

        // Third Line.
        {
            char data[100];
            uint64_t bytes;
            EXPECT_EQ(Spices::FileLibrary::FileLibrary_Read_Line(&handle, 1000, data, &bytes), true);
            EXPECT_EQ(bytes, 32);

            std::string firstLineRead(data, bytes);
            std::string firstLine = "-- @author The Cherno & Spices.\n";

            EXPECT_STREQ(firstLineRead.c_str(), firstLine.c_str());
        }

        Spices::FileLibrary::FileLibrary_Close(&handle);
    }

    /**
    * @brief Testing Spices::FileLibrary::Read_all_bytes.
    */
    TEST(FileLibraryTest, Read_all_bytes) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("premake5.lua", Spices::FileModes::FILE_MODE_READ, false, &handle), true);

        EXPECT_EQ(handle.is_valid, true);
        EXPECT_NE(handle.handle, nullptr);

        {
            char data[6000];
            uint64_t bytes;

            /**
            * @todo Returns false here, but not known why?
            */
            Spices::FileLibrary::FileLibrary_Read_all_bytes(&handle, data, &bytes);
        }

        Spices::FileLibrary::FileLibrary_Close(&handle);
    }

    /**
    * @brief Testing Spices::FileLibrary::Write.
    */
    TEST(FileLibraryTest, Write) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("Write.txt", Spices::FileModes::FILE_MODE_WRITE, false, &handle), true);

        EXPECT_EQ(handle.is_valid, true);
        EXPECT_NE(handle.handle, nullptr);

        {
            std::string data = "First Line.";
            uint64_t bytes;
            EXPECT_EQ(Spices::FileLibrary::FileLibrary_Write(&handle, data.size(), data.c_str(), &bytes), true);

            EXPECT_EQ(bytes, data.size());
        }

        Spices::FileLibrary::FileLibrary_Close(&handle);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Delete("Write.txt"), true);
    }

    /**
    * @brief Testing Spices::FileLibrary::Delete.
    */
    TEST(FileLibraryTest, Delete) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("Write.txt", Spices::FileModes::FILE_MODE_WRITE, false, &handle), true);

        Spices::FileLibrary::FileLibrary_Close(&handle);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Delete("Write.txt"), true);
    }

    /**
    * @brief Testing Spices::FileLibrary::Write_Line.
    */
    TEST(FileLibraryTest, Write_Line) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("Write.txt", Spices::FileModes::FILE_MODE_WRITE, false, &handle), true);

        EXPECT_EQ(handle.is_valid, true);
        EXPECT_NE(handle.handle, nullptr);

        {
            const std::string data = "First Line.";
            uint64_t bytes;
            EXPECT_EQ(Spices::FileLibrary::FileLibrary_Write_Line(&handle, data.c_str()), true);
        }

        Spices::FileLibrary::FileLibrary_Close(&handle);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Delete("Write.txt"), true);
    }

    /**
    * @brief Testing Spices::FileLibrary::CopyFile.
    */
    TEST(FileLibraryTest, CopyFile) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::FileHandle handle{};
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Open("Write.txt", Spices::FileModes::FILE_MODE_WRITE, false, &handle), true);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_CopyFile("Write.txt", "Write1.txt"), true);

        Spices::FileLibrary::FileLibrary_Close(&handle);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Delete("Write.txt"), true);
        EXPECT_EQ(Spices::FileLibrary::FileLibrary_Delete("Write1.txt"), true);
    }
}