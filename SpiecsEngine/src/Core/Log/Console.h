#pragma once
#include "Core/Core.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <deque>

namespace Spiecs {

	struct InfoLevelHelper
	{
		std::string str;
		std::string level;
		glm::vec4 color;
	};

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
			m_TraceLogInfos.clear();
			m_InfoLogInfos.clear();
			m_WarnLogInfos.clear();
			m_ErrorLogInfos.clear();
			m_CriticalLogInfos.clear();
		}
	};

	class Console : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		Console(const std::string& filePath);
		virtual ~Console();

		static std::shared_ptr<Console> Register(
			const std::string& name, 
			const std::string& filePath = ""
		);

		const InfoData& GetInfos() { return m_InfoData; };
		void Clear();
		void Push(const std::string& cmd);
		inline const std::string& GetFilePath() { return m_FilePath; };

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override;
		virtual void flush_() { Clear(); };

	protected:
		std::string m_FilePath;

		InfoData m_InfoData;
	};
}