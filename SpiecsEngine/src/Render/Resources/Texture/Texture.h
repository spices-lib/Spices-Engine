#pragma once
#include "Core/Core.h"
#include "Render/Resources/Loader/TextureLoader.h"

#include <memory>
#include <any>

namespace Spiecs {

	class Texture
	{
	public:
		Texture() {};
		Texture(const std::string& path) : m_ResourcePath(path) {};
		virtual ~Texture() {};

		template<typename T>
		std::shared_ptr<T> GetResource();

	protected:
		std::any m_Resource;
		std::string m_ResourcePath;
	};

	template<typename T>
	inline std::shared_ptr<T> Texture::GetResource()
	{
		return std::any_cast<std::shared_ptr<T>>(m_Resource);
	}
}