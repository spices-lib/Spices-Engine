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
		if      ( LoadFromSASSET(fileName, outMeshPack)) return true;
		else if ( LoadFromOBJ(   fileName, outMeshPack)) return true;
		else if ( LoadFromFBX(   fileName, outMeshPack)) return true;
		else return false;
	}

	bool MeshLoader::LoadFromOBJ(const std::string& fileName, MeshPack* outMeshPack)
	{
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

		//std::unordered_map<Vertex, uint32_t> uniqueVertices{};
		//for (const auto& shape : shapes)
		//{
		//	for (const auto& index : shape.mesh.indices)
		//	{
		//		Vertex vertex{};
		//
		//		if (index.vertex_index >= 0)
		//		{
		//			vertex.position = {
		//				attrib.vertices[3 * index.vertex_index + 0],
		//				attrib.vertices[3 * index.vertex_index + 1],
		//			   -attrib.vertices[3 * index.vertex_index + 2]
		//			};
		//
		//			auto colorIndex = 3 * index.vertex_index + 2;
		//			if (colorIndex < attrib.colors.size())
		//			{
		//				vertex.color = {
		//					attrib.colors[colorIndex - 2],
		//					attrib.colors[colorIndex - 1],
		//					attrib.colors[colorIndex - 0]
		//				};
		//			}
		//			else
		//			{
		//				vertex.color = { 1.0f, 1.0f, 1.0f };
		//			}
		//		}
		//
		//		if (index.normal_index >= 0)
		//		{
		//			vertex.normal = {
		//				attrib.normals[3 * index.normal_index + 0],
		//				attrib.normals[3 * index.normal_index + 1],
		//			   -attrib.normals[3 * index.normal_index + 2]
		//			};
		//		}
		//
		//		if (index.texcoord_index >= 0)
		//		{
		//			vertex.texCoord = {
		//				attrib.texcoords[2 * index.texcoord_index + 0],
		//		 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
		//			};
		//		}
		//
		//		if (uniqueVertices.count(vertex) == 0) {
		//			uniqueVertices[vertex] = static_cast<uint32_t>(outMeshPack->m_Vertices->size());
		//			outMeshPack->m_Vertices->push_back(std::move(vertex));
		//		}
		//
		//		outMeshPack->m_Indices->push_back(uniqueVertices[vertex]);
		//	}
		//}
		//
		//MeshProcessor::GenerateMeshLodClusterHierarchy(outMeshPack);
		//WriteSASSET(index, fileName, outMeshPack);

		return true;
	}

	bool MeshLoader::LoadFromFBX(const std::string& fileName, MeshPack* outMeshPack)
	{
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

		//uint32_t verticesCount = 0;
		//FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &verticesCount, &readed);
		//outMeshPack->m_Vertices->resize(verticesCount);
		//
		//uint32_t indicesCount = 0;
		//FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &indicesCount, &readed);
		//outMeshPack->m_Indices->resize(indicesCount);
		//
		//uint32_t meshletsCount = 0;
		//FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &meshletsCount, &readed);
		//outMeshPack->m_Meshlets->resize(meshletsCount);
		//
		//FileLibrary::FileLibrary_Read(&f, sizeof(Vertex) * verticesCount, outMeshPack->m_Vertices->data(), &readed);
		//FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t) * indicesCount, outMeshPack->m_Indices->data(), &readed);
		//FileLibrary::FileLibrary_Read(&f, sizeof(SpicesShader::Meshlet) * meshletsCount, outMeshPack->m_Meshlets->data(), &readed);

		char overSign[100];
		FileLibrary::FileLibrary_Read(&f, sizeof(char) * 100, &overSign, &readed);

		if (!StringLibrary::StringsEqual(overSign, MeshLoaderSignOver))
		{
			FileLibrary::FileLibrary_Close(&f);
			return false;
		}

		FileLibrary::FileLibrary_Close(&f);

		//outMeshPack->m_NTasks = outMeshPack->m_Meshlets->size() / SUBGROUP_SIZE + 1;
		return true;
	}

	bool MeshLoader::WriteSASSET(int folderIndex, const std::string& fileName, MeshPack* outMeshPack)
	{
		std::string filePath = ResourceSystem::GetSearchFolder()[folderIndex] + defaultBinMeshPath + fileName + ".sasset";

		if (FileLibrary::FileLibrary_Exists(filePath.c_str())) {
			return false;
		}

		FileHandle f;
		FileLibrary::FileLibrary_Open(filePath.c_str(), FILE_MODE_WRITE, true, &f);

		uint64_t written = 0;

		FileLibrary::FileLibrary_Write(&f, sizeof(char) * 100, &MeshLoaderSignSatrt, &written);

		//uint32_t verticesCount = (uint32_t)outMeshPack->m_Vertices->size();
		//FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &verticesCount, &written);
		//
		//uint32_t indicesCount = (uint32_t)outMeshPack->m_Indices->size();
		//FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &indicesCount, &written);
		//
		//uint32_t meshletsCount = (uint32_t)outMeshPack->m_Meshlets->size();
		//FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &meshletsCount, &written);
		//
		//FileLibrary::FileLibrary_Write(&f, sizeof(Vertex) * verticesCount, outMeshPack->m_Vertices->data(), &written);
		//FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t) * indicesCount, outMeshPack->m_Indices->data(), &written);
		//FileLibrary::FileLibrary_Write(&f, sizeof(SpicesShader::Meshlet) * meshletsCount, outMeshPack->m_Meshlets->data(), &written);

		FileLibrary::FileLibrary_Write(&f, sizeof(char) * 100, &MeshLoaderSignOver, &written);

		FileLibrary::FileLibrary_Close(&f);

		return true;
	}
}