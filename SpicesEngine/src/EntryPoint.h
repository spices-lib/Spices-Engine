/**
* @file EntryPoint.h.
* @brief EntryPoint Definitions.
* @author Spices.
*/
#pragma once

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Memory/MemoryPool.h"

/**
* @brief Engine State Enum Definitions.
*/
enum class EngineState
{
	BeforeEntry = 0,  // before entry
	Run         = 1,  // run
	Exit        = 2   // exit
};

/**
* @brief Engine State Instance.
*/
static EngineState engineState = EngineState::BeforeEntry;

/**
* @brief Macros of modify Engine instance state.
*/
#define ENGINE_INSTANCE_ENTRY  { engineState = EngineState::Run; }
#define ENGINE_INSTANCE_EXIT   { engineState = EngineState::Exit; }

#define ENABLE_MAIN_MEMORY_POOL

#ifdef ENABLE_MAIN_MEMORY_POOL

/**
* @brief override new operation.
* @param[in] size memory bytes.
* @return Returns memory pointer.
*/
void* operator new(size_t size)
{
	/**
	* @brief allocate memory using malloc before MemoryPool is initialized.
	*/
	if (engineState != EngineState::Run)
	{
		void* ptr = malloc(size);
		SPICES_PROFILE_ALLOC_N(ptr, size, Spices::memoryPoolNames[0]);
		return std::move(ptr);
	}

	/**
	* @brief allocate memory using MemoryPool if is initialized.
	*/
	else
	{
		void* ptr = Spices::MemoryPool::Alloc(size);
		SPICES_PROFILE_ALLOC_N(ptr, size, Spices::memoryPoolNames[2]);
		return std::move(ptr);
	}
}

/**
* @brief override delete operator.
* @param[in] ptr memory pointer.
*/
void operator delete(void* ptr) noexcept
{
	/**
	* @brief Do nothing while exist program.
	*/
	if (engineState == EngineState::Exit)
	{
		return;
	}

	/**
	* @brief free memory using free before MemoryPool is initialized.
	*/
	if (engineState == EngineState::BeforeEntry)
	{
		SPICES_PROFILE_FREE_N(ptr, Spices::memoryPoolNames[0]);
		free(ptr);
	}

	/**
	* @brief free memory using MemoryPool if is initialized.
	*/
	else
	{
		SPICES_PROFILE_FREE_N(ptr, Spices::memoryPoolNames[2]);
		Spices::MemoryPool::Free(ptr);
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
	void* ptr = malloc(size);
	SPICES_PROFILE_ALLOC_N(ptr, size, Spices::memoryPoolNames[0]);
	return std::move(ptr);
}

/**
* @brief override delete operator.
* Free memory using free.
* @param[in] ptr memory pointer.
*/
void operator delete(void* ptr) noexcept
{
	SPICES_PROFILE_FREE_N(ptr, Spices::memoryPoolNames[0]);
	free(ptr);
}

#endif

/**
* @brief Main Function.
*/
int main() {

	ENGINE_INSTANCE_ENTRY

	try 
	{
		/**
		* @brief Create Application
		*/
		auto engine = Spices::Application();

		/**
		* @brief Application Run.
		*/
		engine.Run();
	}
	catch (const spdlog::spdlog_ex& ex) 
	{
		SPICES_CORE_ERROR(ex.what())
		
		return EXIT_FAILURE;
	}

	ENGINE_INSTANCE_EXIT

	return EXIT_SUCCESS;
}