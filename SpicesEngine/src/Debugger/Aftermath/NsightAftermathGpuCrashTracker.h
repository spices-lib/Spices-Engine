/**
* @file NsightAftermathGpuCrashTracker.h
* @brief The GpuCrashTracker Class Definitions.
* @author NVIDIA
* @see https://github.com/NVIDIA/nsight-aftermath-samples
*/

#pragma once

#include <map>
#include <mutex>

#include "NsightAftermathHelpers.h"
#include "NsightAftermathShaderDataBase.h"

namespace Spices {

    /**
    * @brief Implements GPU crash dump tracking using the Nsight Aftermath API.
    */
    class GpuCrashTracker
    {
    public:
    
        /**
        * @brief keep four frames worth of marker history.
        */
        const static unsigned int c_MarkerFrameHistory = 4;
        typedef std::array<std::map<uint64_t, std::string>, c_MarkerFrameHistory> MarkerMap;

    public:

        /**
        * @brief Constructor Function.
        */
        GpuCrashTracker();

        /**
        * @brief Destructor Function.
        */
        virtual ~GpuCrashTracker();

        /**
        * @brief Initialize the GPU crash dump tracker.
        */
        void Initialize();

        /**
        * @brief Create single instance of this class.
        */
        static void Init();

        /**
        * @brief Aftermath handle device lost function.
        */
        static void AftermathDeviceLostCheck();

        /**
        * @brief Get single instance of this class.
        * @return Return this reference.
        */
        static GpuCrashTracker& Get() { return *m_GpuCrashTracker; }

        /**
        * @brief Set FrameCut.
        * @param[in] frameCut FrameCut.
        */
        void SetFrameCut(uint64_t frameCut);

        /**
        * @brief Set Marker.
        * @param[in] markerId Marker Id.
        * @param[in] info Marker informations.
        */
        void SetMarker(uint64_t& markerId, const std::string& info);

        /**
        * @brief Get ShaderDataBase.
        * @return Returns ShaderDataBase.
        */
        ShaderDataBase& GetShaderDataBase() { return m_ShaderDataBase; }

    private:

        /**
        * @brief Handler for GPU crash dump callbacks from Nsight Aftermath.
        * @param[in] pGpuCrashDump .
        * @param[in] gpuCrashDumpSize .
        */
        void OnCrashDump(const void* pGpuCrashDump, const uint32_t gpuCrashDumpSize);

        /**
        * @brief Handler for shader debug information callbacks.
        * @param[in] pShaderDebugInfo .
        * @param[in] shaderDebugInfoSize .
        */
        void OnShaderDebugInfo(const void* pShaderDebugInfo, const uint32_t shaderDebugInfoSize);

        /**
        * @brief Handler for GPU crash dump description callbacks.
        * @param[in] addDescription PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription.
        */
        void OnDescription(PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription addDescription);

        /**
        * @brief Handler for app-managed marker resolve callback.
        * @param[in] pMarkerData .
        * @param[in] markerDataSize .
        * @param[in] ppResolvedMarkerData .
        * @param[in] pResolvedMarkerDataSize .
        */
        void OnResolveMarker(
            const void*    pMarkerData             , 
            const uint32_t markerDataSize          , 
            void**         ppResolvedMarkerData    , 
            uint32_t*      pResolvedMarkerDataSize
        );

        /**
        * @brief Helper for writing a GPU crash dump to a file.
        * @param[in] pGpuCrashDump .
        * @param[in] gpuCrashDumpSize .
        */
        void WriteGpuCrashDumpToFile(const void* pGpuCrashDump, const uint32_t gpuCrashDumpSize);

        /**
        * @brief Helper for writing shader debug information to a file.
        * @param[in] identifier GFSDK_Aftermath_ShaderDebugInfoIdentifier.
        * @param[in] pShaderDebugInfo .
        * @param[in] shaderDebugInfoSize .
        */
        void WriteShaderDebugInformationToFile(
            GFSDK_Aftermath_ShaderDebugInfoIdentifier identifier          ,
            const void*                               pShaderDebugInfo    ,
            const uint32_t                            shaderDebugInfoSize
        );

