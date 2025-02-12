/**
* @file MeshPack.h.
* @brief The MeshPack Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Vertex.h"
#include "Resources/Material/Material.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "MeshProcessor.h"
#include "Attribute.h"

#include <optional>

#ifdef RENDERAPI_VULKAN
#include "Render/Vulkan/VulkanRayTracing.h"
#endif

namespace Spices {
	
	/**
	* @brief Lod Data.
	*/
	struct Lod
	{
		uint32_t primVertexOffset;     /* @brief Lod PrimVertexOffset (also PrimPointOffset).   */
		uint32_t nPrimitives;          /* @brief Lod PrimVertics Count (also PrimPoints Count). */
		uint32_t meshletOffset;        /* @brief Lod MeshletOffset.                             */
		uint32_t nMeshlets;            /* @brief Lod Meshlets Count.                            */
	};

	/**
	* @brief Mesh Resources Data.
	*/
	struct MeshResource
	{
		/**
		* @brief Constructor Function.
		*/
		MeshResource() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~MeshResource() = default;

		/**
		* @brief Declare value Type.
		*/
		using Positions           = Attribute<glm::vec3>;                // @brief Position.(also as Point)
		using Normals             = Attribute<glm::vec3>;                // @brief Normal
		using Colors              = Attribute<glm::vec3>;                // @brief Color
		using TexCoords           = Attribute<glm::vec2>;                // @brief TexCoord
		using Vertices            = Attribute<glm::uvec4>;               // @brief Vertex.

		using PrimitivePoints     = Attribute<glm::uvec3>;               // @brief PrimPoints
		using PrimitiveVertices   = Attribute<glm::uvec3>;               // @brief PrimVertices
		using PrimitiveLocations  = Attribute<glm::uvec3>;               // @brief PrimLocation
		using Meshlets            = Attribute<Meshlet>;                  // @brief PrimMeshlet
		using Lods                = Attribute<Lod>;                      // @brief Lod

		/**
		* @brief Declare value.
		*/
		Positions                   positions;
		Normals                     normals;
		Colors                      colors;
		TexCoords                   texCoords;
		Vertices                    vertices;
		PrimitivePoints             primitivePoints;
		PrimitiveVertices           primitiveVertices;
		PrimitiveLocations          primitiveLocations;
		Meshlets                    meshlets;
		Lods                        lods;

		/**
		* @brief Create MeshResource Buffers.
		* @param[in] name MeshPack Name.
		*/
		void CreateBuffer(const std::string& name);
	};

	/**
	* @brief Add Construction Functions to SpicesShader::MeshDesc.
	*/
	struct MeshDesc : public SpicesShader::MeshDesc
	{

#define Update_F(item)                                                                \
		void Update##item(std::shared_ptr<VulkanBuffer> buffer)                       \
		{                                                                             \
			if(buffer)                                                                \
			{                                                                         \
				item = static_cast<decltype(item)>(buffer->GetAddress());             \
			}                                                                         \
			else                                                                      \
			{                                                                         \
				item = static_cast<decltype(item)>(0);                                \
			}                                                                         \
			m_Buffer->WriteToBuffer(&item, sizeof(item), offsetof(MeshDesc, item));   \
		}                                                                             \
		void Update##item(uint64_t p)                                                 \
		{                                                                             \
			item = static_cast<decltype(item)>(p);                                    \
			m_Buffer->WriteToBuffer(&item, sizeof(item), offsetof(MeshDesc, item));   \
		}

		/**
		* @brief Constructor Function.
		*/
		MeshDesc();

		/**
		* @brief Copy a MeshDesc from this.
		*/
		MeshDesc Copy() const;

		/**
		* @brief Get m_Buffer's Address.
		* @return Returns the m_Buffer's Address.
		*/
		uint64_t GetBufferAddress() const;

		/**
		* @brief Update Function.
		*/
		Update_F(modelAddress                   )
		Update_F(positionsAddress               )
		Update_F(normalsAddress                 )
		Update_F(colorsAddress                  )
		Update_F(texCoordsAddress               )
		Update_F(verticesAddress                )
		Update_F(primitivePointsAddress         )
		Update_F(primitiveVerticesAddress       )
		Update_F(primitiveLocationsAddress      )
		Update_F(materialParameterAddress       )
		Update_F(meshletsAddress                )
		Update_F(nMeshlets                      )
		Update_F(entityID                       )
		
		/**
		* @brief Buffer of this.
		*/
		std::shared_ptr<VulkanBuffer> m_Buffer;
	};

	/**
	* @brief MeshPack Class.
	* This class defines some basic behaves and variables.
	* This class need to be inherited while use.
	*/
	class MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] name MeshPack Name.
		* @param[in] instanced Is this maesh pack instanced.
		*/
		MeshPack(const std::string& name, bool instanced);

		/**
		* @brief Destructor Function.
		*/
		virtual ~MeshPack() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		MeshPack(const MeshPack&) = delete;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		MeshPack& operator=(const MeshPack&) = delete;

		/**
		* @brief This interface is used for build specific mesh pack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		* @return Returns true if copy resource from Pool.
		*/
		virtual bool OnCreatePack(bool isCreateBuffer = true);

		/**
		* @brief Set specific material for this class.
		* @param[in] materialPath Material path in disk.
		*/
		void SetMaterial(const std::string& materialPath);

		/**
		* @brief Set specific material for this class.
		* @param[in] material Material Pointer.
		*/
		void SetMaterial(std::shared_ptr<Material> material);

		/**
		* @brief Get material in this class.
		* @return Returns the material in this class.
		*/
		std::shared_ptr<Material> GetMaterial() { return m_Material; }

		/**
		* @brief Set hit shader Handle.
		* @param[in] handle the material handle.
		*/
		void SetHitShaderHandle(uint32_t handle) { m_HitShaderHandle = handle; }

		/**
		* @brief Set hit shader Handle.
		* @param[in] handle the material handle.
		*/
		void SetShaderGroupHandle(uint32_t handle) { m_ShaderGroupHandle = handle; }

		/**
		* @brief Get Hit Shader Handle, which accessed by ray gen shader.
		* @return Returns the material handle.
		*/
		uint32_t GetHitShaderHandle() const;

		/**
		* @brief Get ShaderGroup Handle, which accessed by gdc buffer.
		* @return Returns the material handle.
		*/
		uint32_t GetShaderGroupHandle() const;
		
		/**
		* @brief Get mesh pack UUID.
		*/
		UUID GetUUID() const { return m_UUID; }

		/**
		* @brief Bind VBO and EBO.
		* @param[in] commandBuffer Which command buffer we will submit commands.
		*/
		void OnBind(const VkCommandBuffer& commandBuffer) const;

		/**
		* @brief Draw indexed.
		* @param[in] commandBuffer Which command buffer we will submit commands.
		*/
		void OnDraw(const VkCommandBuffer& commandBuffer) const;

		/**
		* @brief Draw Mesh Tasks.
		* @param[in] commandBuffer Which command buffer we will submit commands.
		*/
		void OnDrawMeshTasks(const VkCommandBuffer& commandBuffer) const;

		/**
		* @brief Get Meshlets array.
		* @return Returns the Meshlets array.
		*/
		const std::vector<Meshlet>& GetMeshlets() const { return *m_MeshResource.meshlets.attributes; }

		/**
		* @brief Get NTasks.
		* @return Returns the NTasks.
		*/
		uint32_t GetNTasks() const { return m_NTasks; }

		/**
		* @brief Get Mesh Description.
		* @return Returns the Mesh Description.
		*/
		MeshDesc& GetMeshDesc() { return m_Desc; }

		/**
		* @brief Get Draw Command.
		* @return Returns Draw Command.
		*/
		const VkDrawMeshTasksIndirectCommandNV& GetDrawCommand() const { return m_MeshTaskIndirectDrawCommand; }

		/**
		* @brief Get Pack Type.
		* @return Returns the Pack Type.
		*/
		const std::string& GetPackType() const { return m_PackType; }

		/**
		* @brief Convert MeshPack into the ray tracing geometry used to build the BLAS.
		* @return Returns VulkanRayTracing::BlasInput.
		*/
		VulkanRayTracing::BlasInput MeshPackToVkGeometryKHR();

		/**
		* @brief Is this meshPack has a valid blas.
		* @return Returns true if has a valid blas.
		*/
		bool HasBlasAccel();

		/**
		* @brief Get this accel.
		* @return Returns this accel.
		*/
		AccelKHR& GetAccel();

		/**
		* @brief Get Resource.
		* @return Returns the Resource.
		*/
		const MeshResource& GetResource() const { return m_MeshResource; }

	protected:

		/**
		* @brief Create Vertices buffer anf Indices buffer.
		*/
		void CreateBuffer();

	protected:

		/**
		* @brief MeshPack Name.
		*/
		std::string m_MeshPackName;

		/**
		* @brief If this mesh pack needs instanced.
		*/
		bool m_Instanced;

		/**
		* @brief Mesh Resources.
		*/
		MeshResource m_MeshResource;

		/**
		* @brief Task Shader Work Group Size.
		*/
		uint32_t m_NTasks;

		/**
		* @brief specific material pointer.
		*/
		std::shared_ptr<Material> m_Material;

		/**
		* @brief specific hit shader handle.
		* Used in RayTracing Pipeline.
		*/
		std::optional<std::atomic_uint32_t> m_HitShaderHandle;

		/**
		* @brief specific shader group handle.
		* Used in IndirectDGCPipeline.
		*/
		std::optional<std::atomic_uint32_t> m_ShaderGroupHandle;

		/**
		* @brief Mesh Description.
		*/
		MeshDesc m_Desc;

		/**
		* @brief specific mesh pack type.
		*/
		std::string m_PackType;

		/**
		* @brief This meshPack blas accel.
		*/
		AccelKHR m_Accel;

		/**
		* @brief True if required accel by BLAS Build.
		*/
		std::atomic_bool m_IsRequiredAccel;
		
		/**
		* @brief UUID for mesh pack.
		*/
		UUID m_UUID;

		/**
		* @brief Draw Command.
		*/
		VkDrawMeshTasksIndirectCommandNV m_MeshTaskIndirectDrawCommand{};

		/**
		* @brief Allow MeshLoader access all data.
		*/
		friend class MeshLoader;
		friend class MeshProcessor;
	};

	/**
	* @brief PlanePack Class.
	* This class defines plane type mesh pack.
	*/
	class PlanePack : public MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member variables.
		* @param[in] rows The rows number.
		* @param[in] columns The columns number.
		* @param[in] instanced Is this mesh pack instanced.
		*/
		PlanePack(uint32_t rows = 2, uint32_t columns = 2, bool instanced = true)
			: MeshPack("Plane", instanced)
			, m_Rows(rows)
			, m_Columns(columns)
		{
			m_PackType = "PlanePack";
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~PlanePack() override = default;

		/**
		* @brief This interface is used for build specific mesh pack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		* @return Returns true if Create Pack successfully.
		*/
		virtual bool OnCreatePack(bool isCreateBuffer = true) override;

	private:

		/**
		* @brief How much rows number we use.
		*/
		uint32_t m_Rows;

		/**
		* @brief How much cols number we use.
		*/
		uint32_t m_Columns;
	};


	/**
	* @brief CubePack Class.
	* This class defines box type mesh pack.
	*/
	class CubePack : public MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member variables.
		* @param[in] rows The rows number.
		* @param[in] columns The columns number.
		* @param[in] instanced Is this mesh pack instanced.
		*/
		CubePack(uint32_t rows = 2, uint32_t columns = 2, bool instanced = true)
			: MeshPack("Cube", instanced)
			, m_Rows(rows)
			, m_Columns(columns)
		{
			m_PackType = "CubePack";
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~CubePack() override = default;

		/**
		* @brief This interface is used for build specific mesh pack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		* @return Returns true if Create Pack successfully.
		*/
		virtual bool OnCreatePack(bool isCreateBuffer = true) override;

	private:

		/**
		* @brief How much rows number we use.
		*/
		uint32_t m_Rows;

		/**
		* @brief How much cols number we use.
		*/
		uint32_t m_Columns;
	};

	/**
	* @brief SpherePack Class.
	* This class defines sphere type mesh pack.
	*/
	class SpherePack : public MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member variables.
		* @param[in] rows The rows number.
		* @param[in] columns The columns number.
		* @param[in] instanced Is this mesh pack instanced.
		*/
		SpherePack(uint32_t rows = 15, uint32_t columns = 24, bool instanced = true)
			: MeshPack("Sphere", instanced)
			, m_Rows(rows)
			, m_Columns(columns)
		{
			m_PackType = "SpherePack";
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~SpherePack() override = default;

		/**
		* @brief This interface is used for build specific mesh pack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		* @return Returns true if Create Pack successfully.
		*/
		virtual bool OnCreatePack(bool isCreateBuffer = true) override;

	private:

		/**
		* @brief How much rows number we use.
		*/
		uint32_t m_Rows;

		/**
		* @brief How much cols number we use.
		*/
		uint32_t m_Columns;
	};

	/**
	* @brief FilePack Class.
	* This class defines file type mesh pack.
	*/
	class FilePack : public MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member variables.
		* @param[in] filePath The mesh file path in disk.
		* @param[in] instanced Is this mesh pack instanced.
		*/
		FilePack(const std::string& filePath, bool instanced = true)
			: MeshPack(filePath, instanced)
			, m_Path(filePath)
		{
			m_PackType = "FilePack";
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~FilePack() override = default;

		/**
		* @brief This interface is used for build specific mesh pack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		* @return Returns true if Create Pack successfully.
		*/
		virtual bool OnCreatePack(bool isCreateBuffer = true) override;

	private:

		/**
		* @brief The mesh file path in disk.
		*/
		std::string m_Path;
	};
}