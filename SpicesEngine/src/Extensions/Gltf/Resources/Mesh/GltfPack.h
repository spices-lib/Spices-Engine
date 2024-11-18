/**
* @file GltfPack.h
* @brief The GltfPack Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Mesh/MeshPack.h"
#include "../Object/GltfMeshes.h"

namespace Spices {

	/**
	* @brief GltfPack Class.
	* This class defines gltf type meshPack.
	*/
	class GltfPack : public MeshPack
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] name pack name.
		* @param[in] f function pointer of createPack.
		* @param[in] instanced true if instance from resourcePool.
		*/
		GltfPack(const std::string& name, const std::function<void(GltfPack*)>& f, bool instanced = true)
			: MeshPack(name, instanced)
			, m_Func(f)
		{
			m_PackType = "GltfPack";
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfPack() override = default;

		/**
		* @brief This interface is used for build specific meshPack data.
		* @param[in] isCreateBuffer Whether it needs to create buffer.
		* @return Returns true if Create Pack successfully.
		*/
		virtual bool OnCreatePack(bool isCreateBuffer = true) override;

	private:

		/**
		* @brief function pointer of createPack.
		*/
		std::function<void(GltfPack*)> m_Func;

		/**
		* @brief Allow GltfLoader access all data this class.
		*/
		friend class GltfLoader;
	};
}