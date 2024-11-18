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
			ss << "Gltf File: " << fileName << "  is not finded.";

			SPICES_CORE_ERROR(ss.str())
			return false;
		}

		Json data;
		f >> data;

		std::filesystem::path parentPath = std::filesystem::path(fileName).parent_path();

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

		return true;
	}

	bool GltfLoader::LoadPack(GltfPack* pack, const GltfMeshes::Primitive& primitive, GltfAccessors* accessors, GltfBuffers* buffers, GltfBufferViews* bufferViews)
	{
		SPICES_PROFILE_ZONE;

		// Positions
		{
			GltfAccessors::Item& positionAccessor     = accessors->m_AccessorsData[primitive.POSITION];
			GltfBufferViews::Item& positionBufferView = bufferViews->m_BufferViewsData[positionAccessor.bufferView];
			GltfBuffers::Item& positionBuffer         = buffers->m_BuffersData[positionBufferView.buffer];

			VkFormat format = GltfHelper::GetFormat(positionAccessor.type, positionAccessor.componentType);
			uint32_t bytes = GltfHelper::SizeOfFormat(format);

			pack->m_MeshResource.positions.attributes->resize(positionAccessor.count);
			for (uint32_t i = 0; i < pack->m_MeshResource.positions.attributes->size(); i++)
			{
				const uint32_t offset = i * bytes;

				glm::vec3* p = (glm::vec3*)&(*positionBuffer.buffer)[positionAccessor.byteOffset + positionBufferView.byteOffset + offset];
				(*pack->m_MeshResource.positions.attributes)[i] = *p;

				glm::vec3& pos = (*pack->m_MeshResource.positions.attributes)[i];
				pos.z = -pos.z;
			}
		}

		// Normals
		{
			GltfAccessors::Item& normalAccessor     = accessors->m_AccessorsData[primitive.NORMAL];
			GltfBufferViews::Item& normalBufferView = bufferViews->m_BufferViewsData[normalAccessor.bufferView];
			GltfBuffers::Item& normalBuffer         = buffers->m_BuffersData[normalBufferView.buffer];

			VkFormat format = GltfHelper::GetFormat(normalAccessor.type, normalAccessor.componentType);
			uint32_t bytes = GltfHelper::SizeOfFormat(format);

			pack->m_MeshResource.normals.attributes->resize(normalAccessor.count);
			for (uint32_t i = 0; i < pack->m_MeshResource.normals.attributes->size(); i++)
			{
				const uint32_t offset = i * bytes;

				glm::vec3* n = (glm::vec3*)&(*normalBuffer.buffer)[normalAccessor.byteOffset + normalBufferView.byteOffset + offset];
				(*pack->m_MeshResource.normals.attributes)[i] = *n;

				glm::vec3& nor = (*pack->m_MeshResource.normals.attributes)[i];
				nor.z = -nor.z;
			}
		}
		
		// Colors
		{
			pack->m_MeshResource.colors.attributes->resize(1);
			(*pack->m_MeshResource.colors.attributes)[0] = glm::vec3(0.0f);
		}

		// TexCoords
		{
			GltfAccessors::Item& texCoordAccessor     = accessors->m_AccessorsData[primitive.TEXCOORD_0];
			GltfBufferViews::Item& texCoordBufferView = bufferViews->m_BufferViewsData[texCoordAccessor.bufferView];
			GltfBuffers::Item& texCoordBuffer         = buffers->m_BuffersData[texCoordBufferView.buffer];

			VkFormat format = GltfHelper::GetFormat(texCoordAccessor.type, texCoordAccessor.componentType);
			uint32_t bytes = GltfHelper::SizeOfFormat(format);

			pack->m_MeshResource.texCoords.attributes->resize(texCoordAccessor.count);
			for (uint32_t i = 0; i < pack->m_MeshResource.texCoords.attributes->size(); i++)
			{
				const uint32_t offset = i * bytes;

				glm::vec2* u = (glm::vec2*)&(*texCoordBuffer.buffer)[texCoordAccessor.byteOffset + texCoordBufferView.byteOffset + offset];
				(*pack->m_MeshResource.texCoords.attributes)[i] = *u;

				glm::vec2& uv = (*pack->m_MeshResource.texCoords.attributes)[i];
				uv.y = 1.0f - uv.y;
			}
		}

		// Vertices
		{
			GltfAccessors::Item& positionAccessor = accessors->m_AccessorsData[primitive.POSITION];

			pack->m_MeshResource.vertices.attributes->resize(positionAccessor.count);
			
			for(uint32_t i = 0; i < pack->m_MeshResource.vertices.attributes->size(); i++)
			{
				(*pack->m_MeshResource.vertices.attributes)[i] = glm::uvec4(i, i, 0, i);
			}
		}

		// primVertices
		{
			GltfAccessors::Item& indicesAccessor     = accessors->m_AccessorsData[primitive.indices];
			GltfBufferViews::Item& indicesBufferView = bufferViews->m_BufferViewsData[indicesAccessor.bufferView];
			GltfBuffers::Item& indicesBuffer         = buffers->m_BuffersData[indicesBufferView.buffer];

			VkFormat format = GltfHelper::GetFormat(indicesAccessor.type, indicesAccessor.componentType);
			uint32_t bytes = GltfHelper::SizeOfFormat(format);

			pack->m_MeshResource.primitiveVertices.attributes->resize(indicesAccessor.count / 3);
			for (uint32_t i = 0; i < pack->m_MeshResource.primitiveVertices.attributes->size(); i++)
			{
				const uint32_t offset = i * 3 * bytes;

				short* x = (short*)&(*indicesBuffer.buffer)[indicesAccessor.byteOffset + indicesBufferView.byteOffset + offset + 0];
				short* y = (short*)&(*indicesBuffer.buffer)[indicesAccessor.byteOffset + indicesBufferView.byteOffset + offset + bytes];
				short* z = (short*)&(*indicesBuffer.buffer)[indicesAccessor.byteOffset + indicesBufferView.byteOffset + offset + bytes * 2];

				(*pack->m_MeshResource.primitiveVertices.attributes)[i] = glm::uvec3(*x, *y, *z);
			}
		}

		return true;
	}

	std::shared_ptr<Material> GltfLoader::LoadMaterial(const GltfMaterials::Item& material, GltfImages* images)
	{
		SPICES_PROFILE_ZONE;

		std::shared_ptr<Material> outMaterial = std::make_shared<Material>();

		std::stringstream ss;
		ss << "BasePassRenderer.Mesh." << material.name;
		outMaterial->GetName() = ss.str();

		outMaterial->GetShaderPath()["task"] = {"BasePassRenderer.Mesh.Default"};
		outMaterial->GetShaderPath()["mesh"] = {"BasePassRenderer.Mesh.Default"};
		outMaterial->GetShaderPath()["frag"] = {"BasePassRenderer.Mesh.PBRGltf"};
		outMaterial->GetShaderPath()["rchit"] = {"BasePassRenderer.Mesh.PBRGltf"};

		outMaterial->GetTextureParams().push_back("baseColorTexture", { "Texture2D", images->m_ImagesData[material.baseColorTexture].uri });
		outMaterial->GetTextureParams().push_back("metallicRoughnessTexture", { "Texture2D", images->m_ImagesData[material.metallicRoughnessTexture].uri });
		outMaterial->GetTextureParams().push_back("normalTexture", { "Texture2D", images->m_ImagesData[material.normalTexture].uri });

		ConstantParam baseColorFactorParam;
		baseColorFactorParam.paramType  = "float4";
		baseColorFactorParam.paramValue = material.baseColorFactor;
		outMaterial->GetConstantParams().push_back("baseColorFactor", { baseColorFactorParam , baseColorFactorParam });

		ConstantParam maxRayDepthParam;
		maxRayDepthParam.paramType = "int";
		maxRayDepthParam.paramValue = 6;
		outMaterial->GetConstantParams().push_back("maxRayDepth", { maxRayDepthParam , maxRayDepthParam });

		ConstantParam maxLightDepthParam;
		maxLightDepthParam.paramType = "int";
		maxLightDepthParam.paramValue = 3;
		outMaterial->GetConstantParams().push_back("maxLightDepth", { maxLightDepthParam , maxLightDepthParam });

		ConstantParam maxShadowDepthParam;
		maxShadowDepthParam.paramType = "int";
		maxShadowDepthParam.paramValue = 3;
		outMaterial->GetConstantParams().push_back("maxShadowDepth", { maxShadowDepthParam , maxShadowDepthParam });

		ResourcePool<Material>::Registry(ss.str(), outMaterial);

		return outMaterial;
	}
}