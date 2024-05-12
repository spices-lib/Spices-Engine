#include <iostream>
#include <gmock/gmock.h>
#include "linked_unordered_map_test.h"

int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}