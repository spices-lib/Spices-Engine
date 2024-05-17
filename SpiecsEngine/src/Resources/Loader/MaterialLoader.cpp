/**
* @file MaterialLoader.cpp.
* @brief The MaterialLoader Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "MaterialLoader.h"
#include "Core/Library/FileLibrary.h"
#include "Core/Library/StringLibrary.h"
#include "Utils/YamlUtils.h"
#include "Resources/Material/Material.h"

namespace Spiecs {

	/**
	* @brief Const variable: Bin Material File Path.
	*/
	const std::string defaultBinMaterialPath = SPIECS_ENGINE_ASSETS_PATH + "Materials/bin/";

	/**
	* @brief Const variable: Original Material File Path.
	*/
	const std::string defaultMaterialPath = SPIECS_ENGINE_ASSETS_PATH + "Materials/src/";

	/**
	* @brief Const variable: Bin Shader File Path.
	*/
	const std::string defaultBinShaderPath = SPIECS_ENGINE_ASSETS_PATH + "Shaders/spv/";

	/**
	* @brief Const variable: Material File Confirm header staer.
	*/
	const char LoaderSignSatrt[100] = "#ItisSpiecsMaterialSign: DataStart";

	/**
	* @brief Const variable: Material File Confirm header over.
	*/
	const char LoaderSignOver[100] = "#ItisSpiecsMaterialSign: DateOver";

	/**
	* @brief Serialze Shader Config.
	* @param[in out] out YAML Emitter.
	* @param[in] shaderStage What Stage shader use.
	* @param[in] shaderPath Shader path(short path).
	*/
	static void SerializeShaderConfig(YAML::Emitter& out, const std::string& shaderStage, const std::string& shaderPath);

	/**
	* @brief Serialze Texture Config.
	* @param[in out] out YAML Emitter.
	* @param[in] name Texture name.
	* @param[in] param Texture parameter.
	*/
	static void SerializeTextureConfig(YAML::Emitter& out, const std::string& name, const TextureParam& param);

	bool MaterialLoader::Load(const std::string& fileName, Material* outMaterial)
	{
		/**
		* @brief Load from .sasset file first.
		*/
		if      (LoadFromSASSET(defaultBinMaterialPath + "Material." + fileName + ".sasset", outMaterial))  return true;

		/**
		* @brief Load from .material file second.
		*/
		else if (LoadFromMaterial(defaultMaterialPath + "Material." + fileName + ".material", outMaterial)) return true;
		
		else
		{
			std::stringstream ss;
			ss << "MaterialLoader: Could not find a valid file from the give filename: [" << fileName << "]";
			
			SPIECS_CORE_WARN(ss.str());
			return false;
		}
	}

	bool MaterialLoader::LoadFromMaterial(const std::string& filepath, Material* outMaterial)
	{
		/**
		* @brief Read .material file as bytes.
		*/
		std::ifstream stream(filepath);
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
			ss << filepath << ":  Not find a Material Node.";
			
			SPIECS_CORE_ERROR(ss.str());
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
				outMaterial->m_Shaders[shader["Stage"].as<std::string>()] = shader["Path"].as<std::string>();
			}
		}
		else
		{
			std::stringstream ss;
			ss << filepath << ":  Not find a Shaders Node.";
			
			SPIECS_CORE_ERROR(ss.str());
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
				outMaterial->m_TextureParams[texture["Name"].as<std::string>()] = texture["Value"].as<TextureParam>();
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
				outMaterial->m_ConstantParams.push_back(parameter["Name"].as<std::string>(), parameter["Value"].as<ConstantParam>());
			}
		}

		return true;
	}

	bool MaterialLoader::LoadFromSASSET(const std::string& filepath, Material* outMaterial)
	{
		if (!FileLibrary::FileLibrary_Exists(filepath.c_str())) {
			return false;
		}

		FileHandle f;
		FileLibrary::FileLibrary_Open(filepath.c_str(), FILE_MODE_READ, true, &f);

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
		/*const std::string outFilePath = SPIECS_ENGINE_ASSETS_PATH + "Materials/src/Material.Default.material";

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