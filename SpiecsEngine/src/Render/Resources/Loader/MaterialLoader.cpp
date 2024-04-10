#include "Pchheader.h"
#include "MaterialLoader.h"
#include "Render/Resources/Material.h"
#include "Core/Library/FileLibrary.h"
#include "Core/Library/StringLibrary.h"

namespace Spiecs {

	const std::string defaultBinMaterialPath = SPIECS_ENGINE_ASSETS_PATH + "Materials/bin/";
	const std::string defaultMaterialPath = SPIECS_ENGINE_ASSETS_PATH + "Materials/src/";

	const std::string defaultBinShaderPath = SPIECS_ENGINE_ASSETS_PATH + "Shaders/spv/";

	const char LoaderSignSatrt[100] = "#ItisSpiecsMaterialSign: DataStart";
	const char LoaderSignOver[100] = "#ItisSpiecsMaterialSign: DateOver";

	bool MaterialLoader::Load(const std::string& fileName, Material* outMaterial)
	{
		if (LoadFromSASSET(defaultBinMaterialPath + "Material." + fileName + ".sasset", outMaterial)) return true;
		else if (LoadFromMaterial(defaultMaterialPath + "Material." + fileName + ".material", outMaterial)) return true;
		else return false;
	}

	bool MaterialLoader::LoadFromMaterial(const std::string& filepath, Material* outMaterial)
	{
		if (!FileLibrary::FileLibrary_Exists(filepath.c_str())) {
			return false;
		}

		FileHandle f;
		FileLibrary::FileLibrary_Open(filepath.c_str(), FILE_MODE_READ, true, &f);

		char line_buf[1024] = "";
		char* p = &line_buf[0];
		uint64_t line_length = 0;
		uint32_t line_number = 1; // Used for Debug Loacting. TODO: Finish it
		while (FileLibrary::FileLibrary_Read_Line(&f, 1023, &p, &line_length))
		{
			// Skip blank lines and comments.
			if (line_length < 1 || line_buf[0] == '#' || (line_buf[0] == '/r' && line_buf[1] == '/n')) {
				line_number++;
				continue;
			}

			std::vector<std::string> tempdata = StringLibrary::SplitString(std::string(line_buf), ';');
			std::vector<std::string> data = StringLibrary::SplitString(tempdata[0], '=');

			// Shader
			if (data[0] == "vertShader")
			{
				if (data.size() == 1) { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				if (data[1] == "")    { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				outMaterial->m_VertShaderPath = defaultBinShaderPath + "Shader." + data[1] + ".vert.spv";
				continue;
			}
			if (data[0] == "fragShader")
			{
				if (data.size() == 1) { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				if (data[1] == "")    { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				outMaterial->m_FragShaderPath = defaultBinShaderPath + "Shader." + data[1] + ".frag.spv";
				continue;
			}

			// Texture
			LoadTextureConfig(data, "diffuseTexture", 0, outMaterial);
			LoadTextureConfig(data, "normalTexture", 1, outMaterial);
			LoadTextureConfig(data, "specularTexture", 2, outMaterial);

			// Parameter
			if (data[0] == "diffuseIntensity")
			{
				if (data.size() == 1)  { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				if (data[1] == "")     { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				outMaterial->m_TextureIntensity[0] = std::stof(data[1]);
				continue;
			}
			if (data[0] == "normalIntensity")
			{
				if (data.size() == 1)  { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				if (data[1] == "")     { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				outMaterial->m_TextureIntensity[1] = std::stof(data[1]);
				continue;
			}
			if (data[0] == "specularIntensity")
			{
				if (data.size() == 1)  { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				if (data[1] == "")     { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); }
				outMaterial->m_TextureIntensity[2] = std::stof(data[1]);
				continue;
			}

			line_number++;
		}

		FileLibrary::FileLibrary_Close(&f);

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

	bool MaterialLoader::LoadTextureConfig(const std::vector<std::string>& config, const std::string& name, uint32_t arrayIndex, Material* outMaterial)
	{
		if (config[0] != name) { return false; }
		if (config.size() != 3) { __debugbreak(); return false; }
		if (config[2] == "") { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); return false; }
		if (config[1] != "None")
		{
			outMaterial->m_TextureIsUse[arrayIndex] = true;
			outMaterial->m_TexturePaths[arrayIndex] = SPIECS_ENGINE_ASSETS_PATH + "Textures/src/" + config[1];
			std::vector<std::string> vec = StringLibrary::SplitString(config[2], ',');
			if (vec.size() != 3) { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); return false; }
			outMaterial->m_TextureSetBinding[arrayIndex][0] = std::stoi(vec[0]);
			outMaterial->m_TextureSetBinding[arrayIndex][1] = std::stoi(vec[1]);
			outMaterial->m_TextureSetBinding[arrayIndex][2] = std::stoi(vec[2]);
		}
		else
		{
			std::vector<std::string> vec = StringLibrary::SplitString(config[2], ',');
			if (vec.size() != 3) { SPIECS_LOG("MaterialLoad Error: No Value");  __debugbreak(); return false; }
			outMaterial->m_TextureIsUse[arrayIndex] = false;
			outMaterial->m_TextureV3[arrayIndex] = { std::stof(vec[0]), std::stof(vec[1]), std::stof(vec[2]) };
		}

		return true;
	}
}