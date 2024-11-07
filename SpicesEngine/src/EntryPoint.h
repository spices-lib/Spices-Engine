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
* @brief override new operation.
* @param[in] size memory bytes.
* @return Returns memory pointer.
*/
void* operator new(size_t size)
{
	/**
	* @brief allocate memory using malloc before MemoryPool is initialized.
	*/
	if (!Spices::MemoryPool::IsInitialized())
	{
		void* ptr = malloc(size);
		SPICES_PROFILE_ALLOC(ptr, size);
		return ptr;
	}

	/**
	* @brief allocate memory using MemoryPool if is initialized.
	*/
	else
	{
		return Spices::MemoryPool::Alloc(size);
	}
}

/**
* @brief override delete operator.
* @param[in] ptr memory pointer.
*/
void operator delete(void* ptr) noexcept
{
	/**
	* @brief free memory using free before MemoryPool is initialized.
	*/
	if (!Spices::MemoryPool::IsInitialized())
	{
		SPICES_PROFILE_FREE(ptr);
		free(ptr);
	}

	/**
	* @brief free memory using MemoryPool if is initialized.
	*/
	else
	{
		Spices::MemoryPool::Free(ptr);
	}
}

/**
* @brief Main Function.
*/
int main() {

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
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}