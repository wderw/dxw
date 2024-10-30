#include <windows.h>
#include "Log.h"

void Init()
{
    dxw::Log::Init();
    LPSTR commandLine = GetCommandLineA();
    LOG_INFO("DLL_PROCESS_ATTACH called for process id: {}", GetCurrentProcessId());
    LOG_INFO("Calling process command: {}", commandLine);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Init();
        break;
    case DLL_THREAD_ATTACH:
        LOG_TRACE("DLL_THREAD_ATTACH called for thread id: {}", GetCurrentThreadId());
        break;
    case DLL_THREAD_DETACH:
        LOG_TRACE("DLL_THREAD_DETACH called for thread id: {}", GetCurrentThreadId());
        break;
    case DLL_PROCESS_DETACH:
        LOG_TRACE("DLL_PROCESS_DETACH called for process id: {}", GetCurrentProcessId());
        break;
    }
    return TRUE;
}

