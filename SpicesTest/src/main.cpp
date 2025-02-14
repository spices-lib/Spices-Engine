/**
* @file main.cpp.
* @brief The SpicesTest::main Class Implementation.
* @author Spices.
*/

#include <gmock/gmock.h>
#include "Instrumentor.h"

/* Container */
//#include "Core/Container/DirectedAcyclicGraph_test.h"
//#include "Core/Container/KDTree_test.h"
//#include "Core/Container/LinkedUnorderedMap_test.h"
//#include "Core/Container/RuntimeMemoryBlock_test.h"
//#include "Core/Container/Tuple_test.h"
//#include "Core/Container/FreeList_test.h"
//#include "Core/Container/SpanList_test.h"
//#include "Core/Container/RadixTrie_test.h"
//#include "Core/Container/Vector_test.h"
//#include "Core/Container/BehaveStateList_test.h"
//#include "Core/Container/Tree_test.h"
//
///* Delegate */
//#include "Core/Delegate/Delegate_test.h"
//
///* Library */
//#include "Core/Library/ClassLibrary_test.h"
//#include "Core/Library/FileLibrary_test.h"
#include "Core/Library/MemoryLibrary_test.h"
//#include "Core/Library/ProcessLibrary_test.h"
//#include "Core/Library/StringLibrary_test.h"
//
///* Math */
//#include "Core/Math/Math_test.h"
//
///* Memory */
//#include "Core/Memory/ObjectPool_test.h"
//#include "Core/Memory/ThreadCache_test.h"
//#include "Core/Memory/PageCache_test.h"
//#include "Core/Memory/CentralCache_test.h"
//#include "Core/Memory/MemoryPool_test.h"
//
///* Reflect */
//#include "Core/Reflect/StaticReflect/VariableTraits_test.h"
//#include "Core/Reflect/StaticReflect/FunctionTraits_test.h"
//#include "Core/Reflect/StaticReflect/TypeList_test.h"
//#include "Core/Reflect/StaticReflect/FieldTraits_test.h"
//#include "Core/Reflect/StaticReflect/ClassTraits_test.h"
//#include "Core/Reflect/StaticReflect/IsConst_test.h"
//#include "Core/Reflect/StaticReflect/RemovePointer_test.h"
//#include "Core/Reflect/StaticReflect/IsPointer_test.h"
//
///* Thread */
//#include "Core/Thread/ThreadPoolFixed_test.h"
//#include "Core/Thread/ThreadPoolCached_test.h"
//#include "Core/Thread/Semaphore_test.h"
//
///* Timer */
//#include "Core/Timer/ScopeTimer_test.h"
//
///* NetWork */
//#include "Network/Buffer_test.h"
//#include "Network/EPollPoller_test.h"
//#include "Network/InetAddress_test.h"
//#include "Network/Socket_test.h"
//#include "Network/TcpServer_test.h"

/* Vulkan */
//#include "RenderAPI/Vulkan/VulkanImage_test.h"

/**
* @brief The Entry of SpicesTest.
*/
int main(int argc, char** argv)
{
    try
    {
        SPICESTEST_PROFILE_BEGIN_SESSION("SpicesTestInstruments", "SpiceslProfile-SpicesTest.json");

        /**
        * @brief Init GoogleMock.
        */
        testing::InitGoogleMock(&argc, argv);

        /**
        * @brief Execute all unit tests.
        */
        const int result = RUN_ALL_TESTS();

        SPICESTEST_PROFILE_END_SESSION();
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}