/**
* @file NsightAftermathGpuCrashTracker.cpp
* @brief The GpuCrashTracker Class Implementation.
* @author NVIDIA
* @see https://github.com/NVIDIA/nsight-aftermath-samples
*/

#include "Pchheader.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <winuser.h>

#include "NsightAftermathGpuCrashTracker.h"

namespace Spices {

    std::unique_ptr<GpuCrashTracker> GpuCrashTracker::m_GpuCrashTracker;

    GpuCrashTracker::GpuCrashTracker()
        : m_Initialized(false)
        , m_Mutex()
        , m_ShaderDebugInfo()
        , m_ShaderDatabase()
        , m_FrameCut(0)
    {}

    GpuCrashTracker::~GpuCrashTracker()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief If initialized, disable GPU crash dumps.
        */
        if (m_Initialized)
        {
            GFSDK_Aftermath_DisableGpuCrashDumps();
        }
    }

    void GpuCrashTracker::Initialize()
    {
        SPICES_PROFILE_ZONE;

        /** 
        * @brief Enable GPU crash dumps and set up the callbacks for crash dump notifications,
        * shader debug information notifications, and providing additional crash
        * dump description data.Only the crash dump callback is mandatory. The other two
        * callbacks are optional and can be omitted, by passing nullptr, if the corresponding
        * functionality is not used.
        * The DeferDebugInfoCallbacks flag enables caching of shader debug information data
        * in memory. If the flag is set, ShaderDebugInfoCallback will be called only
        * in the event of a crash, right before GpuCrashDumpCallback. If the flag is not set,
        * ShaderDebugInfoCallback will be called for every shader that is compiled.
        */
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_EnableGpuCrashDumps(
            GFSDK_Aftermath_Version_API                                      ,
            GFSDK_Aftermath_GpuCrashDumpWatchedApiFlags_Vulkan               ,
            GFSDK_Aftermath_GpuCrashDumpFeatureFlags_DeferDebugInfoCallbacks , /* @brief Let the Nsight Aftermath library cache shader debug information.      */
            GpuCrashDumpCallback                                             , /* @brief Register callback for GPU crash dumps.                                */
            ShaderDebugInfoCallback                                          , /* @brief Register callback for shader debug information.                       */
            CrashDumpDescriptionCallback                                     , /* @brief Register callback for GPU crash dump description.                     */
            ResolveMarkerCallback                                            , /* @brief Register callback for resolving application-managed markers.          */
            this                                                                                                                                              
        ));                                                                    /* @brief Set the GpuCrashTracker object as user data for the above callbacks.  */

        m_Initialized = true;
    }

    void GpuCrashTracker::Init()
    {
        SPICES_PROFILE_ZONE;

        if (!m_GpuCrashTracker)
        {
            m_GpuCrashTracker = std::make_unique<GpuCrashTracker>();
            m_GpuCrashTracker->Initialize();
        }
    }

    void GpuCrashTracker::SetFrameCut(uint64_t frameCut)
    {
        SPICES_PROFILE_ZONE;

        m_FrameCut = frameCut % c_MarkerFrameHistory;

        m_MarkerMap[m_FrameCut].clear();
    }

    void GpuCrashTracker::SetMarker(uint64_t& markerId, const std::string& info)
    {
        SPICES_PROFILE_ZONE;

        markerId = m_MarkerMap[m_FrameCut].size();
        m_MarkerMap[m_FrameCut][markerId] = info;
    }

    void GpuCrashTracker::OnCrashDump(const void* pGpuCrashDump, const uint32_t gpuCrashDumpSize)
    {
        SPICES_PROFILE_ZONE;

        std::lock_guard<std::mutex> lock(m_Mutex);

        /**
        * @brief Write to file for later in-depth analysis with Nsight Graphics.
        */
        WriteGpuCrashDumpToFile(pGpuCrashDump, gpuCrashDumpSize);
    }

    void GpuCrashTracker::OnShaderDebugInfo(const void* pShaderDebugInfo, const uint32_t shaderDebugInfoSize)
    {
        SPICES_PROFILE_ZONE;

        std::lock_guard<std::mutex> lock(m_Mutex);

        /**
        * @brief Get shader debug information identifier.
        */
        GFSDK_Aftermath_ShaderDebugInfoIdentifier identifier = {};
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GetShaderDebugInfoIdentifier(
            GFSDK_Aftermath_Version_API ,
            pShaderDebugInfo            ,
            shaderDebugInfoSize         ,
            &identifier
        ));

        /**
        * @brief Store information for decoding of GPU crash dumps with shader address mapping
        * from within the application.
        */ 
        std::vector<uint8_t> data(
            (uint8_t*)pShaderDebugInfo, 
            (uint8_t*)pShaderDebugInfo + shaderDebugInfoSize
        );
        m_ShaderDebugInfo[identifier].swap(data);

        /**
        * @brief Write to file for later in-depth analysis of crash dumps with Nsight Graphics.
        */
        WriteShaderDebugInformationToFile(identifier, pShaderDebugInfo, shaderDebugInfoSize);
    }

    void GpuCrashTracker::OnDescription(PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription addDescription)
    {
        SPICES_PROFILE_ZONE;

        /** 
        * @brief Add some basic description about the crash. This is called after the GPU crash happens, but before
        * the actual GPU crash dump callback. The provided data is included in the crash dump and can be
        * retrieved using GFSDK_Aftermath_GpuCrashDump_GetDescription().
        */
        addDescription(GFSDK_Aftermath_GpuCrashDumpDescriptionKey_ApplicationName    , "SpicesEngineNsightAftermath");
        addDescription(GFSDK_Aftermath_GpuCrashDumpDescriptionKey_ApplicationVersion , "v1.0");
        addDescription(GFSDK_Aftermath_GpuCrashDumpDescriptionKey_UserDefined        , "Aftermath GPU Crash Dump.");
        addDescription(GFSDK_Aftermath_GpuCrashDumpDescriptionKey_UserDefined + 1    , "Engine State: Rendering.");
        addDescription(GFSDK_Aftermath_GpuCrashDumpDescriptionKey_UserDefined + 2    , "More user-defined information...");
    }
    
    void GpuCrashTracker::OnResolveMarker(
        const void*    pMarkerData             , 
        const uint32_t markerDataSize          , 
        void**         ppResolvedMarkerData    , 
        uint32_t*      pResolvedMarkerDataSize
    )
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Important: the pointer passed back via ppResolvedMarkerData must remain valid after this function returns
        * using references for all of the m_markerMap accesses ensures that the pointers refer to the persistent data
        */
        for (auto& map : m_MarkerMap)
        {
            const auto& foundMarker = map.find((uint64_t)pMarkerData);
            if (foundMarker != map.end())
            {
                const std::string& foundMarkerData = foundMarker->second;
                // std::string::data() will return a valid pointer until the string is next modified
                // we don't modify the string after calling data() here, so the pointer should remain valid
                *ppResolvedMarkerData = (void*)foundMarkerData.data();
                *pResolvedMarkerDataSize = static_cast<uint32_t>(foundMarkerData.length());
                return;
            }
        }
    }

    void GpuCrashTracker::WriteGpuCrashDumpToFile(const void* pGpuCrashDump, const uint32_t gpuCrashDumpSize)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Create a GPU crash dump decoder object for the GPU crash dump.
        */
        GFSDK_Aftermath_GpuCrashDump_Decoder decoder = {};
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_CreateDecoder(
            GFSDK_Aftermath_Version_API ,
            pGpuCrashDump               ,
            gpuCrashDumpSize            ,
            &decoder
        ));

        /**
        * @brief Use the decoder object to read basic information, like application
        * name, PID, etc.from the GPU crash dump.
        */
        GFSDK_Aftermath_GpuCrashDump_BaseInfo baseInfo = {};
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GetBaseInfo(decoder, &baseInfo));

        /**
        * @brief Use the decoder object to query the application name that was set
        * in the GPU crash dump description.
        */
        uint32_t applicationNameLength = 0;
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GetDescriptionSize(
            decoder                                                    ,
            GFSDK_Aftermath_GpuCrashDumpDescriptionKey_ApplicationName ,
            &applicationNameLength
        ));

        std::vector<char> applicationName(applicationNameLength, '\0');

        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GetDescription(
            decoder                                                    ,
            GFSDK_Aftermath_GpuCrashDumpDescriptionKey_ApplicationName ,
            static_cast<uint32_t>(applicationName.size())              ,
            applicationName.data()
        ));

        /**
        * @brief Create a unique file name for writing the crash dump data to a file.
        * Note: due to an Nsight Aftermath bug (will be fixed in an upcoming
        * driver release) we may see redundant crash dumps. As a workaround,
        * attach a unique count to each generated file name.
        */
        static int count = 0;
        const std::string baseFileName =
            std::string(applicationName.data())
            + "-"
            + std::to_string(baseInfo.pid)
            + "-"
            + std::to_string(++count);

        /**
        * @brief Write the crash dump data to a file using the .nv-gpudmp extension
        * registered with Nsight Graphics.
        */
        const std::string crashDumpFileName = baseFileName + ".nv-gpudmp";
        std::ofstream dumpFile(crashDumpFileName, std::ios::out | std::ios::binary);
        if (dumpFile)
        {
            dumpFile.write(static_cast<const char*>(pGpuCrashDump), gpuCrashDumpSize);
            dumpFile.close();
        }

        /*
        * @brief Decode the crash dump to a JSON string.
        * Step 1: Generate the JSON and get the size.
        */
        uint32_t jsonSize = 0;
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GenerateJSON(
            decoder                                           ,
            GFSDK_Aftermath_GpuCrashDumpDecoderFlags_ALL_INFO ,
            GFSDK_Aftermath_GpuCrashDumpFormatterFlags_NONE   ,
            ShaderDebugInfoLookupCallback                     ,
            ShaderLookupCallback                              ,
            ShaderSourceDebugInfoLookupCallback               ,
            this                                              ,
            &jsonSize
        ));

        /**
        * @brief Step 2: Allocate a buffer and fetch the generated JSON.
        */
        std::vector<char> json(jsonSize);
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GetJSON(
            decoder               ,
            static_cast<uint32_t>(json.size()) ,
            json.data()
        ));

        /**
        * @brief Write the crash dump data as JSON to a file.
        */
        const std::string jsonFileName = crashDumpFileName + ".json";
        std::ofstream jsonFile(jsonFileName, std::ios::out | std::ios::binary);
        if (jsonFile)
        {
           /**
           * @brief Write the JSON to the file (excluding string termination).
           */
           jsonFile.write(json.data(), json.size() - 1);
           jsonFile.close();
        }

        /**
        * @brief Destroy the GPU crash dump decoder object.
        */
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_DestroyDecoder(decoder));
    }

    void GpuCrashTracker::WriteShaderDebugInformationToFile(
        GFSDK_Aftermath_ShaderDebugInfoIdentifier identifier          ,
        const void*                               pShaderDebugInfo    ,
        const uint32_t                            shaderDebugInfoSize
    )
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Create a unique file name.
        */
        const std::string filePath = "shader-" + std::to_string(identifier) + ".nvdbg";

        std::ofstream f(filePath, std::ios::out | std::ios::binary);
        if (f)
        {
            f.write((const char*)pShaderDebugInfo, shaderDebugInfoSize);
        }
    }

    void GpuCrashTracker::OnShaderDebugInfoLookup(
        const GFSDK_Aftermath_ShaderDebugInfoIdentifier& identifier         ,
        PFN_GFSDK_Aftermath_SetData                      setShaderDebugInfo
    ) 
        const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Search the list of shader debug information blobs received earlier.
        */
        auto i_debugInfo = m_ShaderDebugInfo.find(identifier);
        if (i_debugInfo == m_ShaderDebugInfo.end())
        {
            /**
            * @brief Early exit, nothing found. No need to call setShaderDebugInfo.
            */
            return;
        }

        /**
        * @brief Let the GPU crash dump decoder know about the shader debug information
        * that was found.
        */
        setShaderDebugInfo(i_debugInfo->second.data(), uint32_t(i_debugInfo->second.size()));
    }

    void GpuCrashTracker::OnShaderLookup(
        const GFSDK_Aftermath_ShaderBinaryHash& shaderHash      ,
        PFN_GFSDK_Aftermath_SetData             setShaderBinary
    ) 
        const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Find shader binary data for the shader hash in the shader database.
        */
        std::vector<uint8_t> shaderBinary;
        if (!m_ShaderDatabase.FindShaderBinary(shaderHash, shaderBinary))
        {
            /**
            * @brief Early exit, nothing found. No need to call setShaderBinary.
            */
            return;
        }

        /**
        * @brief Let the GPU crash dump decoder know about the shader data
        * that was found.
        */
        setShaderBinary(shaderBinary.data(), uint32_t(shaderBinary.size()));
    }

    void GpuCrashTracker::OnShaderSourceDebugInfoLookup(
        const GFSDK_Aftermath_ShaderDebugName& shaderDebugName ,
        PFN_GFSDK_Aftermath_SetData            setShaderBinary
    ) 
        const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Find source debug info for the shader DebugName in the shader database.
        */
        std::vector<uint8_t> shaderBinary;
        if (!m_ShaderDatabase.FindShaderBinaryWithDebugData(shaderDebugName, shaderBinary))
        {
            /**
            * @brief Early exit, nothing found. No need to call setShaderBinary.
            */
            return;
        }

        /**
        * @brief Let the GPU crash dump decoder know about the shader debug data that was
        * found.
        */
        setShaderBinary(shaderBinary.data(), uint32_t(shaderBinary.size()));
    }

    void GpuCrashTracker::GpuCrashDumpCallback(
        const void*    pGpuCrashDump    ,
        const uint32_t gpuCrashDumpSize ,
        void*          pUserData
    )
    {
        SPICES_PROFILE_ZONE;

        GpuCrashTracker* pGpuCrashTracker = reinterpret_cast<GpuCrashTracker*>(pUserData);
        pGpuCrashTracker->OnCrashDump(pGpuCrashDump, gpuCrashDumpSize);
    }

    void GpuCrashTracker::ShaderDebugInfoCallback(
        const void*    pShaderDebugInfo    ,
        const uint32_t shaderDebugInfoSize ,
        void*          pUserData
    )
    {
        SPICES_PROFILE_ZONE;

        GpuCrashTracker* pGpuCrashTracker = reinterpret_cast<GpuCrashTracker*>(pUserData);
        pGpuCrashTracker->OnShaderDebugInfo(pShaderDebugInfo, shaderDebugInfoSize);
    }

    void GpuCrashTracker::CrashDumpDescriptionCallback(
        PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription addDescription ,
        void*                                          pUserData
    )
    {
        SPICES_PROFILE_ZONE;

        GpuCrashTracker* pGpuCrashTracker = reinterpret_cast<GpuCrashTracker*>(pUserData);
        pGpuCrashTracker->OnDescription(addDescription);
    }

    void GpuCrashTracker::ResolveMarkerCallback(
        const void*    pMarkerData             ,
        const uint32_t markerDataSize          ,
        void*          pUserData               ,
        void**         ppResolvedMarkerData    ,
        uint32_t*      pResolvedMarkerDataSize
    )
    {
        SPICES_PROFILE_ZONE;

        GpuCrashTracker* pGpuCrashTracker = reinterpret_cast<GpuCrashTracker*>(pUserData);
        pGpuCrashTracker->OnResolveMarker(pMarkerData, markerDataSize, ppResolvedMarkerData, pResolvedMarkerDataSize);
    }

    void GpuCrashTracker::ShaderDebugInfoLookupCallback(
        const GFSDK_Aftermath_ShaderDebugInfoIdentifier* pIdentifier        ,
        PFN_GFSDK_Aftermath_SetData                      setShaderDebugInfo ,
        void*                                            pUserData
    )
    {
        SPICES_PROFILE_ZONE;

        GpuCrashTracker* pGpuCrashTracker = reinterpret_cast<GpuCrashTracker*>(pUserData);
        pGpuCrashTracker->OnShaderDebugInfoLookup(*pIdentifier, setShaderDebugInfo);
    }

    void GpuCrashTracker::ShaderLookupCallback(
        const GFSDK_Aftermath_ShaderBinaryHash* pShaderHash     ,
        PFN_GFSDK_Aftermath_SetData             setShaderBinary ,
        void*                                   pUserData
    )
    {
        SPICES_PROFILE_ZONE;

        GpuCrashTracker* pGpuCrashTracker = reinterpret_cast<GpuCrashTracker*>(pUserData);
        pGpuCrashTracker->OnShaderLookup(*pShaderHash, setShaderBinary);
    }

    void GpuCrashTracker::ShaderSourceDebugInfoLookupCallback(
        const GFSDK_Aftermath_ShaderDebugName* pShaderDebugName ,
        PFN_GFSDK_Aftermath_SetData            setShaderBinary  ,
        void*                                  pUserData
    )
    {
        SPICES_PROFILE_ZONE;

        GpuCrashTracker* pGpuCrashTracker = reinterpret_cast<GpuCrashTracker*>(pUserData);
        pGpuCrashTracker->OnShaderSourceDebugInfoLookup(*pShaderDebugName, setShaderBinary);
    }

}