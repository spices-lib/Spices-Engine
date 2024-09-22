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
#include "NsightAftermathShaderDatabase.h"

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
        * @param[in] markerMap .
        */
        GpuCrashTracker(const MarkerMap& markerMap);

        /**
        * @brief Destructor Function.
        */
        virtual ~GpuCrashTracker();

        /**
        * @brief Initialize the GPU crash dump tracker.
        */
        void Initialize();

    private:

        /**
        * @brief Handler for GPU crash dump callbacks from Nsight Aftermath.
        */
        void OnCrashDump(const void* pGpuCrashDump, const uint32_t gpuCrashDumpSize);

        /**
        * @brief Handler for shader debug information callbacks.
        */
        void OnShaderDebugInfo(const void* pShaderDebugInfo, const uint32_t shaderDebugInfoSize);

        /**
        * @brief Handler for GPU crash dump description callbacks.
        */
        void OnDescription(PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription addDescription);

        /**
        * @brief Handler for app-managed marker resolve callback.
        */
        void OnResolveMarker(
            const void*    pMarkerData             , 
            const uint32_t markerDataSize          , 
            void**         ppResolvedMarkerData    , 
            uint32_t*      pResolvedMarkerDataSize
        );

        /**
        * @brief Helper for writing a GPU crash dump to a file.
        */
        void WriteGpuCrashDumpToFile(const void* pGpuCrashDump, const uint32_t gpuCrashDumpSize);

        /**
        * @brief Helper for writing shader debug information to a file.
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
        */
        void OnShaderSourceDebugInfoLookup(
            const GFSDK_Aftermath_ShaderDebugName& shaderDebugName ,
            PFN_GFSDK_Aftermath_SetData            setShaderBinary
        ) const;

        /**
        * @brief GPU crash dump callback.
        */
        static void GpuCrashDumpCallback(
            const void*    pGpuCrashDump    ,
            const uint32_t gpuCrashDumpSize ,
            void*          pUserData
        );

        /**
        * @brief Shader debug information callback.
        */
        static void ShaderDebugInfoCallback(
            const void*    pShaderDebugInfo    ,
            const uint32_t shaderDebugInfoSize ,
            void*          pUserData
        );

        /**
        * @brief GPU crash dump description callback.
        */
        static void CrashDumpDescriptionCallback(
            PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription addDescription,
            void*                                          pUserData
        );

        /**
        * @brief App-managed marker resolve callback.
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
        */
        static void ShaderDebugInfoLookupCallback(
            const GFSDK_Aftermath_ShaderDebugInfoIdentifier* pIdentifier        ,
            PFN_GFSDK_Aftermath_SetData                      setShaderDebugInfo ,
            void*                                            pUserData
        );

        /**
        * @brief Shader lookup callback.
        */
        static void ShaderLookupCallback(
            const GFSDK_Aftermath_ShaderBinaryHash* pShaderHash     ,
            PFN_GFSDK_Aftermath_SetData             setShaderBinary ,
            void*                                   pUserData
        );

        /**
        * @brief Shader source debug info lookup callback.
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
        ShaderDatabase m_ShaderDatabase;

        /**
        * @brief App-managed marker tracking
        */
        const MarkerMap& m_MarkerMap;
    };

}