        /**
        * @brief Handler for shader debug information lookup callbacks.
        * This is used by the JSON decoder for mapping shader instruction
        * addresses to SPIR-V IL lines or GLSL source lines.
        * @param[in] identifier GFSDK_Aftermath_ShaderDebugInfoIdentifier.
        * @param[in] setShaderDebugInfo PFN_GFSDK_Aftermath_SetData.
        */
        void OnShaderDebugInfoLookup(
            const GFSDK_Aftermath_ShaderDebugInfoIdentifier& identifier         ,
            PFN_GFSDK_Aftermath_SetData                      setShaderDebugInfo
        ) const;

        /**
        * @brief Handler for shader lookup callbacks.
        * This is used by the JSON decoder for mapping shader instruction
        * addresses to SPIR-V IL lines or GLSL source lines.
        * NOTE: If the application loads stripped shader binaries (ie; --strip-all in spirv-remap),
        * Aftermath will require access to both the stripped and the not stripped
        * shader binaries.
        * @param[in] shaderHash GFSDK_Aftermath_ShaderBinaryHash.
        * @param[in] setShaderBinary PFN_GFSDK_Aftermath_SetData.
        */
        void OnShaderLookup(
            const GFSDK_Aftermath_ShaderBinaryHash& shaderHash      ,
            PFN_GFSDK_Aftermath_SetData             setShaderBinary
        ) const;

        /**
        * @brief Handler for shader source debug info lookup callbacks.
        * This is used by the JSON decoder for mapping shader instruction addresses to
        * GLSL source lines, if the shaders used by the application were compiled with
        * separate debug info data files.
        * @param[in] shaderDebugName GFSDK_Aftermath_ShaderDebugName.
        * @param[in] setShaderBinary PFN_GFSDK_Aftermath_SetData.
        */
        void OnShaderSourceDebugInfoLookup(
            const GFSDK_Aftermath_ShaderDebugName& shaderDebugName ,
            PFN_GFSDK_Aftermath_SetData            setShaderBinary
        ) const;

        /**
        * @brief GPU crash dump callback.
        * @param[in] pGpuCrashDump .
        * @param[in] gpuCrashDumpSize .
        * @param[in] pUserData .
        */
        static void GpuCrashDumpCallback(
            const void*    pGpuCrashDump    ,
            const uint32_t gpuCrashDumpSize ,
            void*          pUserData
        );

        /**
        * @brief Shader debug information callback.
        * @param[in] pShaderDebugInfo .
        * @param[in] shaderDebugInfoSize .
        * @param[in] pUserData .
        */
        static void ShaderDebugInfoCallback(
            const void*    pShaderDebugInfo    ,
            const uint32_t shaderDebugInfoSize ,
            void*          pUserData
        );

        /**
        * @brief GPU crash dump description callback.
        * @param[in] addDescription PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription.
        * @param[in] pUserData .
        */
        static void CrashDumpDescriptionCallback(
            PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription addDescription,
            void*                                          pUserData
        );

        /**
        * @brief App-managed marker resolve callback.
        * @param[in] pMarkerData .
        * @param[in] markerDataSize .
        * @param[in] pUserData .
        * @param[in] ppResolvedMarkerData .
        * @param[in] pResolvedMarkerDataSize .
        */
        static void ResolveMarkerCallback(
            const void*    pMarkerData             ,
            const uint32_t markerDataSize          ,
            void*          pUserData               ,
            void**         ppResolvedMarkerData    ,
            uint32_t*      pResolvedMarkerDataSize
        );

        /**
        * @brief Shader debug information lookup callback.
        * @param[in] pIdentifier GFSDK_Aftermath_ShaderDebugInfoIdentifier.
        * @param[in] setShaderDebugInfo PFN_GFSDK_Aftermath_SetData.
        * @param[in] pUserData .
        */
        static void ShaderDebugInfoLookupCallback(
            const GFSDK_Aftermath_ShaderDebugInfoIdentifier* pIdentifier        ,
            PFN_GFSDK_Aftermath_SetData                      setShaderDebugInfo ,
            void*                                            pUserData
        );

