/**
* @file GltfLoader.cpp.
* @brief The GltfLoader Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "GltfLoader.h"
#include "../Mesh/GltfPack.h"

#include <nlohmann/json.hpp>

namespace Spices {

	using Json = nlohmann::json;

	bool GltfLoader::Load(const std::string& fileName, GltfCollection* collection)
	{
		SPICES_PROFILE_ZONE;

		std::ifstream f(fileName);
		if (!f)
		{
			std::stringstream ss;
			ss << "Gltf File: " << fileName << "  is not found.";

			SPICES_CORE_ERROR(ss.str())
			return false;
		}

		Json data;
		f >> data;

		std::filesystem::path parentPath = std::filesystem::path(fileName).parent_path();

		SPICES_CORE_INFO("----------------------------------------------------------------");

		if(data.find("asset")       != data.end()) collection->m_Asset       = std::make_unique<GltfAsset      >(data["asset"]);
		if(data.find("accessors")   != data.end()) collection->m_Accessors   = std::make_unique<GltfAccessors  >(data["accessors"]);
		if(data.find("buffers")     != data.end()) collection->m_Buffers     = std::make_unique<GltfBuffers    >(data["buffers"], parentPath);
		if(data.find("bufferViews") != data.end()) collection->m_BufferViews = std::make_unique<GltfBufferViews>(data["bufferViews"]);

		if(data.find("images")      != data.end()) collection->m_Images      = std::make_unique<GltfImages     >(data["images"], parentPath);
		if(data.find("materials")   != data.end()) collection->m_Materials   = std::make_unique<GltfMaterials  >(data["materials"]);
		if(data.find("meshes")      != data.end()) collection->m_Meshes      = std::make_unique<GltfMeshes     >(data["meshes"]);
		if(data.find("nodes")       != data.end()) collection->m_Nodes       = std::make_unique<GltfNodes      >(data["nodes"]);
		if(data.find("samplers")    != data.end()) collection->m_Samplers    = std::make_unique<GltfSamplers   >(data["samplers"]);
		if(data.find("scene")       != data.end()) collection->m_Scene       = std::make_unique<GltfScene      >(data["scene"]);
		if(data.find("scenes")      != data.end()) collection->m_Scenes      = std::make_unique<GltfScenes     >(data["scenes"]);
		if(data.find("textures")    != data.end()) collection->m_Textures    = std::make_unique<GltfTextures   >(data["textures"]);

		SPICES_CORE_INFO("----------------------------------------------------------------");

		return true;
	}

	bool GltfLoader::LoadPack(GltfPack* pack, const GltfMeshes::Primitive& primitive, GltfAccessors* accessors, GltfBuffers* buffers, GltfBufferViews* bufferViews)
	{
		SPICES_PROFILE_ZONE;

		// Positions
		{
			if (primitive.POSITION < 0)
			{
				SPICES_CORE_ERROR("Positions buffer invalid.")
				return false;
			}
			const GltfAccessors::Item& positionAccessor     = accessors->m_AccessorsData[primitive.POSITION];
			const GltfBufferViews::Item& positionBufferView = bufferViews->m_BufferViewsData[positionAccessor.bufferView];
			const GltfBuffers::Item& positionBuffer         = buffers->m_BuffersData[positionBufferView.buffer];

			const VkFormat format                           = GltfHelper::GetFormat(positionAccessor.type, positionAccessor.componentType);
			const uint32_t bytes                            = GltfHelper::SizeOfFormat(format);

			pack->m_MeshResource.positions.attributes->resize(positionAccessor.count);
			for (uint32_t i = 0; i < pack->m_MeshResource.positions.attributes->size(); i++)
			{
				const uint32_t offset = i * bytes;

				auto p = reinterpret_cast<glm::vec3*>(&(*positionBuffer.buffer)[positionAccessor.byteOffset + positionBufferView.byteOffset + offset]);
				(*pack->m_MeshResource.positions.attributes)[i] = *p;

				glm::vec3& pos = (*pack->m_MeshResource.positions.attributes)[i];
				pos.z = -pos.z;
			}
		}

		// Normals
		bool hasNormal = false;
		{
			if (primitive.NORMAL < 0)
			{
				  pack->m_MeshResource.normals.attributes->resize(1);
				(*pack->m_MeshResource.normals.attributes)[0] = glm::vec3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				hasNormal = true;

				const GltfAccessors::Item& normalAccessor     = accessors->m_AccessorsData[primitive.NORMAL];
				const GltfBufferViews::Item& normalBufferView = bufferViews->m_BufferViewsData[normalAccessor.bufferView];
				const GltfBuffers::Item& normalBuffer         = buffers->m_BuffersData[normalBufferView.buffer];

				const VkFormat format                         = GltfHelper::GetFormat(normalAccessor.type, normalAccessor.componentType);
				const uint32_t bytes                          = GltfHelper::SizeOfFormat(format);

				pack->m_MeshResource.normals.attributes->resize(normalAccessor.count);
				for (uint32_t i = 0; i < pack->m_MeshResource.normals.attributes->size(); i++)
				{
					const uint32_t offset = i * bytes;

					auto n = reinterpret_cast<glm::vec3*>(&(*normalBuffer.buffer)[normalAccessor.byteOffset + normalBufferView.byteOffset + offset]);
					(*pack->m_MeshResource.normals.attributes)[i] = *n;

					glm::vec3& nor = (*pack->m_MeshResource.normals.attributes)[i];
					nor.z = -nor.z;
				}
			}
		}
		
		// Colors
		bool hasColor = false;
		{
			  pack->m_MeshResource.colors.attributes->resize(1);
			(*pack->m_MeshResource.colors.attributes)[0] = glm::vec3(0.0f);
		}

		// TexCoords
		bool hasTexCoords = false;
		{
			if (primitive.TEXCOORD_0 < 0)
			{
				  pack->m_MeshResource.texCoords.attributes->resize(1);
				(*pack->m_MeshResource.texCoords.attributes)[0] = glm::vec2(0.0f);
			}
			else
			{
				hasTexCoords = true;

				const GltfAccessors::Item& texCoordAccessor     = accessors->m_AccessorsData[primitive.TEXCOORD_0];
				const GltfBufferViews::Item& texCoordBufferView = bufferViews->m_BufferViewsData[texCoordAccessor.bufferView];
				const GltfBuffers::Item& texCoordBuffer         = buffers->m_BuffersData[texCoordBufferView.buffer];

				const VkFormat format                           = GltfHelper::GetFormat(texCoordAccessor.type, texCoordAccessor.componentType);
				const uint32_t bytes                            = GltfHelper::SizeOfFormat(format);

				pack->m_MeshResource.texCoords.attributes->resize(texCoordAccessor.count);
				for (uint32_t i = 0; i < pack->m_MeshResource.texCoords.attributes->size(); i++)
				{
					const uint32_t offset = i * bytes;

					auto u = reinterpret_cast<glm::vec2*>(&(*texCoordBuffer.buffer)[texCoordAccessor.byteOffset + texCoordBufferView.byteOffset + offset]);
					(*pack->m_MeshResource.texCoords.attributes)[i] = *u;

					glm::vec2& uv = (*pack->m_MeshResource.texCoords.attributes)[i];
					//uv.y = 1.0f - uv.y;
				}
			}
		}

		// Vertices
		{
			const GltfAccessors::Item& positionAccessor = accessors->m_AccessorsData[primitive.POSITION];

			pack->m_MeshResource.vertices.attributes->resize(positionAccessor.count);
			
			for(uint32_t i = 0; i < pack->m_MeshResource.vertices.attributes->size(); i++)
			{
				(*pack->m_MeshResource.vertices.attributes)[i] = glm::uvec4(i, hasNormal ? i : 0, hasColor ? i : 0, hasTexCoords ? i: 0);
			}
		}

		// primVertices
		{
			if (primitive.indices < 0)
			{
				SPICES_CORE_ERROR("indices buffer invalid.")
				return false;
			}
			const GltfAccessors::Item& indicesAccessor     = accessors->m_AccessorsData[primitive.indices];
			const GltfBufferViews::Item& indicesBufferView = bufferViews->m_BufferViewsData[indicesAccessor.bufferView];
			const GltfBuffers::Item& indicesBuffer         = buffers->m_BuffersData[indicesBufferView.buffer];

			const VkFormat format                          = GltfHelper::GetFormat(indicesAccessor.type, indicesAccessor.componentType);
			const uint32_t bytes                           = GltfHelper::SizeOfFormat(format);

			pack->m_MeshResource.primitiveVertices.attributes->resize(indicesAccessor.count / 3);
			for (uint32_t i = 0; i < pack->m_MeshResource.primitiveVertices.attributes->size(); i++)
			{
				const uint32_t offset = i * 3 * bytes;

				auto x = reinterpret_cast<unsigned short*>(&(*indicesBuffer.buffer)[indicesAccessor.byteOffset + indicesBufferView.byteOffset + offset + 0]);
				auto y = reinterpret_cast<unsigned short*>(&(*indicesBuffer.buffer)[indicesAccessor.byteOffset + indicesBufferView.byteOffset + offset + bytes]);
				auto z = reinterpret_cast<unsigned short*>(&(*indicesBuffer.buffer)[indicesAccessor.byteOffset + indicesBufferView.byteOffset + offset + bytes * 2]);

				(*pack->m_MeshResource.primitiveVertices.attributes)[i] = glm::uvec3(*x, *y, *z);
			}
		}

		return true;
	}

	std::shared_ptr<Material> GltfLoader::LoadMaterial(const GltfMaterials::Item& material, GltfImages* images)
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << "BasePassRenderer.Mesh." << material.name;

		if(ResourcePool<Material>::Has(ss.str()))
		{
			return ResourcePool<Material>::Access(ss.str());
		}
		
		auto outMaterial = std::make_shared<Material>();

		ResourcePool<Material>::Registry(ss.str(), outMaterial);

		outMaterial->SetName(ss.str());
		
		outMaterial->PushToShaderPath("task", "BasePassRenderer.Mesh.Default");
		outMaterial->PushToShaderPath("mesh", "BasePassRenderer.Mesh.Default");
		outMaterial->PushToShaderPath("frag", "BasePassRenderer.Mesh.PBRGltf");
		outMaterial->PushToShaderPath("rchit", "BasePassRenderer.Mesh.PBRGltf");
		
		outMaterial->PushToTextureParams("baseColorTexture", {"Texture2D", material.baseColorTexture.has_value() ? images->m_ImagesData[material.baseColorTexture.value()].uri : "" });
		outMaterial->PushToTextureParams("metallicRoughnessTexture", {"Texture2D",  material.metallicRoughnessTexture.has_value() ? images->m_ImagesData[material.metallicRoughnessTexture.value()].uri : "" });
		outMaterial->PushToTextureParams("normalTexture", {"Texture2D", material.normalTexture.has_value() ? images->m_ImagesData[material.normalTexture.value()].uri : ""});
		outMaterial->PushToTextureParams("emissiveTexture", {"Texture2D", material.emissiveTexture.has_value() ? images->m_ImagesData[material.emissiveTexture.value()].uri : "" });
		outMaterial->PushToTextureParams("occlusionTexture", {"Texture2D", material.occlusionTexture.has_value() ? images->m_ImagesData[material.occlusionTexture.value()].uri : "" });
		
		outMaterial->PushToConstParams("baseColorFactor", {"float4", material.baseColorFactor});
		outMaterial->PushToConstParams("emissiveFactor", {"float4", material.emissiveFactor});

		outMaterial->PushToConstParams("maxRayDepth", {"int", 3});
		outMaterial->PushToConstParams("maxLightDepth", {"int", 2});
		outMaterial->PushToConstParams("maxShadowDepth", {"int", 1});
		
		return outMaterial;
	}
}