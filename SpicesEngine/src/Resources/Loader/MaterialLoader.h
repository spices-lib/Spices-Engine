/**
* @file MaterialLoader.h.
* @brief The MaterialLoader Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief Forward declare.
	*/
	class Material;

	/**
	* @brief This enum defines tree types of material file.
	* @note But not in use now.
	* @todo If enable this enum, cuse a conflict.
	*/
	
	//enum MaterialExtension
	//{
	//	/**
	//	* @brief error type
	//	*/
	//	UNKNOWN = 0,

	//	/**
	//	* @brief material type
	//	*/
	//	MATERIAL = 1,

	//	/**
	//	* @brief binary type
	//	*/
	//	SASSET = 2,
	//};
		
	/**
	* @brief MaterialLoader Class.
	* This class only defines static function for load data from material file. 
	*/
	class MaterialLoader
	{
	public:

		/**
		* @brief Public called API, it is entrance.
		* @param[in] fileName Material name.
		* @param[in,out] outMaterial Material pointer, only pass this to it.
		* Feel free for using row pointer here, it's very safe, for this API only called during Material.
		* @return Returns true if load data successfully.
		*/
		static bool Load(const std::string& fileName, Material* outMaterial);

	private:

		/**
		* @brief Load data from a .material file.
		* @param[in] fileName Material path in disk.
		* @param[in,out] outMaterial Material pointer, only pass this to it.
		* @return Returns true if load data successfully.
		*/
		static bool LoadFromMaterial(const std::string& fileName, Material* outMaterial);

		/**
		* @brief Load data from a .sasset file.
		* @param[in] fileName Material path in disk.
		* @param[in,out] outMaterial Material pointer, only pass this to it.
		* @return Returns true if load data successfully.
		* @todo Finish it.
		*/
		static bool LoadFromSASSET(const std::string& fileName, Material* outMaterial);

	public:

		/**
		* @brief Test function.
		* @return Returns true if load data successfully.
		* @todo Move to Test project.
		*/
		static bool SaveDefaultMaterial();
	};
}