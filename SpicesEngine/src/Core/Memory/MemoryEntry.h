#pragma once
#include "Core/Memory/MemoryPool.h"

namespace Spices {
    
/**
* @brief Use MemoryPool for memories management. 
*/
#define ENABLE_MAIN_MEMORY_POOL 1
    
    /**
    * @brief Process State Enum Definitions.
    */
    enum class ProcessState
    {
        BeforeEntry = 0,  // before entry
        Run         = 1,  // run
        Exit        = 2   // exit
    };

    class MemoryEntry
    {
    public:

        /**
        * @brief Get ProcessState Instance.
        * @return Returns ProcessState Instance.
        */
        static ProcessState GetProcessState()
        {
            return processState;
        }

        /**
        * @brief Set ProcessState Instance.
        * @param[in] state ProcessState Instance.
        */
        static void SetProcessState(ProcessState state)
        {
            processState = state;
        }

        /**
        * @brief malloc memory from MemoryPool.
        * @param[in] size bytes.
        * @return Return memory pointer.
        */
        static void* mallocFromPool(size_t size)
        {
            void* ptr = Spices::MemoryPool::Alloc(size);

            SPICES_PROFILE_ALLOC_N(ptr, size, Spices::memoryPoolNames[2]);

            return std::move(ptr);
        }

        /**
        * @brief malloc memory from OS.
        * @param[in] size bytes.
        * @return Return memory pointer.
        */
        static void* mallocFromOS(size_t size)
        {
            void* ptr = malloc(size);

            SPICES_PROFILE_ALLOC_N(ptr, size, Spices::memoryPoolNames[0]);

            return std::move(ptr);
        }

        /**
        * @brief malloc memory from OS aligned version.
        * @param[in] size bytes.
        * @param[in] align aligned bytes.
        * @return Return memory pointer.
        */
        static void* mallocFromOS_Aligned(size_t size, std::align_val_t align)
        {
            void* ptr = _aligned_malloc(size, static_cast<size_t>(align));

            SPICES_PROFILE_ALLOC_N(ptr, size, Spices::memoryPoolNames[0]);

            return std::move(ptr);
        }

        /**
        * @brief free memory to MemoryPool.
        * @param[in] ptr memory pointer.
        */
        static void freeToPool(void* ptr)
        {
            SPICES_PROFILE_FREE_N(ptr, Spices::memoryPoolNames[2]);

            Spices::MemoryPool::Free(ptr);
        }

        /**
        * @brief free memory to OS.
        * @param[in] ptr memory pointer.
        */
        static void freeToOS(void* ptr)
        {
            SPICES_PROFILE_FREE_N(ptr, Spices::memoryPoolNames[0]);

            free(ptr);
        }

        /**
        * @brief free memory to OS aligned version.
        * @param[in] ptr memory pointer.
        * @param[in] align aligned bytes.
        */
        static void freeToOS_Aligned(void* ptr, std::align_val_t align)
        {
            SPICES_PROFILE_FREE_N(ptr, Spices::memoryPoolNames[0]);

            _aligned_free(ptr);
        }

    private:

        /**
        * @brief Single Instance of ProcessState
        */
        static ProcessState processState;
    };

    ProcessState MemoryEntry::processState = ProcessState::BeforeEntry;
}

/**
* @brief Macros of modify Process instance state.
*/
#define PROCESS_INSTANCE_ENTRY  { Spices::MemoryEntry::SetProcessState(Spices::ProcessState::Run); }
#define PROCESS_INSTANCE_EXIT   { Spices::MemoryEntry::SetProcessState(Spices::ProcessState::Exit); }

/******************************************* override new delete ***********************************************************/

#if ENABLE_MAIN_MEMORY_POOL

/**
* @brief override new operation.
* @param[in] size memory bytes.
* @return Returns memory pointer.
*/
void* operator new(size_t size)
{
    switch (Spices::MemoryEntry::GetProcessState())
    {
        /**
        * @brief allocate memory using MemoryPool if is initialized.
        */
        case Spices::ProcessState::Run:
        {
            return Spices::MemoryEntry::mallocFromPool(size);
        }

        /**
        * @brief allocate memory using malloc before MemoryPool is initialized or already exit.
        */
        case Spices::ProcessState::BeforeEntry:
        case Spices::ProcessState::Exit:
        default:
        {
            return Spices::MemoryEntry::mallocFromOS(size);
        }
    }
}