        /**
        * @brief Shader lookup callback.
        * @param[in] pShaderHash GFSDK_Aftermath_ShaderBinaryHash.
        * @param[in] setShaderBinary PFN_GFSDK_Aftermath_SetData.
        * @param[in] pUserData .
        */
        static void ShaderLookupCallback(
            const GFSDK_Aftermath_ShaderBinaryHash* pShaderHash     ,
            PFN_GFSDK_Aftermath_SetData             setShaderBinary ,
            void*                                   pUserData
        );

        /**
        * @brief Shader source debug info lookup callback.
        * @param[in] pShaderDebugName GFSDK_Aftermath_ShaderDebugName.
        * @param[in] setShaderBinary PFN_GFSDK_Aftermath_SetData.
        * @param[in] pUserData .
        */
        static void ShaderSourceDebugInfoLookupCallback(
            const GFSDK_Aftermath_ShaderDebugName* pShaderDebugName ,
            PFN_GFSDK_Aftermath_SetData            setShaderBinary  ,
            void*                                  pUserData
        );

    private:

        /**
        * @brief Is the GPU crash dump tracker initialized?
        */
        bool m_Initialized;

        /**
        * @brief For thread-safe access of GPU crash tracker state.
        */
        mutable std::mutex m_Mutex;

        /**
        * @brief List of Shader Debug Information by ShaderDebugInfoIdentifier.
        */
        std::map<GFSDK_Aftermath_ShaderDebugInfoIdentifier, std::vector<uint8_t>> m_ShaderDebugInfo;

        /**
        * @brief The mock shader database.
        */
        ShaderDataBase m_ShaderDataBase;

        /**
        * @brief App-managed marker tracking.
        */
        MarkerMap m_MarkerMap;

        /**
        * @brief Frame Count cut.
        */
        uint32_t m_FrameCut;

        /**
        * @brief GpuCrashTracker single instance.
        */
        static std::unique_ptr<GpuCrashTracker> m_GpuCrashTracker;
    };

#ifdef SPICES_DEBUG

#define NSIGHTAFTERMATH_GPUCRASHTRACKER_INIT                                                                        { ::Spices::GpuCrashTracker::Init(); }
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_DEVICELOSECHECK(val)                                                        { if (val == VK_ERROR_DEVICE_LOST){ GpuCrashTracker::AftermathDeviceLostCheck(); } }
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_SETFRAMECUT(...)                                                            { ::Spices::GpuCrashTracker::Get().SetFrameCut(__VA_ARGS__); }
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(commandBuffer, funcLibrary, string)                           { uint64_t markId; ::Spices::GpuCrashTracker::Get().SetMarker(markId, string);  funcLibrary.vkCmdSetCheckpointNV(commandBuffer, (const void*)markId); }
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_ADDSHADERBINARY(...)                                                        { ::Spices::GpuCrashTracker::Get().GetShaderDataBase().AddShaderBinary(__VA_ARGS__); }
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_ADDSHADERBINARY_WITHDEBUGINFO(...)                                          { ::Spices::GpuCrashTracker::Get().GetShaderDataBase().AddShaderBinaryWithDebugInfo(__VA_ARGS__); }
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_ADDSHADERSOURCE(...)                                                        { ::Spices::GpuCrashTracker::Get().GetShaderDataBase().AddShaderSource(__VA_ARGS__); }
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_ADDSHADERSOURCE_WITHDEBUGINFO(...)                                          { ::Spices::GpuCrashTracker::Get().GetShaderDataBase().AddShaderSourceWithDebugInfo(__VA_ARGS__); }

#endif

#ifdef SPICES_RELEASE

#define NSIGHTAFTERMATH_GPUCRASHTRACKER_INIT                                             
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_DEVICELOSECHECK(val)                             
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_SETFRAMECUT(...)                                 
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(commandBuffer, funcLibrary, string)
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_ADDSHADERBINARY(...)                             
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_ADDSHADERBINARY_WITHDEBUGINFO(...)               
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_ADDSHADERSOURCE(...)              
#define NSIGHTAFTERMATH_GPUCRASHTRACKER_ADDSHADERSOURCE_WITHDEBUGINFO(...)

#endif

}