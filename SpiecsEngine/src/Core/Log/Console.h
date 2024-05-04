#pragma once
#include "Core/Core.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace Spiecs {

	struct InfoLevelHelper
	{
		std::string str;
		std::string level;
		glm::vec4 color;
	};

	class Console : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		Console();
		virtual ~Console();

		static std::shared_ptr<Console> Register(const std::string& name);

		const std::vector<InfoLevelHelper>& GetInfos() { return m_LogInfos; };
		void Clear();
		void Push(const std::string& cmd);

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override;
		virtual void flush_() { Clear(); };

	protected:
		std::vector<InfoLevelHelper> m_LogInfos;
	};
}