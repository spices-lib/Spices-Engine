/**
* @file MeshLoader.h.
* @brief The MeshLoader Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief Forward declare.
	*/
	class MeshPack;

	/**
	* @brief This enum defines tree types of mesh file.
	*/
	enum MeshExtension
	{
		/**
		* @brief error type
		*/
		UNKNOWN = 0,

		/**
		* @brief obj type
		*/
		OBJ = 1,

		/**
		* @brief fbx type
		*/
		FBX = 2,

		/**
		* @brief binary type
		*/
		SASSET = 3,
	};
	
	/**
	* @brief MeshLoader Class.
	* This class only defines static function for load data from mesh file.
	*/
	class MeshLoader
	{
	public:

		/**
		* @brief Public called API, it is entrance.
		* @param[in] fileName meshfile name.
		* @param[in,out] outMeshPack meshpack pointer, only pass this to it.
		* Feel free for using row pointer here, it's very safe, for this API only called during MeshPack.
		* @return Returns true if load data succssfully.
		*/
		static bool Load(const std::string& fileName, MeshPack* outMeshPack);

	private:

		/**
		* @brief Load data from a .obj file.
		* @param[in] filepath Mesh file path in disk.
		* @param[in,out] outMeshPack meshpack pointer, only pass this to it.
		* @return Returns true if load data succssfully.
		*/
		static bool LoadFromOBJ(const std::string& filepath, MeshPack* outMeshPack);

		/**
		* @brief Load data from a .fbx file.
		* @param[in] filepath Mesh file path in disk.
		* @param[in,out] outMeshPack meshpack pointer, only pass this to it.
		* @return Returns true if load data succssfully.
		*/
		static bool LoadFromFBX(const std::string& filepath, MeshPack* outMeshPack);

		/**
		* @brief Load data from a .sasset file.
		* @param[in] filepath Mesh file path in disk.
		* @param[in,out] outMeshPack meshpack pointer, only pass this to it.
		* @return Returns true if load data succssfully.
		*/
		static bool LoadFromSASSET(const std::string& filepath, MeshPack* outMeshPack);

		/**
		* @brief Write the readed data to the sasset file.
		* @param[in] filepath Mesh file path in disk.
		* @param[in,out] outMeshPack meshpack pointer, only pass this to it.
		* @return Returns true if write data succssfully.
		*/
		static bool WriteSASSET(const std::string& filepath, MeshPack* outMeshPack);

		/**
		* @brief Get Bin Mesh Path.
		* @param[in] filePath Mesh file path(short path).
		*/
		static void GetBinPath(std::string& filePath);
	};
}