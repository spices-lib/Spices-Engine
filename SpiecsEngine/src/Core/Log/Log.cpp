#include "Pchheader.h"
#include "Log.h"
#include "Console.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Spiecs {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%l] %v");
		spdlog::flush_every(std::chrono::seconds(5));

		auto max_size = 1048576 * 5;
		auto max_files = 3;

		/**
		* @brief Log/Log.log file.
		*/
		auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(SPIECS_lOG_PATH, max_size, max_files);
		file_sink->set_level(spdlog::level::trace);

		/**
		* @brief console log.
		*/
		auto ide_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		ide_console_sink->set_level(spdlog::level::trace);

		auto console_sink = Console::Register("Console");

		/**
		* @brief console slate log.
		*/
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(ide_console_sink);
		sinks.push_back(file_sink);
		sinks.push_back(console_sink);

		s_CoreLogger = std::make_shared<spdlog::logger>("Engine", begin(sinks), end(sinks));
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("Game", begin(sinks), end(sinks));
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	void Log::ShutDown()
	{
		s_CoreLogger.reset();
		s_ClientLogger.reset();
		spdlog::drop_all();
	}
}