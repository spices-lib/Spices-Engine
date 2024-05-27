#include "Pchheader.h"
#include "Console.h"

namespace Spiecs {

	static std::unordered_map<std::string, std::shared_ptr<Console>> m_GlobalConsolePool;

	Console::Console(const std::string& filePath)
		: m_FilePath(filePath)
	{
		m_LogInfos.clear();
	}

	Console::~Console()
	{

	}

	std::shared_ptr<Console> Console::Register(const std::string& name, const std::string& filePath)
	{
		if (m_GlobalConsolePool.find(name) == m_GlobalConsolePool.end())
		{
			m_GlobalConsolePool[name] = std::make_shared<Console>(filePath);
		}

		return m_GlobalConsolePool[name];
	}

	void Console::Clear()
	{
		m_LogInfos.clear();
	}

	void Console::Push(const std::string& cmd)
	{
		m_LogInfos.push_back({ cmd , "info", glm::vec4(1.0f)});
	}

	void Console::sink_it_(const spdlog::details::log_msg& msg)
	{
		//std::lock_guard<std::mutex> lock(mutex_);

		std::stringstream ss;

		/**
		* @brief Get Log time.
		*/
		time_t t = std::chrono::system_clock::to_time_t(msg.time);
		std::string times = ctime(&t);
		times.pop_back();

		/**
		* @brief Get Log level.
		*/
		InfoLevelHelper helper;

		switch (msg.level)
		{
		case spdlog::level::level_enum::trace: 
			helper.level = "trace";
			helper.color = glm::vec4(1.0f);
			break;
		case spdlog::level::level_enum::info:
			helper.level = "info";
			helper.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case spdlog::level::level_enum::warn:
			helper.level = "warn";
			helper.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			break;
		case spdlog::level::level_enum::err:
			helper.level = "error";
			helper.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case spdlog::level::level_enum::critical:
			helper.level = "critical";
			helper.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}

		/**
		* @brief Build Log string.
		*/
		ss << "[" << times << "] [" << msg.logger_name.data() << "] [" << helper.level << "] " << msg.payload.data();
		helper.str = ss.str();

		/**
		* @brief Keep it.
		*/
		m_LogInfos.push_back(helper);
	}
}