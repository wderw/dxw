#pragma once

#include <memory>
#include <spdlog/spdlog.h>

#include "Utils.h"

namespace dxw
{

class Log
{
	Log() {}
	~Log() {}

public:
	static void Init();
	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};

}

#define LOG_TRACE(format, ...) \
    ::dxw::Log::GetLogger()->trace("[{}:{}]: " format, dxw::Utils::ExtractBaseName(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) \
    ::dxw::Log::GetLogger()->debug("[{}:{}]: " format, dxw::Utils::ExtractBaseName(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_INFO(format, ...) \
    ::dxw::Log::GetLogger()->info("[{}:{}]: " format, dxw::Utils::ExtractBaseName(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_WARN(format, ...) \
    ::dxw::Log::GetLogger()->warn("[{}:{}]: " format, dxw::Utils::ExtractBaseName(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) \
    ::dxw::Log::GetLogger()->error("[{}:{}]: " format, dxw::Utils::ExtractBaseName(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_CRITICAL(format, ...) \
    ::dxw::Log::GetLogger()->critical("[{}:{}]: " format, dxw::Utils::ExtractBaseName(__FILE__), __LINE__, ##__VA_ARGS__)

