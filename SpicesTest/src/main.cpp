/**
* @file main.cpp.
* @brief The SpicesTest::main Class Implementation.
* @author Spices.
*/

#include <gmock/gmock.h>

/* Container */
#include "Core/Container/linked_unordered_map_test.h"
#include "Core/Container/runtime_memory_block_test.h"

/* Thread */
#include "Core/Thread/ThreadPoolFixed_test.h"
#include "Core/Thread/ThreadPoolCached_test.h"

/* Library */
#include "Core/Library/ClassLibrary_test.h"
#include "Core/Library/FileLibrary_test.h"
#include "Core/Library/MemoryLibrary_test.h"
#include "Core/Library/ProcessLibrary_test.h"
#include "Core/Library/StringLibrary_test.h"

/* Vulkan */
#include "RenderAPI/Vulkan/VulkanImage_test.h"

/**
* @brief The Entry of SpicesTest.
*/
int main(int argc, char** argv)
{
    /**
    * @brief Init GoogleMock.
    */
    testing::InitGoogleMock(&argc, argv);

    /**
    * @brief Execute all unit tests.
    */
    return RUN_ALL_TESTS();
}