/**
* @brief override new[] operation.
* @param[in] size memory bytes.
* @return Returns memory pointer.
*/
void* operator new[](size_t size)
{
   switch (Spices::MemoryEntry::GetProcessState())
    {
        /**
        * @brief allocate memory using MemoryPool if is initialized.
        */
        case Spices::ProcessState::Run:
        {
            return Spices::MemoryEntry::mallocFromPool(size);
        }

        /**
        * @brief allocate memory using malloc before MemoryPool is initialized or already exit.
        */
        case Spices::ProcessState::BeforeEntry:
        case Spices::ProcessState::Exit:
        default:
        {
            return Spices::MemoryEntry::mallocFromOS(size);
        }
    }
}

/**
* @brief override delete operator.
* @param[in] ptr memory pointer.
*/
void operator delete(void* ptr) noexcept
{
    switch(Spices::MemoryEntry::GetProcessState())
    {
        /**
        * @brief free memory using free before MemoryPool is initialized.
        */
        case Spices::ProcessState::BeforeEntry:
        {
            Spices::MemoryEntry::freeToOS(ptr);
            return;
        }

        /**
        * @brief free memory using MemoryPool if is initialized.
        */
        case Spices::ProcessState::Run:
        {
            Spices::MemoryEntry::freeToPool(ptr);
            return;
        }

        /**
        * @brief Do nothing while exist program.
        */
        case Spices::ProcessState::Exit:
        {
            return;
        }

        /**
        * @brief free memory using free default.
        */
        default:
        {
            Spices::MemoryEntry::freeToOS(ptr);
            return;
        }
    }
}

/**
* @brief override delete[] operator.
* @param[in] ptr memory pointer.
*/
void operator delete[](void* ptr) noexcept
{
    switch(Spices::MemoryEntry::GetProcessState())
    {
        /**
        * @brief free memory using free before MemoryPool is initialized.
        */
        case Spices::ProcessState::BeforeEntry:
        {
            Spices::MemoryEntry::freeToOS(ptr);
            return;
        }

        /**
        * @brief free memory using MemoryPool if is initialized.
        */
        case Spices::ProcessState::Run:
        {
            Spices::MemoryEntry::freeToPool(ptr);
            return;
        }

        /**
        * @brief Do nothing while exist program.
        */
        case Spices::ProcessState::Exit:
        {
            return;
        }

        /**
        * @brief free memory using free default.
        */
        default:
        {
            Spices::MemoryEntry::freeToOS(ptr);
            return;
        }
    }
}







/**
* @brief override new operation.
* @param[in] size memory bytes.
* @param[in] align aligned bytes.
* @return Returns memory pointer.
*/
void* operator new(size_t size, std::align_val_t align)
{
    switch (Spices::MemoryEntry::GetProcessState())
    {
        /**
        * @brief allocate memory using MemoryPool if is initialized.
        */
        case Spices::ProcessState::Run:
        {
            return Spices::MemoryEntry::mallocFromPool(static_cast<size_t>(align));
        }

        /**
        * @brief allocate memory using malloc before MemoryPool is initialized or already exit.
        */
        case Spices::ProcessState::BeforeEntry:
        case Spices::ProcessState::Exit:
        default:
        {
            return Spices::MemoryEntry::mallocFromOS_Aligned(size, align);
        }
    }
}

/**
* @brief override new[] operation.
* @param[in] size memory bytes.
* @param[in] align aligned bytes.
* @return Returns memory pointer.
*/
void* operator new[](size_t size, std::align_val_t align)
{
    switch (Spices::MemoryEntry::GetProcessState())
    {
        /**
        * @brief allocate memory using MemoryPool if is initialized.
        */
        case Spices::ProcessState::Run:
        {
            return Spices::MemoryEntry::mallocFromPool(static_cast<size_t>(align));
        }

        /**
        * @brief allocate memory using malloc before MemoryPool is initialized or already exit.
        */
        case Spices::ProcessState::BeforeEntry:
        case Spices::ProcessState::Exit:
        default:
        {
            return Spices::MemoryEntry::mallocFromOS_Aligned(size, align);
        }
    }
}

