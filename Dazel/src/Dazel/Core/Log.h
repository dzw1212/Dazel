#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h" //自定义spdlog的输出流操作符


namespace DAZEL
{
	class DAZEL_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define CORE_LOG_TRACE(...)			DAZEL::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_LOG_DEBUG(...)			DAZEL::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define CORE_LOG_INFO(...)			DAZEL::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...)			DAZEL::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...)			DAZEL::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_LOG_CRITICAL(...)		DAZEL::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...)				DAZEL::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)				DAZEL::Log::GetClientLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)				DAZEL::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)				DAZEL::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)				DAZEL::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)			DAZEL::Log::GetClientLogger()->critical(__VA_ARGS__)