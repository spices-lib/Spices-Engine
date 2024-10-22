/**
* @file main.cpp.
* @brief The SpicesTest::main Class Implementation.
* @author Spices.
*/

#include <gmock/gmock.h>
#include "Instrumentor.h"

/* Container */
#include "Core/Container/DirectedAcyclicGraph_test.h"
#include "Core/Container/KDTree_test.h"
#include "Core/Container/LinkedUnorderedMap_test.h"
#include "Core/Container/RuntimeMemoryBlock_test.h"
#include "Core/Container/Tuple_test.h"

/* Thread */
//#include "Core/Thread/ThreadPoolFixed_test.h"
//#include "Core/Thread/ThreadPoolCached_test.h"

/* Library */
#include "Core/Library/ClassLibrary_test.h"
#include "Core/Library/FileLibrary_test.h"
#include "Core/Library/MemoryLibrary_test.h"
#include "Core/Library/ProcessLibrary_test.h"
#include "Core/Library/StringLibrary_test.h"

/* Delegate */
#include "Core/Delegate/Delegate_test.h"

/* Reflect */
#include "Core/Reflect/StaticReflect/VariableTraits_test.h"
#include "Core/Reflect/StaticReflect/FunctionTraits_test.h"
#include "Core/Reflect/StaticReflect/TypeList_test.h"
#include "Core/Reflect/StaticReflect/FieldTraits_test.h"
#include "Core/Reflect/StaticReflect/StaticReflectClass_test.h"

/* Vulkan */
//#include "RenderAPI/Vulkan/VulkanImage_test.h"

/**
* @brief The Entry of SpicesTest.
*/
int main(int argc, char** argv)
{
    SPICESTEST_PROFILE_BEGIN_SESSION("SpicesTestInstruments", "SpiceslProfile-SpicesTest.json");

    /**
    * @brief Init GoogleMock.
    */
    testing::InitGoogleMock(&argc, argv);

    /**
    * @brief Execute all unit tests.
    */
    int result = RUN_ALL_TESTS();

    SPICESTEST_PROFILE_END_SESSION();

    return result;
}