#include "Pchheader.h"
#include "MaterialLoader.h"
#include "Core/Library/FileLibrary.h"
#include "Core/Library/StringLibrary.h"
#include "Utils/YamlUtils.h"
#include "Render/Material/Material.h"

namespace Spiecs {

	const std::string defaultBinMaterialPath = SPIECS_ENGINE_ASSETS_PATH + "Materials/bin/";
	const std::string defaultMaterialPath = SPIECS_ENGINE_ASSETS_PATH + "Materials/src/";

	const std::string defaultBinShaderPath = SPIECS_ENGINE_ASSETS_PATH + "Shaders/spv/";

	const char LoaderSignSatrt[100] = "#ItisSpiecsMaterialSign: DataStart";
	const char LoaderSignOver[100] = "#ItisSpiecsMaterialSign: DateOver";

	static void SerializeShaderConfig(YAML::Emitter& out, const std::string& shaderStage, const std::string& shaderPath);
	static void SerializeTextureConfig(YAML::Emitter& out, const std::string& name, const Material::TextureParam& param);

	bool MaterialLoader::Load(const std::string& fileName, Material* outMaterial)
	{
		if (LoadFromSASSET(defaultBinMaterialPath + "Material." + fileName + ".sasset", outMaterial)) return true;
		else if (LoadFromMaterial(defaultMaterialPath + "Material." + fileName + ".material", outMaterial)) return true;
		else return false;
	}

	bool MaterialLoader::LoadFromMaterial(const std::string& filepath, Material* outMaterial)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Material"])
		{
			return false;
		}

		std::string materialName = data["Material"].as<std::string>();

		auto shaders = data["Shaders"];
		if (shaders)
		{
			for (auto shader : shaders)
			{
				outMaterial->m_Shaders[shader["ShaderStage"].as<std::string>()] = shader["ShaderPath"].as<std::string>();
			}
		}

		auto textures = data["Textures"];
		if (textures)
		{
			for (auto texture : textures)
			{
				outMaterial->m_TextureParams[texture["TextureName"].as<std::string>()] = texture["TextureParam"].as<Material::TextureParam>();
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
		const std::string outFilePath = SPIECS_ENGINE_ASSETS_PATH + "Materials/src/Material.Default.material";

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
		fout << out.c_str();

		return true;
	}

	void SerializeShaderConfig(YAML::Emitter& out, const std::string& shaderStage, const std::string& shaderPath)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "ShaderStage" << YAML::Value << shaderStage;
		out << YAML::Key << "ShaderPath" << YAML::Value << shaderPath;
		out << YAML::EndMap;
	}

	void SerializeTextureConfig(YAML::Emitter& out, const std::string& name, const Material::TextureParam& param)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "TextureName" << YAML::Value << name;
		out << YAML::Key << "TextureParam" << YAML::Value << param;
		out << YAML::EndMap;
	}
}