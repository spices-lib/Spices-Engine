#include "Pchheader.h"
#include "GltfLoader.h"

#include <nlohmann/json.hpp>

namespace Spices {

	using json = nlohmann::json;

	bool GltfLoader::Load(const std::string& fileName)
	{
		std::ifstream f(fileName);

		json j3;
		f >> j3;

		// Load Buffers
		std::vector<char*> m_buffersData;
		if (j3.find("buffers") != j3.end())
		{
			const json& buffers = j3["buffers"];
			m_buffersData.resize(buffers.size());
			for (int i = 0; i < buffers.size(); i++)
			{
				const std::string& name = buffers[i]["uri"];
				std::ifstream ff(fileName, std::ios::in | std::ios::binary);

				ff.seekg(0, ff.end);
				std::streamoff length = ff.tellg();
				ff.seekg(0, ff.beg);

				char* p = new char[length];
				ff.read(p, length);
				m_buffersData[i] = p;
			}
		}

		// Load Meshes
		const json* m_pAccessors;
		const json* m_pBufferViews;
		m_pAccessors = &j3["accessors"];
		m_pBufferViews = &j3["bufferViews"];
		const json& meshes = j3["meshes"];
		std::vector<tfMesh> m_meshes;
		m_meshes.resize(meshes.size());
		for (int i = 0; i < meshes.size(); i++)
		{
			tfMesh* tfmesh = &m_meshes[i];
			auto& primitives = meshes[i]["primitives"];
			tfmesh->m_primitives.resize(primitives.size());
			for (int p = 0; p < primitives.size(); p++)
			{
				tfPrimitives* pPrimitive = &tfmesh->m_pPrimitives[p];

				int positionId = primitives[p]["attributes"]["POSITION"];
			}
		}


		return false;
	}
}