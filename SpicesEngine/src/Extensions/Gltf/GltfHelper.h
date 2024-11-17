/**
* @file GltfHelper.h
* @brief The GltfHelper Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include <nlohmann/json.hpp>

namespace Spices {

    /**
    * @brief Static functions to help resolve Gltf json.
    */
	class GltfHelper
	{
	public:

		using Json = nlohmann::json;

        static VkFormat GetFormat(const std::string& str, int id);
        static uint32_t SizeOfFormat(VkFormat format);

        /**
        * @brief Get Variable Type bytes.
        * @param[in] id Type identify.
        * @return Returns bytes.
        */
		static int GetFormatSize(int id);

        /**
        * @brief Get Variable Type dimensions.
        * @param[in] str Variable Type string.
        * @return Returns dimensions.
        */
		static int GetDimensions(const std::string& str);

        /**
        * @brief Split digital number in attribute.
        * @param[in] attribute .
        * @param[out] semanticName out attribute.
        * @param[out] semanticIndex split location.
        */
		static void SplitGltfAttribute(std::string attribute, std::string* semanticName, uint32_t* semanticIndex);

        /**
        * @brief Turn a Json::array_t to glm::vec4.
        * @param[in] accessor Json::array_t.
        * @return Returns glm::vec4.
        */
		static glm::vec4 GetVector(const Json::array_t& accessor);

        /**
        * @brief Turn a Json::array_t to glm::mat4.
        * @param[in] accessor Json::array_t.
        * @return Returns glm::mat4.
        */
		static glm::mat4 GetMatrix(const Json::array_t& accessor);

        /**
        * @brief Get string value in Json::object_t.
        * @param[in] pRoot json object.
        * @param[in] path type name.
        * @param[in] pDefault default return value.
        * return Returns value if finded or default if not.
        */
		static std::string GetElementString(const Json::object_t& root, const char* path, std::string pDefault);

        /**
        * @brief Get float value in Json::object_t.
        * @param[in] pRoot json object.
        * @param[in] path type name.
        * @param[in] pDefault default return value.
        * return Returns value if finded or default if not.
        */
		static float GetElementFloat(const Json::object_t& root, const char* path, float pDefault);

        /**
        * @brief Get int value in Json::object_t.
        * @param[in] pRoot json object.
        * @param[in] path type name.
        * @param[in] pDefault default return value.
        * return Returns value if finded or default if not.
        */
		static int GetElementInt(const Json::object_t& root, const char* path, int pDefault);

        /**
        * @brief Get bool value in Json::object_t.
        * @param[in] pRoot json object.
        * @param[in] path type name.
        * @param[in] pDefault default return value.
        * return Returns value if finded or default if not.
        */
		static bool GetElementBoolean(const Json::object_t& root, const char* path, bool pDefault);

        /**
        * @brief Get array value in Json::object_t.
        * @param[in] pRoot json object.
        * @param[in] path type name.
        * @param[in] pDefault default return value.
        * return Returns value if finded or default if not.
        */
		static Json::array_t GetElementJsonArray(const Json::object_t& root, const char* path, Json::array_t pDefault);

        /**
        * @brief Get vec4 value in Json::object_t.
        * @param[in] pRoot json object.
        * @param[in] path type name.
        * @param[in] pDefault default return value.
        * return Returns value if finded or default if not.
        */
		static glm::vec4 GetElementVector(Json::object_t& root, const char* path, glm::vec4 default);

	private:

        /**
        * @brief Get specific type in Json::object_t.
        * @tparam type specific type.
        * @param[in] pRoot json object.
        * @param[in] path type name.
        * @param[in] pDefault default return value.
        * return Returns value if finded or default if not.
        */
		template <class type>
        static type GetElement(const Json::object_t* pRoot, const char* path, type pDefault);
	};

	template<class type>
	inline type GltfHelper::GetElement(const Json::object_t* pRoot, const char* path, type pDefault)
	{
        SPICES_PROFILE_ZONE;

        const char* p = path;
        char token[128];
        while (true)
        {
            for (; *p != '/' && *p != 0 && *p != '['; p++);
            memcpy(token, path, p - path);
            token[p - path] = 0;

            auto it = pRoot->find(token);
            if (it == pRoot->end())
            {
                return pDefault;
            }

            if (*p == '[')
            {
                p++;
                int i = atoi(p);
                for (; *p != 0 && *p != ']'; p++);
                pRoot = it->second.at(i).get_ptr<const Json::object_t*>();
                p++;
            }
            else
            {
                if (it->second.is_object())
                {
                    pRoot = it->second.get_ptr<const Json::object_t*>();
                }
                else
                {
                    return it->second.get<type>();
                }
            }
            p++;
            path = p;
        }

        return pDefault;
	}
}