/**
* @brief override delete operator.
* @param[in] ptr memory pointer.
* @param[in] align aligned bytes.
*/
void operator delete(void* ptr, std::align_val_t align) noexcept
{
    switch(Spices::MemoryEntry::GetProcessState())
    {
        /**
        * @brief free memory using free before MemoryPool is initialized.
        */
        case Spices::ProcessState::BeforeEntry:
        {
            Spices::MemoryEntry::freeToOS_Aligned(ptr, align);
            return;
        }

        /**
        * @brief free memory using MemoryPool if is initialized.
        */
        case Spices::ProcessState::Run:
        {
            Spices::MemoryEntry::freeToPool(ptr);
            return;
        }

        /**
        * @brief Do nothing while exist program.
        */
        case Spices::ProcessState::Exit:
        {
            return;
        }

        /**
        * @brief free memory using free default.
        */
        default:
        {
            Spices::MemoryEntry::freeToOS_Aligned(ptr, align);
            return;
        }
    }
}

/**
* @brief override delete[] operator.
* @param[in] ptr memory pointer.
* @param[in] align aligned bytes.
*/
void operator delete[](void* ptr, std::align_val_t align) noexcept
{
    switch(Spices::MemoryEntry::GetProcessState())
    {
        /**
        * @brief free memory using free before MemoryPool is initialized.
        */
        case Spices::ProcessState::BeforeEntry:
        {
            Spices::MemoryEntry::freeToOS_Aligned(ptr, align);
            return;
        }

        /**
        * @brief free memory using MemoryPool if is initialized.
        */
        case Spices::ProcessState::Run:
        {
            Spices::MemoryEntry::freeToPool(ptr);
            return;
        }

        /**
        * @brief Do nothing while exist program.
        */
        case Spices::ProcessState::Exit:
        {
            return;
        }

        /**
        * @brief free memory using free default.
        */
        default:
        {
            Spices::MemoryEntry::freeToOS_Aligned(ptr, align);
            return;
        }
    }
}

#else

/**
* @brief override new operation.
* Allocate memory using malloc.
* @param[in] size memory bytes.
* @return Returns memory pointer.
*/
void* operator new(size_t size)
{
    return std::move(Spices::MemoryEntry::mallocFromOS(size));
}

/**
* @brief override new[] operation.
* Allocate memory using malloc.
* @param[in] size memory bytes.
* @return Returns memory pointer.
*/
void* operator new[](size_t size)
{
    return std::move(Spices::MemoryEntry::mallocFromOS(size));
}

/**
* @brief override delete operator.
* Free memory using free.
* @param[in] ptr memory pointer.
*/
void operator delete(void* ptr) noexcept
{
    Spices::MemoryEntry::freeToOS(ptr);
}

/**
* @brief override delete[] operator.
* Free memory using free.
* @param[in] ptr memory pointer.
*/
void operator delete[](void* ptr) noexcept
{
    Spices::MemoryEntry::freeToOS(ptr);
}





/**
* @brief override new operation.
* Allocate memory using malloc.
* @param[in] size memory bytes.
* @param[in] align aligned bytes.
* @return Returns memory pointer.
*/
void* operator new(size_t size, std::align_val_t align)
{
    return std::move(Spices::MemoryEntry::mallocFromOS_Aligned(size, align));
}

/**
* @brief override new[] operation.
* Allocate memory using malloc.
* @param[in] size memory bytes.
* @param[in] align aligned bytes.
* @return Returns memory pointer.
*/
void* operator new[](size_t size, std::align_val_t align)
{
    return std::move(Spices::MemoryEntry::mallocFromOS_Aligned(size, align));
}

/**
* @brief override delete operator.
* Free memory using free.
* @param[in] ptr memory pointer.
* @param[in] a aligned bytes.
*/
void operator delete(void* ptr, std::align_val_t align) noexcept
{
    Spices::MemoryEntry::freeToOS_Aligned(ptr, align);
}

/**
* @brief override delete[] operator.
* Free memory using free.
* @param[in] ptr memory pointer.
* @param[in] a aligned bytes.
*/
void operator delete[](void* ptr, std::align_val_t align) noexcept
{
    Spices::MemoryEntry::freeToOS_Aligned(ptr, align);
}

#endif

/***************************************************************************************************************************/