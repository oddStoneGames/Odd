#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Odd
{
	class ODD_API Log
	{
		public:
			static void Init();

			inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
			inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		private:
			static std::shared_ptr<spdlog::logger> s_CoreLogger;
			static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define DEBUG_CORE_TRACE(...)	::Odd::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DEBUG_CORE_INFO(...)	::Odd::Log::GetCoreLogger()-> info(__VA_ARGS__)
#define DEBUG_CORE_WARN(...)	::Odd::Log::GetCoreLogger()-> warn(__VA_ARGS__)
#define DEBUG_CORE_ERROR(...)	::Odd::Log::GetCoreLogger()->error(__VA_ARGS__)

#define DEBUG_TRACE(...)	::Odd::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DEBUG_INFO(...)	::Odd::Log::GetClientLogger()-> info(__VA_ARGS__)
#define DEBUG_WARN(...)	::Odd::Log::GetClientLogger()-> warn(__VA_ARGS__)
#define DEBUG_ERROR(...)	::Odd::Log::GetClientLogger()->error(__VA_ARGS__)