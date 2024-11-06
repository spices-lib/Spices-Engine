/**
* @file EntryPoint.h.
* @brief EntryPoint Definitions.
* @author Spices.
*/
#pragma once

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Memory/MemoryPool.h"

void* operator new(size_t size)
{
	if (!Spices::MemoryPool::IsInitialized())
	{
		void* ptr = malloc(size);
		return ptr;
	}
	else
	{
		void* ptr = Spices::MemoryPool::Alloc(size);
		return ptr;
	}
}

void operator delete(void* ptr) noexcept
{
	if (!Spices::MemoryPool::IsInitialized())
	{
		free(ptr);
	}
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