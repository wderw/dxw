#pragma once

#include <memory>
#include <string_view>

#include <spdlog/spdlog.h>

inline const char* extract_base_name(const char* full_path)
{
    const char* base_name = strrchr(full_path, '\\');
    return base_name ? base_name + 1 : full_path;
}

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
    ::dxw::Log::GetLogger()->trace("[{}:{}]: " format, extract_base_name(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_INFO(format, ...) \
    ::dxw::Log::GetLogger()->info("[{}:{}]: " format, extract_base_name(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_WARN(format, ...) \
    ::dxw::Log::GetLogger()->warn("[{}:{}]: " format, extract_base_name(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) \
    ::dxw::Log::GetLogger()->error("[{}:{}]: " format, extract_base_name(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_CRITICAL(format, ...) \
    ::dxw::Log::GetLogger()->critical("[{}:{}]: " format, extract_base_name(__FILE__), __LINE__, ##__VA_ARGS__)

