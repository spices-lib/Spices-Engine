#include <iostream>
#include <gmock/gmock.h>

#include "Container/linked_unordered_map_test.h"
#include "Container/runtime_memory_block_test.h"
#include "Library/ClassLibrary_test.h"

int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}