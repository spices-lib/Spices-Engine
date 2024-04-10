#include "Pchheader.h"
#include "MeshLoader.h"
#include "Render/Resources/MeshPack.h"
#include "Core/Library/FileLibrary.h"
#include "Core/Library/StringLibrary.h"

#include "tiny_obj_loader.h"

namespace Spiecs {

	const std::string defaultBinMeshPath = SPIECS_ENGINE_ASSETS_PATH + "Meshes/bin/";
	const std::string defaultOBJMeshPath = SPIECS_ENGINE_ASSETS_PATH + "Meshes/src/obj/";
	const std::string defaultFBXMeshPath = SPIECS_ENGINE_ASSETS_PATH + "Meshes/src/fbx/";

	const char MeshLoaderSignSatrt[100] = "#ItisSpiecsMeshSign: DataStart";
	const char MeshLoaderSignOver[100] = "#ItisSpiecsMeshSign: DateOver";

	bool MeshLoader::Load(const std::string& fileName, MeshPack* outMeshPack)
	{
		if      ( LoadFromSASSET(  defaultBinMeshPath + fileName + ".sasset", outMeshPack) ) return true;
		else if ( LoadFromOBJ(     defaultOBJMeshPath + fileName + ".obj",    outMeshPack) ) return true;
		else if ( LoadFromFBX(     defaultFBXMeshPath + fileName + ".fbx",    outMeshPack) ) return true;
		else return false;
	}

	bool MeshLoader::LoadFromOBJ(const std::string& filepath, MeshPack* outMeshPack)
	{
		if (!FileLibrary::FileLibrary_Exists(filepath.c_str())) {
			return false;
		}

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
		{
			return false;
		}

		outMeshPack->m_Vertices.clear();
		outMeshPack->m_Indices.clear();

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};
		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				if (index.vertex_index >= 0)
				{
					vertex.position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};

					auto colorIndex = 3 * index.vertex_index + 2;
					if (colorIndex < attrib.colors.size())
					{
						vertex.color = {
							attrib.colors[colorIndex - 2],
							attrib.colors[colorIndex - 1],
							attrib.colors[colorIndex - 0]
						};
					}
					else
					{
						vertex.color = { 1.0f, 1.0f, 1.0f };
					}
				}

				if (index.normal_index >= 0)
				{
					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}

				if (index.texcoord_index >= 0)
				{
					vertex.texCoord = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(outMeshPack->m_Vertices.size());
					outMeshPack->m_Vertices.push_back(vertex);
				}

				outMeshPack->m_Indices.push_back(uniqueVertices[vertex]);
			}
		}

		WriteSASSET(filepath, outMeshPack);

		return true;
	}

	bool MeshLoader::LoadFromFBX(const std::string& filepath, MeshPack* outMeshPack)
	{
		// TODO: 
		return false;
	}

	bool MeshLoader::LoadFromSASSET(const std::string& filepath, MeshPack* outMeshPack)
	{
		if (!FileLibrary::FileLibrary_Exists(filepath.c_str())) {
			return false;
		}

		FileHandle f;
		FileLibrary::FileLibrary_Open(filepath.c_str(), FILE_MODE_READ, true, &f);

		uint64_t readed = 0;

		char startSign[100];
		FileLibrary::FileLibrary_Read(&f, sizeof(char) * 100, &startSign, &readed);

		if (!StringLibrary::StringsEqual(startSign, MeshLoaderSignSatrt))
		{
			FileLibrary::FileLibrary_Close(&f);
			return false;
		}

		uint32_t verticesCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &verticesCount, &readed);
		outMeshPack->m_Vertices.resize(verticesCount);

		uint32_t indicesCount = 0;
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t), &indicesCount, &readed);
		outMeshPack->m_Indices.resize(indicesCount);

		FileLibrary::FileLibrary_Read(&f, sizeof(Vertex) * verticesCount, outMeshPack->m_Vertices.data(), &readed);
		FileLibrary::FileLibrary_Read(&f, sizeof(uint32_t) * indicesCount, outMeshPack->m_Indices.data(), &readed);

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

	bool MeshLoader::WriteSASSET(const std::string& filepath, MeshPack* outMeshPack)
	{
		std::string path = filepath;
		GetBinPath(path);

		if (FileLibrary::FileLibrary_Exists(path.c_str())) {
			return false;
		}

		FileHandle f;
		FileLibrary::FileLibrary_Open(path.c_str(), FILE_MODE_WRITE, true, &f);

		uint64_t written = 0;

		FileLibrary::FileLibrary_Write(&f, sizeof(char) * 100, &MeshLoaderSignSatrt, &written);

		uint32_t verticesCount =  outMeshPack->m_Vertices.size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &verticesCount, &written);

		uint32_t indicesCount = outMeshPack->m_Indices.size();
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t), &indicesCount, &written);

		FileLibrary::FileLibrary_Write(&f, sizeof(Vertex) * verticesCount, outMeshPack->m_Vertices.data(), &written);
		FileLibrary::FileLibrary_Write(&f, sizeof(uint32_t) * indicesCount, outMeshPack->m_Indices.data(), &written);

		FileLibrary::FileLibrary_Write(&f, sizeof(char) * 100, &MeshLoaderSignOver, &written);

		FileLibrary::FileLibrary_Close(&f);

		return true;
	}

	void MeshLoader::GetBinPath(std::string& filePath)
	{
		std::vector<std::string> out1 = StringLibrary::SplitString(filePath, '/');
		std::vector<std::string> out2 = StringLibrary::SplitString(out1[out1.size() - 1], '.');

		filePath = defaultBinMeshPath + out2[0] + ".sasset";
	}

}