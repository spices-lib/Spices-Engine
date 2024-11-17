#pragma once
#include "Core/Core.h"
#include "Resources/Mesh/MeshPack.h"
#include "../Object/GltfMeshes.h"

namespace Spices {

	class GltfPack : public MeshPack
	{
	public:

		GltfPack(const std::string& name, const std::function<void(GltfPack*)>& f, bool instanced = true)
			: MeshPack(name, instanced)
			, m_Func(f)
		{
			m_PackType = "GltfPack";
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfPack() = default;

		virtual bool OnCreatePack(bool isCreateBuffer = true) override;

	private:

		std::function<void(GltfPack*)> m_Func;
		friend class GltfLoader;
	};
}