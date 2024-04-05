#pragma once
#include "Core/Core.h"

namespace Spiecs {

	class Material;

	//enum MaterialExtension
	//{
	//	UNKNOWN = 0,       // ERROR TYPE
	//	MATERIAL = 1,      // CUSTOM TYPE
	//	SASSET = 2         // BINARY TYPE
	//};

	class MaterialLoader
	{
	public:
		static bool Load(const std::string& fileName, Material* outMaterial);

	private:
		static bool LoadFromMaterial(const std::string& filepath, Material* outMaterial);
		static bool LoadFromSASSET(const std::string& filepath, Material* outMaterial);
	};
}