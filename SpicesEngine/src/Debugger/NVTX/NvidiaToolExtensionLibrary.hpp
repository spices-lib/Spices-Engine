/**
* @file NvidiaToolExtensionLibrary.hpp file.
* @brief The NvidiaToolExtensionLibrary Class Definitions.
* @author Spices.
* @see https://nvidia.github.io/NVTX/doxygen-cpp/index.html .
*/

#pragma once
#include "Core/Core.h"

#include <nvtx3/nvtx3.hpp>

namespace Spices {

	/**
	* @brief Define a custom domain tag type.
	*/
	struct SpicesEngineDomain 
	{ 
		static constexpr char const* name{ "SpicesEngine Domain" }; 
	};

	/**
	* @brief Define a named category tag type.
	*/
	struct SpicesEngineCategory 
	{
		static constexpr char const* name{ "SpicesEngine Category" };
		static constexpr uint32_t id{ 42 };
	};

	/**
	* @brief Define a registered string tag type.
	*/
	struct SpicesEngineMessage
	{ 
		static constexpr char const* message{ "SpicesEngine Message" }; 
	};

#ifdef SPICES_DEBUG

#define NVTX_FUNCTION                 NVTX3_FUNC_RANGE()
#define NVTX_MARK(...)               { nvtx3::mark(__VA_ARGS__); }         
#define NVTX_RANGE(str)                                                                                                    \
	auto& cat_##str = nvtx3::named_category_in<SpicesEngineDomain>::get<SpicesEngineCategory>();                           \
	nvtx3::scoped_range_in<SpicesEngineDomain> r_##str{ nvtx3::rgb{255, 255, 0}, #str, cat_##str, nvtx3::payload{42} };    \

#endif

#ifdef SPICES_RELEASE

#define NVTX_FUNCTION 
#define NVTX_MARK(...)  
#define NVTX_RANGE(str)

#endif

}