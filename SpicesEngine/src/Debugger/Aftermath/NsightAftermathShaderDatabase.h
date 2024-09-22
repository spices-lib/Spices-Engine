/**
* @file NsightAftermathShaderDatabase.h
* @brief The ShaderDatabase Class Definitions.
* @author NVIDIA
* @see https://github.com/NVIDIA/nsight-aftermath-samples
*/

#pragma once

#include <vector>
#include <map>
#include <mutex>

#include "NsightAftermathHelpers.h"

namespace Spices {

    /**
    * @brief Implements a very simple shader database to help demonstrate
    * how to use the Nsight Aftermath GPU crash dump decoder API.
    *
    * In a real world scenario this would be part of an offline
    * analysis tool. This is for demonstration purposes only!
    */
    class ShaderDatabase
    {
    public:

        /**
        * @brief Constructor Function.
        */
        ShaderDatabase();

        /**
        * @brief Destructor Function.
        */
        virtual ~ShaderDatabase() = default;

        /**
        * @brief Find a shader bytecode binary by shader hash.
        */
        bool FindShaderBinary(const GFSDK_Aftermath_ShaderBinaryHash& shaderHash, std::vector<uint8_t>& shader) const;

        /**
        * @brief Find a source shader debug info by shader debug name generated by the DXC compiler.
        */
        bool FindShaderBinaryWithDebugData(const GFSDK_Aftermath_ShaderDebugName& shaderDebugName, std::vector<uint8_t>& shader) const;

    private:

        void AddShaderBinary(const char* shaderFilePath);
        void AddShaderBinaryWithDebugInfo(const char* strippedShaderFilePath, const char* shaderFilePath);

        static bool ReadFile(const char* filename, std::vector<uint8_t>& data);

        /**
        * @brief List of shader binaries by ShaderBinaryHash.
        */
        std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<uint8_t>> m_shaderBinaries;

        /**
        * @brief List of available shader binaries with source debug information by ShaderDebugName.
        */
        std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<uint8_t>> m_shaderBinariesWithDebugInfo;
    };

}