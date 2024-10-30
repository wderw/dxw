#pragma once

#include <memory>

#include <spdlog/spdlog.h>

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

#define LOG_TRACE(...)    ::dxw::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)     ::dxw::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     ::dxw::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    ::dxw::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::dxw::Log::GetLogger()->critical(__VA_ARGS__)

