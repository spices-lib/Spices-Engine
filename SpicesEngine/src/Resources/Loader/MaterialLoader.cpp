/**
* @file MaterialLoader.cpp.
* @brief The MaterialLoader Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "MaterialLoader.h"
#include "Core/Library/FileLibrary.h"
#include "Core/Library/StringLibrary.h"
#include "Utils/YamlUtils.h"
#include "Resources/Material/Material.h"
#include "Systems/ResourceSystem.h"

namespace Spices {

	/**
	* @brief Const variable: Bin Material File Path.
	*/
	const std::string defaultBinMaterialPath = "Materials/bin/";

	/**
	* @brief Const variable: Original Material File Path.
	*/
	const std::string defaultMaterialPath = "Materials/src/";

	/**
	* @brief Const variable: Bin Shader File Path.
	*/
	const std::string defaultBinShaderPath = "Shaders/spv/";

	/**
	* @brief Const variable: Material File Confirm header start.
	*/
	const char LoaderSignSatrt[100] = "#ItisSpicesMaterialSign: DataStart";

	/**
	* @brief Const variable: Material File Confirm header over.
	*/
	const char LoaderSignOver[100] = "#ItisSpicesMaterialSign: DateOver";

	/**
	* @brief Serialze Shader Config.
	* @param[in,out] out YAML Emitter.
	* @param[in] shaderStage What Stage shader use.
	* @param[in] shaderPath Shader path(short path).
	*/
	static void SerializeShaderConfig(
		YAML::Emitter&     out         , 
		const std::string& shaderStage , 
		const std::string& shaderPath
	);

	/**
	* @brief Serialze Texture Config.
	* @param[in,out] out YAML Emitter.
	* @param[in] name Texture name.
	* @param[in] param Texture parameter.
	*/
	static void SerializeTextureConfig(
		YAML::Emitter&      out   , 
		const std::string&  name  , 
		const TextureParam& param
	);

	bool MaterialLoader::Load(const std::string& fileName, Material* outMaterial)
	{
		/**
		* @brief Load from .sasset file first.
		*/
		if      (LoadFromSASSET(fileName, outMaterial))  return true;

		/**
		* @brief Load from .material file second.
		*/
		else if (LoadFromMaterial(fileName, outMaterial)) return true;
		
		else
		{
			std::stringstream ss;
			ss << "MaterialLoader: Could not find a valid file from the give filename: [" << fileName << "]";
			
			SPICES_CORE_WARN(ss.str());
			return false;
		}
	}

	bool MaterialLoader::LoadFromMaterial(const std::string& fileName, Material* outMaterial)
	{
		SPICES_PROFILE_ZONE;

		bool isFind = false;
		std::string filePath;
		for (auto& it : ResourceSystem::GetSearchFolder())
		{
			filePath = it + defaultMaterialPath + "Material." + fileName + ".material";
			if (FileLibrary::FileLibrary_Exists(filePath.c_str()))
			{
				isFind = true;
				break;
			}
		}
		if (!isFind) return false;

		/**
		* @brief Read .material file as bytes.
		*/
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		/**
		* @brief Explain bytes as a YAML::Node.
		*/
		YAML::Node data = YAML::Load(strStream.str());

		/**
		 * @brief Try get material name.
		 */
		if (!data["Material"])
		{
			std::stringstream ss;
			ss << filePath << ":  Not find a Material Node.";
			
			SPICES_CORE_ERROR(ss.str());
			return false;
		}

		std::string materialName = data["Material"].as<std::string>();

		/**
		 * @breif Try get shaders this material used.
		 */
		auto& shaders = data["Shaders"];
		if (shaders)
		{
			for (auto& shader : shaders)
			{
				if(shader["Stage"].IsDefined() && shader["Path"].IsDefined())
				{
					outMaterial->m_Shaders       [shader["Stage"].as<std::string>()].push_back(shader["Path"].as<std::string>());
					outMaterial->m_DefaultShaders[shader["Stage"].as<std::string>()].push_back(shader["Path"].as<std::string>());
				}
				else
				{
					std::stringstream ss;
					ss << "Stage/Path not finded in " << fileName;
					SPICES_CORE_ERROR(ss.str());
				}
			}
		}
		else
		{
			std::stringstream ss;
			ss << filePath << ":  Not find a Shaders Node.";
			
			SPICES_CORE_ERROR(ss.str());
			return false;
		}

		/**
		 * @breif Try get textures this material used.
		 */
		auto& textures = data["Textures"];
		if (textures)
		{
			for (auto& texture : textures)
			{
				if (texture["Name"].IsDefined() && texture["Value"].IsDefined())
				{
					outMaterial->m_TextureParams       .push_back(texture["Name"].as<std::string>(), texture["Value"].as<TextureParam>());
					outMaterial->m_DefaultTextureParams.push_back(texture["Name"].as<std::string>(), texture["Value"].as<TextureParam>());
				}
				else
				{
					std::stringstream ss;
					ss << "Name/Value not finded in " << fileName;
					SPICES_CORE_ERROR(ss.str());
				}
			}
		}

		/**
		 * @brief Try get parameters this material used.
		 */
		auto& parameters = data["Parameters"];
		if(parameters)
		{
			for (auto& parameter : parameters)
			{
				ConstantParams constantParams;
				if (parameter["Name"].IsDefined() && parameter["Value"].IsDefined())
				{
					
					constantParams.value        = parameter["Value"].as<ConstantParam>();
					constantParams.defaultValue = parameter["Value"].as<ConstantParam>();
				}
				else
				{
					std::stringstream ss;
					ss << "Name/Value not finded in " << fileName;
					SPICES_CORE_ERROR(ss.str());
				}

				if (parameter["MinValue"].IsDefined())
				{
					constantParams.hasMinValue  = true;
					constantParams.min          = parameter["MinValue"].as<ConstantParam>();
				}
				if (parameter["MaxValue"].IsDefined())
				{
					constantParams.hasMaxValue  = true;
					constantParams.max          = parameter["MaxValue"].as<ConstantParam>();
				}

				outMaterial->m_ConstantParams.push_back(parameter["Name"].as<std::string>(), constantParams);
			}
		}

		return true;
	}

	bool MaterialLoader::LoadFromSASSET(const std::string& fileName, Material* outMaterial)
	{
		SPICES_PROFILE_ZONE;

		bool isFind = false;
		std::string filePath;
		for (auto& it : ResourceSystem::GetSearchFolder())
		{
			filePath = it + defaultBinMaterialPath + "Material." + fileName + ".sasset";
			if (FileLibrary::FileLibrary_Exists(filePath.c_str()))
			{
				isFind = true;
				break;
			}
		}
		if (!isFind) return false;

		FileHandle f;
		FileLibrary::FileLibrary_Open(filePath.c_str(), FILE_MODE_READ, true, &f);

		uint64_t readed = 0;

		char startSign[100];
		FileLibrary::FileLibrary_Read(&f, sizeof(char) * 100, &startSign, &readed);

		if (!StringLibrary::StringsEqual(startSign, LoaderSignSatrt))
		{
			FileLibrary::FileLibrary_Close(&f);
			return false;
		}

		// TODO: ReadData

		char overSign[100];
		FileLibrary::FileLibrary_Read(&f, sizeof(char) * 100, &overSign, &readed);

		if (!StringLibrary::StringsEqual(overSign, LoaderSignOver))
		{
			FileLibrary::FileLibrary_Close(&f);
			return false;
		}

		FileLibrary::FileLibrary_Close(&f);

		return true;
	}

	bool MaterialLoader::SaveDefaultMaterial()
	{
		/*const std::string outFilePath = SPICES_ENGINE_ASSETS_PATH + "Materials/src/Material.Default.material";

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Material" << YAML::Value << "Default";

		out << YAML::Key << "Shaders" << YAML::Value << YAML::BeginSeq;
		SerializeShaderConfig(out, "vertShader", "MeshRenderer");
		SerializeShaderConfig(out, "fragShader", "MeshRenderer");
		out << YAML::EndSeq;

		out << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq;
		SerializeTextureConfig(out, "diffuse", { 1, nullptr, "stone_tile_vjqifhu/vjqifhu_4K_Albedo.jpg", 1, 0, 0, { 1, 1, 1 }, 1 });
		SerializeTextureConfig(out, "normal", { 1, nullptr, "stone_tile_vjqifhu/vjqifhu_4K_Normal.jpg", 1, 0, 1, {1, 1, 1}, 1 });
		SerializeTextureConfig(out, "specular", { 1, nullptr, "stone_tile_vjqifhu/vjqifhu_4K_Specular.jpg", 1, 0, 2, {1, 1, 1}, 1 });
		out << YAML::EndSeq;

		YAML::EndMap;

		std::ofstream fout(outFilePath);
		fout << out.c_str();*/

		return true;
	}

	void SerializeShaderConfig(YAML::Emitter& out, const std::string& shaderStage, const std::string& shaderPath)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "ShaderStage" << YAML::Value << shaderStage;
		out << YAML::Key << "ShaderPath" << YAML::Value << shaderPath;
		out << YAML::EndMap;
	}

	void SerializeTextureConfig(YAML::Emitter& out, const std::string& name, const TextureParam& param)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "TextureName" << YAML::Value << name;
		out << YAML::Key << "TextureParam" << YAML::Value << param;
		out << YAML::EndMap;
	}
}