/**
* @file NsightAftermathShaderDataBase.h
* @brief The ShaderDataBase Class Definitions.
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
    class ShaderDataBase
    {
    public:

        /**
        * @brief Constructor Function.
        */
        ShaderDataBase();

        /**
        * @brief Destructor Function.
        */
        virtual ~ShaderDataBase() = default;

        /**
        * @brief Find a shader bytecode binary by shader hash.
        * @param[in] shaderHash GFSDK_Aftermath_ShaderBinaryHash.
        * @param[in] shader .
        */
        bool FindShaderBinary(
            const GFSDK_Aftermath_ShaderBinaryHash& shaderHash , 
            std::vector<uint8_t>&                   shader
        ) const;

        /**
        * @brief Find a source shader debug info by shader debug name generated by the DXC compiler.
        * @param[in] shaderDebugName GFSDK_Aftermath_ShaderDebugName.
        * @param[in] shader .
        */
        bool FindShaderBinaryWithDebugData(
            const GFSDK_Aftermath_ShaderDebugName& shaderDebugName , 
            std::vector<uint8_t>&                  shader
        ) const;

        /**
        * @brief Add a shader binary to m_ShaderBinaries.
        * @param[in] shaderFilePath .
        */
        void AddShaderBinary(const char* shaderFilePath);

        /**
        * @brief Add a shader binary with debug info to m_ShaderBinariesWithDebugInfo.
        * @param[in] strippedShaderFilePath .
        * @param[in] shaderFilePath .
        */
        void AddShaderBinaryWithDebugInfo(const char* strippedShaderFilePath, const char* shaderFilePath);

    private:

        /**
        * @brief Read a file.
        * @param[in] filename .
        * @param[in] data File data.
        * @return Returns true if read successfully.
        */
        static bool ReadFile(const char* filename, std::vector<uint8_t>& data);

        /**
        * @brief List of shader binaries by ShaderBinaryHash.
        */
        std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<uint8_t>> m_ShaderBinaries;

        /**
        * @brief List of available shader binaries with source debug information by ShaderDebugName.
        */
        std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<uint8_t>> m_ShaderBinariesWithDebugInfo;
    };

}