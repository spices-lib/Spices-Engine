/**
* @file MeshPack.h.
* @brief The MeshPack Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Vertex.h"
#include "Resources/Material/Material.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Resources/ResourcePool/ResourcePool.h"

#include <optional>

#ifdef RENDERAPI_VULKAN

#include "Render/Vulkan/VulkanRayTracing.h"

#endif

namespace Spiecs {

	/**
	* @brief MeshPack Class.
	* This class defines some basic behaver and variables.
	* This class need to be inherited while use.
	*/
	class MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		MeshPack() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~MeshPack() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		MeshPack(const MeshPack&) = delete;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		MeshPack& operator=(const MeshPack&) = delete;

		/**
		* @brief This interface is used for build specific meshpack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		*/
		virtual void OnCreatePack(bool isCreateBuffer = true) {};

		/**
		* @brief Set specific material for this class.
		* @param[in] materialPath Material path in disk.
		*/
		void SetMaterial(const std::string& materialPath);

		/**
		* @brief Get material in this class.
		* @return Returns the material in this class.
		*/
		inline std::shared_ptr<Material> GetMaterial() { return m_Material; };

		/**
		* @brief Set Material Handle.
		* @param[in] handle the material handle.
		*/
		void SetMaterialHandle(uint32_t handle) { m_MaterialHandle = handle; };

		/**
		* @brief Get Material Handle, which accessed by ray gen shader.
		* @return Returns the material handle.
		*/
		uint32_t GetMaterialHandle();
		
		/**
		* @brief Bind VBO and EBO.
		* @param[in] commandBuffer Which commandbufer we will submit commmand.
		*/
		void OnBind(VkCommandBuffer& commandBuffer);

		/**
		* @brief Draw indexed.
		* @param[in] commandBuffer Which commandbufer we will submit commmand.
		*/
		void OnDraw(VkCommandBuffer& commandBuffer);

		/**
		* @brief Get Vertices array.
		* @return Returns the Vertices array.
		*/
		inline const std::vector<Vertex>& GetVertices() const { return m_Vertices; };

		/**
		* @brief Get Indices array.
		* @return Returns the Indices array.
		*/
		inline const std::vector<uint32_t>& GetIndices() const { return m_Indices; };

#ifdef RENDERAPI_VULKAN

		/**
		* @brief Convert MeshPack into the ray tracing geometry used to build the BLAS.
		* @return Returns VulkanRayTracing::BlasInput.
		*/
		VulkanRayTracing::BlasInput MeshPackToVkGeometryKHR();

		/**
		* @brief Get VerticesBuffer Video memory address.
		* @return Returns the VerticesBuffer Video memory address.
		*/
		VkDeviceAddress GetVerticesBufferAddress() const { return m_VertexBuffer->GetAddress(); };

		/**
		* @brief Get IndicesBuffer Video memory address.
		* @return Returns the IndicesBuffer Video memory address.
		*/
		VkDeviceAddress GetIndicesBufferAddress() const { return m_IndicesBuffer->GetAddress(); };

#endif

	protected:

		/**
		* @brief Create Vertices buffer anf Inddices buffer.
		*/
		void CreateBuffer();

	public:
		/**
		* @brief Transform Vertices Position before CreateBuffer
		* @param[in] matrix Which matrix we want apply.
		*/
		void ApplyMatrix(const glm::mat4& matrix);

		/**
		* @brief Copy this Vertices to another pack
		* @param[out] vertices Which vertices we want Copy to.
		*/
		void CopyToVertices(std::vector<Vertex>& vertices);

		/**
		* @brief Copy this Indices to another pack
		* @param[out] indices Which indices we want Copy to.
		* @param[in] offest How much offest we want apply.
		*/
		void CopyToIndices(std::vector<uint32_t>& indices, uint32_t offest = 0);

	protected:

		/**
		* @brief Vertices array.
		*/
		std::vector<Vertex> m_Vertices{};

		/**
		* @brief Indices array.
		*/
		std::vector<uint32_t> m_Indices{};

		/**
		* @brief Vertices buffer.
		*/
		std::unique_ptr<VulkanBuffer> m_VertexBuffer;

		/**
		* @brief Indices buffer.
		*/
		std::unique_ptr<VulkanBuffer> m_IndicesBuffer;

		/**
		* @brief specific material poinnter.
		*/
		std::shared_ptr<Material> m_Material;

		/**
		* @brief specific material handle.
		*/
		std::optional<uint32_t> m_MaterialHandle;
		
		/**
		* @brief Allow MeshLoader access all data.
		*/
		friend class MeshLoader;
	};

	/**
	* @brief SquarePack Class.
	* This class defines suqare type meshpack.
	*/
	class SquarePack : public MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rows The rows number.
		* @param[in] colums The colums number.
		*/
		SquarePack(uint32_t rows = 2, uint32_t colums = 2)
			: MeshPack(), m_Rows(rows), m_Colums(colums) {};

		/**
		* @brief This interface is used for build specific meshpack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		*/
		virtual void OnCreatePack(bool isCreateBuffer = true) override;

	private:

		/**
		* @brief How much rows number we use.
		*/
		uint32_t m_Rows;

		/**
		* @brief How much cols number we use.
		*/
		uint32_t m_Colums;
	};


	/**
	* @brief BoxPack Class.
	* This class defines box type meshpack.
	*/
	class BoxPack : public MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rows The rows number.
		* @param[in] colums The colums number.
		*/
		BoxPack(uint32_t rows = 2, uint32_t colums = 2)
			: MeshPack(), m_Rows(rows), m_Colums(colums) {};

		/**
		* @brief This interface is used for build specific meshpack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		*/
		virtual void OnCreatePack(bool isCreateBuffer = true) override;

	private:

		/**
		* @brief How much rows number we use.
		*/
		uint32_t m_Rows;

		/**
		* @brief How much cols number we use.
		*/
		uint32_t m_Colums;
	};

	/**
	* @brief SpherePack Class.
	* This class defines sphere type meshpack.
	*/
	class SpherePack : public MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rows The rows number.
		* @param[in] colums The colums number.
		*/
		SpherePack(uint32_t rows = 15, uint32_t colums = 24)
			: MeshPack(), m_Rows(rows), m_Colums(colums) {};

		/**
		* @brief This interface is used for build specific meshpack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		*/
		virtual void OnCreatePack(bool isCreateBuffer = true) override;

	private:

		/**
		* @brief How much rows number we use.
		*/
		uint32_t m_Rows;

		/**
		* @brief How much cols number we use.
		*/
		uint32_t m_Colums;
	};

	/**
	* @brief FilePack Class.
	* This class defines file type meshpack.
	*/
	class FilePack : public MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] filePath The mesh file path in disk.
		*/
		FilePack(const std::string& filePath) : MeshPack(), m_Path(filePath) {};

		/**
		* @brief This interface is used for build specific meshpack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		*/
		virtual void OnCreatePack(bool isCreateBuffer = true) override;

	private:

		/**
		* @brief The mesh file path in disk.
		*/
		std::string m_Path;
	};
}