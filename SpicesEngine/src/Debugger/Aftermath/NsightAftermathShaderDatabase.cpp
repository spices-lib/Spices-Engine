/**
* @file NsightAftermathShaderDatabase.cpp
* @brief The ShaderDatabase Class Implementation.
* @author NVIDIA
* @see https://github.com/NVIDIA/nsight-aftermath-samples
*/

#include "Pchheader.h"
#include <fstream>
#include <iomanip>
#include <winuser.h>

#include "NsightAftermathShaderDatabase.h"

namespace Spices {

    ShaderDatabase::ShaderDatabase()
        : m_shaderBinaries()
        , m_shaderBinariesWithDebugInfo()
    {
        /**
        * @brief Add shader binaries to database.
        */
        AddShaderBinary("cube.vert.spirv");
        AddShaderBinary("cube.frag.spirv");

        /**
        * @brief Add the not stripped shader binaries to the database, too.
        */
        AddShaderBinaryWithDebugInfo("cube.vert.spirv", "cube.vert.full.spirv");
        AddShaderBinaryWithDebugInfo("cube.frag.spirv", "cube.frag.full.spirv");
    }

    bool ShaderDatabase::ReadFile(const char* filename, std::vector<uint8_t>& data)
    {
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

    void ShaderDatabase::AddShaderBinary(const char* shaderFilePath)
    {
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
        m_shaderBinaries[shaderHash].swap(data);
    }

    void ShaderDatabase::AddShaderBinaryWithDebugInfo(const char* strippedShaderFilePath, const char* shaderFilePath)
    {
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
        m_shaderBinariesWithDebugInfo[debugName].swap(data);
    }

    bool ShaderDatabase::FindShaderBinary(const GFSDK_Aftermath_ShaderBinaryHash& shaderHash, std::vector<uint8_t>& shader) const
    {
        /**
        * @brief Find shader binary data for the shader hash.
        */
        auto i_shader = m_shaderBinaries.find(shaderHash);
        if (i_shader == m_shaderBinaries.end())
        {
            /**
            * @brief Nothing found.
            */
            return false;
        }

        shader = i_shader->second;
        return true;
    }

    bool ShaderDatabase::FindShaderBinaryWithDebugData(const GFSDK_Aftermath_ShaderDebugName& shaderDebugName, std::vector<uint8_t>& shader) const
    {
        /**
        * @brief Find shader binary for the shader debug name.
        */
        auto i_shader = m_shaderBinariesWithDebugInfo.find(shaderDebugName);
        if (i_shader == m_shaderBinariesWithDebugInfo.end())
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