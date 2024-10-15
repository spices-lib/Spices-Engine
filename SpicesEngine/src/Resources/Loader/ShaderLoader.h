/**
* @file ShaderLoader.h.
* @brief The ShaderLoader Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Shader/ShaderHelper.h"

namespace Spices {

	/**
	* @brief Forward declare.
	*/
	class Shader;

	/**
	* @brief ShaderLoader Class.
	* This class only defines static function for load data from shader file.
	*/
	class ShaderLoader
	{
	public:

		/**
		* @brief Public called API, it is entrance.
		* @param[in] fileName Shader name.
		* @param[in] stage ShaderStage.
		* @param[in,out] outShader Shader pointer, only pass this to it.
		* Feel free for using row pointer here, it's very safe, for this API only called during Shader.
		* @return Returns true if load data successfully.
		*/
		static bool Load(const std::string& fileName, ShaderStage stage, Shader* outShader);
	};
}