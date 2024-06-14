/**
* @file main.h.
* @brief The SpiecsTest::main Class Implementation.
* @author Spiecs.
*/

#include <gmock/gmock.h>

#include "Container/linked_unordered_map_test.h"
#include "Container/runtime_memory_block_test.h"
#include "Library/ClassLibrary_test.h"
#include "RenderAPI/Vulkan/VulkanImage_test.h"

/**
* @brief The Entry of SpiecsTest.
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