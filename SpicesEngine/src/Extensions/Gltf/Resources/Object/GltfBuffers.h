/**
* @file GltfBuffers.h.
* @brief The GltfBuffers Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	/**
	* @brief Wrapper of Gltf Json Buffers.
	*/
	class GltfBuffers : public GltfObject
	{
	public:

		/**
		* @brief Buffers Item data.
		*/
		struct Item
		{
			int byteLength;                                /* @brief Buffer bytes.    */
			std::string uri;                               /* @brief Buffer file url. */
			std::unique_ptr<std::vector<char>> buffer;     /* @brief Buffer data.     */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] data Specific Json element.
		* @param[in] path gltf file directory path.
		*/
		GltfBuffers(const Json& data, const std::filesystem::path& path)
			: GltfObject(data)
			, m_Path(path)
		{
			SPICES_PROFILE_ZONE;

			m_BuffersData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_BuffersData[i];
				const Json& json = data[i];

				item.byteLength    = GltfHelper::GetElementInt(json, "byteLength", -1);
				item.uri           = GltfHelper::GetElementString(json, "uri", "");

				/**
				* @brief Read buffer file.
				*/
				std::stringstream file;
				file << path.string() << "/" << item.uri;
				
				std::ifstream f(file.str(), std::ios::in | std::ios::binary);
				if (!f)
				{
					std::stringstream ss;
					ss << "Buffer Files: " << file.str() << "  is not finded.";

					SPICES_CORE_ERROR(ss.str())
					continue;
				}

				f.seekg(0, f.end);
				std::streamoff length = f.tellg();
				f.seekg(0, f.beg);

				item.buffer = std::make_unique<std::vector<char>>(length, 0);
				f.read(item.buffer->data(), length);
			}
		}

		/**
		* @brief Destructor Function.
		*/
		virtual ~GltfBuffers() override = default;

	private:

		/**
		* @brief gltf file directory path.
		*/
		std::filesystem::path m_Path;

		/**
		* @brief Data of Gltf Json Buffers
		*/
		std::vector<Item> m_BuffersData;

		/**
		* @brief Allow GltfLoader and GltfCollection access all data this class.
		*/
		friend class GltfLoader;
		friend class GltfCollection;
	};
}
