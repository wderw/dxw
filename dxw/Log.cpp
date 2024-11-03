#include <spdlog/sinks/stdout_color_sinks.h>

#include "Log.h"

namespace dxw
{

std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::Init()
{
	spdlog::set_pattern("%^[%n] <%H:%M:%S.%e> [%l] %v%$");
	s_Logger = spdlog::stdout_color_mt("DXW_LOG");
	s_Logger->set_level(spdlog::level::trace);
}

}
