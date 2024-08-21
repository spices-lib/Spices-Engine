/**
* @file Console.h
* @brief The Console Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <deque>

namespace Spices {

	/**
	* @brief Help data of specific level of information.
	*/
	struct InfoLevelHelper
	{
		/**
		* @brief Info information.
		*/
		std::string str;

		/**
		* @brief specific level of this information.
		*/
		std::string level;

		/**
		* @brief specific color of this information.
		*/
		glm::vec4 color;
	};

	/**
	* @brief Console Information Data.
	*/
	struct InfoData
	{
		/**
		* @brief trace deque.
		*/
		std::deque<InfoLevelHelper> m_TraceLogInfos;

		/**
		* @brief info deque.
		*/
		std::deque<InfoLevelHelper> m_InfoLogInfos;

		/**
		* @brief warn deque.
		*/
		std::deque<InfoLevelHelper> m_WarnLogInfos;

		/**
		* @brief error deque.
		*/
		std::deque<InfoLevelHelper> m_ErrorLogInfos;

		/**
		* @brief critical deque.
		*/
		std::deque<InfoLevelHelper> m_CriticalLogInfos;

		void Clear() 
		{
			SPICES_PROFILE_ZONE;

			m_TraceLogInfos.clear();
			m_InfoLogInfos.clear();
			m_WarnLogInfos.clear();
			m_ErrorLogInfos.clear();
			m_CriticalLogInfos.clear();
		}
	};

	/**
	* @brief Console Entity Class.
	*/
	class Console : public spdlog::sinks::base_sink<std::mutex>
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] filePath Console output file.
		* @param[in] maxInfos Maxiumn Num of Informations.
		*/
		Console(const std::string& filePath, uint32_t maxInfos = 50);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Console() override = default;

		/**
		* @brief Registry a console to ConsolePool.
		* @param[in] name ConsoleName.
		* @param[in] filePath Console output file.
		* @return Returns Registried Console form Pool.
		*/
		static std::shared_ptr<Console> Registry(
			const std::string& name, 
			const std::string& filePath = ""
		);

		/**
		* @brief Get Console Infos.
		* @return Returns Console Infos.
		*/
		const InfoData& GetInfos() const { return m_InfoData; }

		/**
		* @brief Clear Console Infos.
		*/
		void Clear();

		/**
		* @brief Push a Command to Console.
		* @param[in] cmd Command.
		* @todo Implementate it.
		*/
		void Push(const std::string& cmd);

		/**
		* @brief Get Console output file.
		* @return Returns Console output file.
		*/
		const std::string& GetFilePath() { return m_FilePath; }

	protected:

		/**
		* @brief Inherited from spdlog, run when a message pushed.
		* @param[in] msg spdlog message.
		*/
		void sink_it_(const spdlog::details::log_msg& msg) override;

		/**
		* @brief Inherited from spdlog, run when spdlog flush.
		*/
		virtual void flush_() override { Clear(); }

	protected:

		/**
		* @brief Console File Path.
		*/
		std::string m_FilePath;

		/**
		* @brief Maxiumn Num of Informations.
		*/
		uint32_t m_MaxInfos;

		/**
		* @brief Console Information.
		*/
		InfoData m_InfoData;
	};
}