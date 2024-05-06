/**
* @file FrameInfo.h.
* @brief The FrameInfo Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Mesh/Mesh.h"

// STL Header.
#include <memory>

namespace Spiecs {

	/**
	* @brief Forward Declear.
	*/
	class World;

	/**
	* @brief FrameInfo Class.
	* This class defines the FrameInfo data.
	*/
	class FrameInfo
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		FrameInfo() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~FrameInfo() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		FrameInfo(const FrameInfo&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		FrameInfo& operator=(const FrameInfo&) = delete;

		/**
		* @brief Get FrameInfo.
		* @return Returns static FrameInfo reference.
		*/
		static FrameInfo& Get();

	public:

		/**
		* @brief FrameIndex, varing during 0 - (MaxFrameInFlight - 1).
		* Used almost anywhere.
		*/
		uint32_t m_FrameIndex = 0;

		/**
		* @brief ImageIndex, varing during 0 - (MaxFrameInFlight - 1).
		* Used in swapchain index and framebuffer index.
		*/
		uint32_t m_Imageindex = 0;

		std::vector<int> m_SelectFrontEntityID{};

		std::vector<int> m_SelectBackEntityID{};

		/**
		* @brief The shared pointer of specific world.
		*/
		std::shared_ptr<World> m_World;
	};
}