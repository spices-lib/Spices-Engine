/**
* @file NsightAftermathShaderDataBase.cpp
* @brief The ShaderDataBase Class Implementation.
* @author NVIDIA
* @see https://github.com/NVIDIA/nsight-aftermath-samples
*/

#include "Pchheader.h"
#include <fstream>
#include <iomanip>
#include <winuser.h>

#include "NsightAftermathShaderDatabase.h"

namespace Spices {

    ShaderDataBase::ShaderDataBase()
        : m_ShaderBinaries()
        , m_ShaderBinariesWithDebugInfo()
    {}

    bool ShaderDataBase::ReadFile(const char* filename, std::vector<uint8_t>& data)
    {
        SPICES_PROFILE_ZONE;

        std::ifstream fs(filename, std::ios::in | std::ios::binary);
        if (!fs)
        {
            return false;
        }

        fs.seekg(0, std::ios::end);
        data.resize(fs.tellg());
        fs.seekg(0, std::ios::beg);
        fs.read(reinterpret_cast<char*>(data.data()), data.size());
        fs.close();

        return true;
    }

    void ShaderDataBase::AddShaderBinary(const char* shaderFilePath)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Read the shader binary code from the file.
        */
        std::vector<uint8_t> data;
        if (!ReadFile(shaderFilePath, data))
        {
            return;
        }

        /**
        * @brief Create shader hash for the shader.
        */
        const GFSDK_Aftermath_SpirvCode shader{ data.data(), uint32_t(data.size()) };
        GFSDK_Aftermath_ShaderBinaryHash shaderHash;
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GetShaderHashSpirv(
            GFSDK_Aftermath_Version_API ,
            &shader                     ,
            &shaderHash
        ));

        /**
        * @brief Store the data for shader mapping when decoding GPU crash dumps.
        * cf. FindShaderBinary()
        */
        m_ShaderBinaries[shaderHash].swap(data);
    }

    void ShaderDataBase::AddShaderSource(std::vector<uint8_t> spirv)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Create shader hash for the shader.
        */
        const GFSDK_Aftermath_SpirvCode shader{ spirv.data(), uint32_t(spirv.size()) };
        GFSDK_Aftermath_ShaderBinaryHash shaderHash;
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GetShaderHashSpirv(
            GFSDK_Aftermath_Version_API ,
            &shader                     ,
            &shaderHash
        ));

        /**
        * @brief Store the data for shader mapping when decoding GPU crash dumps.
        * cf. FindShaderBinary()
        */
        m_ShaderBinaries[shaderHash].swap(spirv);
    }

    void ShaderDataBase::AddShaderBinaryWithDebugInfo(const char* strippedShaderFilePath, const char* shaderFilePath)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Read the shader debug data from the file.
        */
        std::vector<uint8_t> data;
        if (!ReadFile(shaderFilePath, data))
        {
            return;
        }
        std::vector<uint8_t> strippedData;
        if (!ReadFile(strippedShaderFilePath, strippedData))
        {
            return;
        }

        /**
        * @brief Generate shader debug name.
        */
        GFSDK_Aftermath_ShaderDebugName debugName;
        const GFSDK_Aftermath_SpirvCode shader{ data.data(), uint32_t(data.size()) };
        const GFSDK_Aftermath_SpirvCode strippedShader{ strippedData.data(), uint32_t(strippedData.size()) };
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GetShaderDebugNameSpirv(
            GFSDK_Aftermath_Version_API ,
            &shader                     ,
            &strippedShader             ,
            &debugName
        ));

        /**
        * @brief Store the data for shader instruction address mapping when decoding GPU crash dumps.
        * cf. FindShaderBinaryWithDebugData()
        */
        m_ShaderBinariesWithDebugInfo[debugName].swap(data);
    }

    void ShaderDataBase::AddShaderSourceWithDebugInfo(std::vector<uint8_t> strippedSpirv, std::vector<uint8_t> spirv)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Generate shader debug name.
        */
        GFSDK_Aftermath_ShaderDebugName debugName;
        const GFSDK_Aftermath_SpirvCode shader{ spirv.data(), uint32_t(spirv.size()) };
        const GFSDK_Aftermath_SpirvCode strippedShader{ strippedSpirv.data(), uint32_t(strippedSpirv.size()) };
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GetShaderDebugNameSpirv(
            GFSDK_Aftermath_Version_API ,
            &shader                     ,
            &strippedShader             ,
            &debugName
        ));

        /**
        * @brief Store the data for shader instruction address mapping when decoding GPU crash dumps.
        * cf. FindShaderBinaryWithDebugData()
        */
        m_ShaderBinariesWithDebugInfo[debugName].swap(spirv);
    }

    bool ShaderDataBase::FindShaderBinary(const GFSDK_Aftermath_ShaderBinaryHash& shaderHash, std::vector<uint8_t>& shader) const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Find shader binary data for the shader hash.
        */
        auto i_shader = m_ShaderBinaries.find(shaderHash);
        if (i_shader == m_ShaderBinaries.end())
        {
            /**
            * @brief Nothing found.
            */
            return false;
        }

        shader = i_shader->second;
        return true;
    }

    bool ShaderDataBase::FindShaderBinaryWithDebugData(const GFSDK_Aftermath_ShaderDebugName& shaderDebugName, std::vector<uint8_t>& shader) const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Find shader binary for the shader debug name.
        */
        auto i_shader = m_ShaderBinariesWithDebugInfo.find(shaderDebugName);
        if (i_shader == m_ShaderBinariesWithDebugInfo.end())
        {
            /**
            * @brief Nothing found.
            */
            return false;
        }

        shader = i_shader->second;
        return true;
    }

}