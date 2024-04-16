#pragma once
#include "Core/Core.h"

namespace Spiecs {

	class MeshPack;

	enum MeshExtension
	{
		UNKNOWN = 0, // ERROR TYPE
		OBJ = 1,     // OBJ TYPE
		FBX = 2,     // FBX TYPE
		SASSET = 3   // BINARY TYPE
	};
	
	class MeshLoader
	{
	public:
		static bool Load(const std::string& fileName, MeshPack* outMeshPack);

	private:
		static bool LoadFromOBJ(const std::string& filepath, MeshPack* outMeshPack);
		static bool LoadFromFBX(const std::string& filepath, MeshPack* outMeshPack);
		static bool LoadFromSASSET(const std::string& filepath, MeshPack* outMeshPack);
		static bool WriteSASSET(const std::string& filepath, MeshPack* outMeshPack);

		static void GetBinPath(std::string& filePath);
	};
}