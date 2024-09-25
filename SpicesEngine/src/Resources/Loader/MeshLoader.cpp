/**
* @file MeshLoader.cpp.
* @brief The MeshLoader Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "MeshLoader.h"
#include "Resources/Mesh/MeshPack.h"
#include "Core/Library/FileLibrary.h"
#include "Core/Library/StringLibrary.h"
#include "Systems/ResourceSystem.h"
#include "Resources/Mesh/MeshProcessor.h"

#include "tiny_obj_loader.h"

namespace Spices {

	/**
	* @brief Const variable: Bin Mesh File Path.
	*/
	const std::string defaultBinMeshPath = "Meshes/bin/";

	/**
	* @brief Const variable: OBJ Mesh File Path.
	*/
	const std::string defaultOBJMeshPath = "Meshes/src/obj/";

	/**
	* @brief Const variable: FBX Mesh File Path.
	*/
	const std::string defaultFBXMeshPath = "Meshes/src/fbx/";

	/**
	* @brief Const variable: Mesh File Confirm header staer.
	*/
	const char MeshLoaderSignSatrt[100] = "#ItisSpicesMeshSign: DataStart";

	/**
	* @brief Const variable: Mesh File Confirm header over.
	*/
	const char MeshLoaderSignOver[100] = "#ItisSpicesMeshSign: DateOver";

	bool MeshLoader::Load(const std::string& fileName, MeshPack* outMeshPack)
	{
		SPICES_PROFILE_ZONE;

		if      ( LoadFromSASSET(fileName, outMeshPack)) return true;
		else if ( LoadFromOBJ(   fileName, outMeshPack)) return true;
		else if ( LoadFromFBX(   fileName, outMeshPack)) return true;
		else return false;
	}

	bool MeshLoader::LoadFromOBJ(const std::string& fileName, MeshPack* outMeshPack)
	{
		SPICES_PROFILE_ZONE;

		bool isFind = false;
		std::string filePath;
		int index = 0;
		for (auto& it : ResourceSystem::GetSearchFolder())
		{
			filePath = it + defaultOBJMeshPath + fileName + ".obj";
			if (FileLibrary::FileLibrary_Exists(filePath.c_str()))
			{
				isFind = true;
				break;
			}
			index++;
		}
		if (!isFind) return false;
		
		if (!FileLibrary::FileLibrary_Exists(filePath.c_str())) {
			return false;
		}
		
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;
		
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str()))
		{
			return false;
		}
		
		outMeshPack->m_MeshResource.positions.attributes->resize(attrib.vertices.size() / 3);
		for (uint32_t i = 0; i < attrib.vertices.size() / 3; i++)
		{
			(*outMeshPack->m_MeshResource.positions.attributes)[i].x =  attrib.vertices[3 * i + 0];
			(*outMeshPack->m_MeshResource.positions.attributes)[i].y =  attrib.vertices[3 * i + 1];
			(*outMeshPack->m_MeshResource.positions.attributes)[i].z = -attrib.vertices[3 * i + 2];
		}
		if (attrib.vertices.size() == 0)
		{
			outMeshPack->m_MeshResource.positions.attributes->resize(1);

			(*outMeshPack->m_MeshResource.positions.attributes)[0].x = 0.0f;
			(*outMeshPack->m_MeshResource.positions.attributes)[0].y = 0.0f;
			(*outMeshPack->m_MeshResource.positions.attributes)[0].z = 0.0f;
		}

		outMeshPack->m_MeshResource.normals.attributes->resize(attrib.normals.size() / 3);
		for (uint32_t i = 0; i < attrib.normals.size() / 3; i++)
		{
			(*outMeshPack->m_MeshResource.normals.attributes)[i].x =  attrib.normals[3 * i + 0];
			(*outMeshPack->m_MeshResource.normals.attributes)[i].y =  attrib.normals[3 * i + 1];
			(*outMeshPack->m_MeshResource.normals.attributes)[i].z = -attrib.normals[3 * i + 2];
		}
		if (attrib.normals.size() == 0)
		{
			outMeshPack->m_MeshResource.normals.attributes->resize(1);

			(*outMeshPack->m_MeshResource.normals.attributes)[0].x = 0.0f;
			(*outMeshPack->m_MeshResource.normals.attributes)[0].y = 1.0f;
			(*outMeshPack->m_MeshResource.normals.attributes)[0].z = 0.0f;
		}

		outMeshPack->m_MeshResource.colors.attributes->resize(attrib.colors.size() / 3);
		for (uint32_t i = 0; i < attrib.colors.size() / 3; i++)
		{
			(*outMeshPack->m_MeshResource.colors.attributes)[i].x = attrib.colors[3 * i + 0];
			(*outMeshPack->m_MeshResource.colors.attributes)[i].y = attrib.colors[3 * i + 1];
			(*outMeshPack->m_MeshResource.colors.attributes)[i].z = attrib.colors[3 * i + 2];
		}
		if (attrib.colors.size() == 0)
		{
			outMeshPack->m_MeshResource.colors.attributes->resize(1);

			(*outMeshPack->m_MeshResource.colors.attributes)[0].x = 1.0f;
			(*outMeshPack->m_MeshResource.colors.attributes)[0].y = 1.0f;
			(*outMeshPack->m_MeshResource.colors.attributes)[0].z = 1.0f;
		}

		outMeshPack->m_MeshResource.texCoords.attributes->resize(attrib.texcoords.size() / 2);
		for (uint32_t i = 0; i < attrib.texcoords.size() / 2; i++)
		{
			(*outMeshPack->m_MeshResource.texCoords.attributes)[i].x =        attrib.texcoords[2 * i + 0];
			(*outMeshPack->m_MeshResource.texCoords.attributes)[i].y = 1.0f - attrib.texcoords[2 * i + 1];
		}
		if (attrib.texcoords.size() == 0)
		{
			outMeshPack->m_MeshResource.texCoords.attributes->resize(1);

			(*outMeshPack->m_MeshResource.texCoords.attributes)[0].x = 0.0f;
			(*outMeshPack->m_MeshResource.texCoords.attributes)[0].y = 0.0f;
		}

		std::unordered_map<glm::uvec4, uint32_t> verticesMap;
		for (const auto& shape : shapes)
		{
			uint32_t start = outMeshPack->m_MeshResource.primitiveVertices.attributes->size();
			outMeshPack->m_MeshResource.primitiveVertices.attributes->resize(start + shape.mesh.indices.size() / 3);

			for (uint32_t i = 0; i < shape.mesh.indices.size() / 3; i++)
			{
				std::array<glm::uvec4, 3> vertexArray;
				for (uint32_t j = 0; j < 3; j++)
				{
					const auto& index = shape.mesh.indices[3 * i + j];

					vertexArray[j] = glm::uvec4(
						index.vertex_index   == -1 ? 0 : index.vertex_index,
						index.normal_index   == -1 ? 0 : index.normal_index,
						index.vertex_index   == -1 ? 0 : index.vertex_index,
						index.texcoord_index == -1 ? 0 : index.texcoord_index
					);

					if (verticesMap.count(vertexArray[j]) == 0)
					{
						verticesMap[vertexArray[j]] = static_cast<uint32_t>(outMeshPack->m_MeshResource.vertices.attributes->size());
						outMeshPack->m_MeshResource.vertices.attributes->push_back(vertexArray[j]);
					}
				}

				(*outMeshPack->m_MeshResource.primitiveVertices.attributes)[start + i].x = verticesMap[vertexArray[0]];
				(*outMeshPack->m_MeshResource.primitiveVertices.attributes)[start + i].y = verticesMap[vertexArray[1]];
				(*outMeshPack->m_MeshResource.primitiveVertices.attributes)[start + i].z = verticesMap[vertexArray[2]];
			}
		}

		MeshProcessor::GenerateMeshLodClusterHierarchy(outMeshPack);
		WriteSASSET(index, fileName, outMeshPack);

		return true;
	}

	bool MeshLoader::LoadFromFBX(const std::string& fileName, MeshPack* outMeshPack)
	{
		SPICES_PROFILE_ZONE;

		bool isFind = false;
		std::string filePath;
		for (auto& it : ResourceSystem::GetSearchFolder())
		{
			filePath = it + defaultFBXMeshPath + fileName + ".fbx";
			if (FileLibrary::FileLibrary_Exists(filePath.c_str()))
			{
				isFind = true;
				break;
			}
		}
		if (!isFind) return false;

		// TODO: 
		return false;
	}

	bool MeshLoader::LoadFromSASSET(const std::string& fileName, MeshPack* outMeshPack)
	{
		SPICES_PROFILE_ZONE;

		bool isFind = false;
		std::string filePath;
		for (auto& it : ResourceSystem::GetSearchFolder())
		{
			filePath = it + defaultBinMeshPath + fileName + ".sasset";
			if (FileLibrary::FileLibrary_Exists(filePath.c_str()))
			{
				isFind = true;
				break;
			}
		}
		if (!isFind) return false;

		if (!FileLibrary::FileLibrary_Exists(filePath.c_str())) {
			return false;
		}

		FileHandle f;
		FileLibrary::FileLibrary_Open(filePath.c_str(), FILE_MODE_READ, true, &f);

		uint64_t readed = 0;

		char startSign[100];
		FileLibrary::FileLibrary_Read(&f, sizeof(char) * 100, &startSign, &readed);

		if (!StringLibrary::StringsEqual(startSign, MeshLoaderSignSatrt))
		{
			FileLibrary::FileLibrary_Close(&f);
			return false;
		}

		uint32_t positionsCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &positionsCount, &readed);
		outMeshPack->m_MeshResource.positions.attributes->resize(positionsCount);
		
		uint32_t normalsCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &normalsCount, &readed);
		outMeshPack->m_MeshResource.normals.attributes->resize(normalsCount);

		uint32_t colorsCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &colorsCount, &readed);
		outMeshPack->m_MeshResource.colors.attributes->resize(colorsCount);

		uint32_t texCoordsCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &texCoordsCount, &readed);
		outMeshPack->m_MeshResource.texCoords.attributes->resize(texCoordsCount);

		uint32_t verticesCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &verticesCount, &readed);
		outMeshPack->m_MeshResource.vertices.attributes->resize(verticesCount);

		uint32_t primitivePointsCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &primitivePointsCount, &readed);
		outMeshPack->m_MeshResource.primitivePoints.attributes->resize(primitivePointsCount);

		uint32_t primitiveVerticesCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &primitiveVerticesCount, &readed);
		outMeshPack->m_MeshResource.primitiveVertices.attributes->resize(primitiveVerticesCount);

		uint32_t primitiveLocationsCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &primitiveLocationsCount, &readed);
		outMeshPack->m_MeshResource.primitiveLocations.attributes->resize(primitiveLocationsCount);

		uint32_t meshletsCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &meshletsCount, &readed);
		outMeshPack->m_MeshResource.meshlets.attributes->resize(meshletsCount);

		uint32_t lodsCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &lodsCount, &readed);
		outMeshPack->m_MeshResource.lods.attributes->resize(lodsCount);
		
		FileLibrary::FileLibrary_Read(&f, sizeof(glm::vec3)  * positionsCount          , outMeshPack->m_MeshResource.positions.attributes          ->data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(glm::vec3)  * normalsCount            , outMeshPack->m_MeshResource.normals.attributes            ->data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(glm::vec3)  * colorsCount             , outMeshPack->m_MeshResource.colors.attributes             ->data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(glm::vec2)  * texCoordsCount          , outMeshPack->m_MeshResource.texCoords.attributes          ->data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(glm::uvec4) * verticesCount           , outMeshPack->m_MeshResource.vertices.attributes           ->data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(glm::uvec3) * primitivePointsCount    , outMeshPack->m_MeshResource.primitivePoints.attributes    ->data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(glm::uvec3) * primitiveVerticesCount  , outMeshPack->m_MeshResource.primitiveVertices.attributes  ->data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(glm::uvec3) * primitiveLocationsCount , outMeshPack->m_MeshResource.primitiveLocations.attributes ->data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(Meshlet)    * meshletsCount           , outMeshPack->m_MeshResource.meshlets.attributes           ->data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(Lod)        * lodsCount               , outMeshPack->m_MeshResource.lods.attributes               ->data(), &readed);

		char overSign[100];
		FileLibrary::FileLibrary_Read(&f, sizeof(char) * 100, &overSign, &readed);

		if (!StringLibrary::StringsEqual(overSign, MeshLoaderSignOver))
		{
			FileLibrary::FileLibrary_Close(&f);
			return false;
		}

		FileLibrary::FileLibrary_Close(&f);

		return true;
	}

	bool MeshLoader::WriteSASSET(int folderIndex, const std::string& fileName, MeshPack* outMeshPack)
	{
		SPICES_PROFILE_ZONE;

		std::string filePath = ResourceSystem::GetSearchFolder()[folderIndex] + defaultBinMeshPath + fileName + ".sasset";

		if (FileLibrary::FileLibrary_Exists(filePath.c_str())) {
			return false;
		}

		FileHandle f;
		FileLibrary::FileLibrary_Open(filePath.c_str(), FILE_MODE_WRITE, true, &f);

		uint64_t written = 0;

		FileLibrary::FileLibrary_Write(&f, sizeof(char) * 100, &MeshLoaderSignSatrt, &written);

		uint32_t positionsCount = (uint32_t)outMeshPack->m_MeshResource.positions.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &positionsCount, &written);
		
		uint32_t normalsCount = (uint32_t)outMeshPack->m_MeshResource.normals.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &normalsCount, &written);

		uint32_t colorsCount = (uint32_t)outMeshPack->m_MeshResource.colors.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &colorsCount, &written);

		uint32_t texCoordsCount = (uint32_t)outMeshPack->m_MeshResource.texCoords.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &texCoordsCount, &written);

		uint32_t verticesCount = (uint32_t)outMeshPack->m_MeshResource.vertices.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &verticesCount, &written);

		uint32_t primitivePointsCount = (uint32_t)outMeshPack->m_MeshResource.primitivePoints.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &primitivePointsCount, &written);

		uint32_t primitiveVerticesCount = (uint32_t)outMeshPack->m_MeshResource.primitiveVertices.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &primitiveVerticesCount, &written);
		
		uint32_t primitiveLocationsCount = (uint32_t)outMeshPack->m_MeshResource.primitiveLocations.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &primitiveLocationsCount, &written);

		uint32_t meshletsCount = (uint32_t)outMeshPack->m_MeshResource.meshlets.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &meshletsCount, &written);
		
		uint32_t lodsCount = (uint32_t)outMeshPack->m_MeshResource.lods.attributes->size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &lodsCount, &written);

		FileLibrary::FileLibrary_Write(&f, sizeof(glm::vec3)  * positionsCount          , outMeshPack->m_MeshResource.positions.attributes          ->data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(glm::vec3)  * normalsCount            , outMeshPack->m_MeshResource.normals.attributes            ->data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(glm::vec3)  * colorsCount             , outMeshPack->m_MeshResource.colors.attributes             ->data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(glm::vec2)  * texCoordsCount          , outMeshPack->m_MeshResource.texCoords.attributes          ->data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(glm::uvec4) * verticesCount           , outMeshPack->m_MeshResource.vertices.attributes           ->data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(glm::uvec3) * primitivePointsCount    , outMeshPack->m_MeshResource.primitivePoints.attributes    ->data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(glm::uvec3) * primitiveVerticesCount  , outMeshPack->m_MeshResource.primitiveVertices.attributes  ->data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(glm::uvec3) * primitiveLocationsCount , outMeshPack->m_MeshResource.primitiveLocations.attributes ->data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(Meshlet)    * meshletsCount           , outMeshPack->m_MeshResource.meshlets.attributes           ->data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(Lod)        * lodsCount               , outMeshPack->m_MeshResource.lods.attributes               ->data(), &written);

		FileLibrary::FileLibrary_Write(&f, sizeof(char) * 100, &MeshLoaderSignOver, &written);

		FileLibrary::FileLibrary_Close(&f);

		return true;
	}
}