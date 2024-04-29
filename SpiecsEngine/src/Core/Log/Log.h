#pragma once

#include "Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE //必须定义这个宏,才能输出文件名和行号
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Spiecs {

	class Log
	{
	public:
		static void Init();
		static void ShutDown();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	public:
		template <typename... Args>
		using format_string_t = fmt::format_string<Args...>;

		template <typename... Args>
		static void PostHandle(format_string_t<Args...> fmt, Args &&...args);

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

	template<typename ...Args>
	inline void Log::PostHandle(format_string_t<Args...> fmt, Args && ...args)
	{
	}
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

#ifdef SPIECS_DEBUG

// Core log macros
#define SPIECS_CORE_TRACE(...)    ::Spiecs::Log::GetCoreLogger()->trace      (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__)
#define SPIECS_CORE_INFO(...)     ::Spiecs::Log::GetCoreLogger()->info       (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__)
#define SPIECS_CORE_WARN(...)     ::Spiecs::Log::GetCoreLogger()->warn       (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__)
#define SPIECS_CORE_ERROR(...)    ::Spiecs::Log::GetCoreLogger()->error      (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__);  throw std::runtime_error(__VA_ARGS__)
#define SPIECS_CORE_CRITICAL(...) ::Spiecs::Log::GetCoreLogger()->critical   (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__);  throw std::runtime_error(__VA_ARGS__)

// Client log macros
#define SPIECS_TRACE(...)         ::Spiecs::Log::GetClientLogger()->trace    (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__)
#define SPIECS_INFO(...)          ::Spiecs::Log::GetClientLogger()->info     (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__)
#define SPIECS_WARN(...)          ::Spiecs::Log::GetClientLogger()->warn     (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__)
#define SPIECS_ERROR(...)         ::Spiecs::Log::GetClientLogger()->error    (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__);  throw std::runtime_error(__VA_ARGS__)
#define SPIECS_CRITICAL(...)      ::Spiecs::Log::GetClientLogger()->critical (__VA_ARGS__)   ;   ::Spiecs::Log::PostHandle(__VA_ARGS__);  throw std::runtime_error(__VA_ARGS__)

#endif // SPIECS_DEBUG

#ifdef SPIECS_RELEASE

// Core log macros
#define SPIECS_CORE_TRACE(...)    
#define SPIECS_CORE_INFO(...)     
#define SPIECS_CORE_WARN(...)     
#define SPIECS_CORE_ERROR(...)    
#define SPIECS_CORE_CRITICAL(...) 

// Client log macros
#define SPIECS_TRACE(...)         
#define SPIECS_INFO(...)          
#define SPIECS_WARN(...)          
#define SPIECS_ERROR(...)         
#define SPIECS_CRITICAL(...)      

#endif // SPIECS_RELEASE


