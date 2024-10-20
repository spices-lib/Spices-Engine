/**
* @file TracyProfilerWapper.h file.
* @brief The TracyProfilerWapper Class Definitions.
* @author Spices.
*/

#pragma once

#include "Core/Core.h"

#include <vulkan/vulkan.h>
#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>
#include <tracy/TracyVulkan.hpp>
#include <vector>
#include <memory>

namespace Spices {

	struct VulkanState;

	/**
	* @brief Wapper of Tracy GPU collect features.
	*/
	class TracyGPUContext
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		TracyGPUContext(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~TracyGPUContext() = default;

		/**
		* @brief Create this Single Instance.
		* @param[in] state VulkanState.
		*/
		static void CreateInstance(VulkanState& state);

		/**
		* @brief Get this Single Instance.
		* @return Returns this Single Instance.
		*/
		static TracyGPUContext& Get() { return *m_TracyGPUContext; }

		/**
		* @brief Get Context.
		* @return Returns Context.
		*/
		tracy::VkCtx*& GetContext() { return m_Context; }

	private:

		/**
		* @brief VulkanState.
		*/
		VulkanState& m_VulkanState;

		/**
		* @brief This Single Instance.
		*/
		static std::shared_ptr<TracyGPUContext> m_TracyGPUContext;

		/**
		* @brief Tracy Vulkan Context.
		*/
		tracy::VkCtx* m_Context;
		
	};

#ifdef TRACY_ENABLE

#define SPICES_PROFILE_GPU_CREATEINSTANCE(...)                           { TracyGPUContext::CreateInstance(__VA_ARGS__); } 
#define SPICES_PROFILE_FRAME                                             FrameMark
#define SPICES_PROFILE_ZONE                                              ZoneScoped
#define SPICES_PROFILE_ZONEN(...)                                        ZoneScopedN(__VA_ARGS__)
#define SPICES_PROFILE_ALLOC(ptr, size)                                  TracySecureAllocS(ptr, size, 20)
#define SPICES_PROFILE_FREE(ptr)                                         TracySecureFreeS(ptr, 20)
#define SPICES_PROFILE_MARK(...)                                         TracyMessageL(__VA_ARGS__)
#define SPICES_PROFILE_IMAGE(...)                                        FrameImage(__VA_ARGS__)

#define SPICES_PROFILE_VK_CONTEXHOSTCALIBRATED(pd, d, qr, ctd, ct)       TracyVkContextHostCalibrated(pd, d, qr, ctd, ct)
#define SPICES_PROFILE_VK_DESTROY                                        TracyVkDestroy(TracyGPUContext::Get().GetContext())                   
#define SPICES_PROFILE_VK_CONTEXTNAME(ctx, name, size)                   TracyVkContextName(ctx, name, size)
#define SPICES_PROFILE_VK_ZONE(cmdbuf, name)                             TracyVkZone(TracyGPUContext::Get().GetContext(), cmdbuf, name)
#define SPICES_PROFILE_VK_COLLECT(cmdbuf)                                TracyVkCollect(TracyGPUContext::Get().GetContext(), cmdbuf)

#else

#define SPICES_PROFILE_GPU_CREATEINSTANCE(...)                     
#define SPICES_PROFILE_FRAME                                       
#define SPICES_PROFILE_ZONE                                        
#define SPICES_PROFILE_ZONEN(...)                                  
#define SPICES_PROFILE_ALLOC(ptr, size)                            
#define SPICES_PROFILE_FREE(ptr)                                   
#define SPICES_PROFILE_MARK(...)                                   
#define SPICES_PROFILE_IMAGE(...)                                  

#define SPICES_PROFILE_VK_CONTEXHOSTCALIBRATED(pd, d, qr, ctd, ct) 
#define SPICES_PROFILE_VK_DESTROY                                  
#define SPICES_PROFILE_VK_CONTEXTNAME(ctx, name, size)             
#define SPICES_PROFILE_VK_ZONE(cmdbuf, name)                       
#define SPICES_PROFILE_VK_COLLECT(cmdbuf)                          

#endif

#ifdef TRACY_ENABLE

/********************************************* Those Macros and enviroment must be defined in preprocesser ******************************************************/

/**
* @brief tracy Feature Enable.
*
* #define TRACY_ENABLE
*/

/**
* @brief Used profile a short-lived program.
* Tracy connection won't be finisd even application is quit.
*
* #define TRACY_NO_EXIT
*/

/**
* @brief Used if want profile on demand.
* Enable profiling only when there��s an established connection with the server.
*
* #define TRACY_ON_DEMAND
*/

/**
* @brief Disable announce its presence to the local network.
* The program name that is sent out in the broadcast messages can be customized by using the
* TracySetProgramName(name) macro
*
* #define TRACY_NO_BROADCAST
*/

/**
* @brief Only listen on localhost interfaces.
*
* #define TRACY_ONLY_LOCALHOST
*/

/**
* @brief Extra listened address.
*
* TRACY_CLIENT_ADDRESS = xxx.xxx.x.xxx
*/

/**
* @brief Restrict it to only listening on IPv4 interfaces.
*
* #define TRACY_ONLY_IPV4
*/

/**
* @brief About multiple DLL.
*
* #define TRACY_MANUAL_LIFETIME
*/

/**
* @brief Display advanced information .
*
* #define TRACY_VERBOSE
*/

/**
* @brief The data connection port
*
* TRACY_BROADCAST_PORT = xx
* TRACY_PORT = xx
*/

/**
* @brief Controls whether a system load/save dialog is compiled in. If it��s enabled,
* the saved traces will be named trace.tracy.
*
* #define TRACY_NO_FILESELECTOR
*/

/**
* @brief Tracy will perform statistical data collection on the fly, if this macro is not
* defined. This allows extended trace analysis (for example, you can perform a live search for matching
* zones) at a small CPU processing cost and a considerable memory usage increase (at least 8 bytes per
* zone).
*
* #define TRACY_NO_STATISTICS
*/

/**
* @brief the main profiler view won��t occupy the whole window if this macro is
* defined. Additional setup is required for this to work. If you want to embed the server into your
* application, you probably should enable this option.
*
* #define TRACY_NO_ROOT_WINDOW
*/

/**
* @brief Set adapt to the system DPI scaling.
*
* TRACY_DPI_SCALE = xx
*/

/**
* @brief Disable the built in crash handling.
*
* #define TRACY_NO_CRASH_HANDLER
*/

/**
* @brief Disable Frame Image Feature.
*
* /#define TRACY_NO_FRAME_IMAGE
*/

#endif

}