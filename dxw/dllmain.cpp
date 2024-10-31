#include <windows.h>
#include "Log.h"
#include "DxwSharedContext.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Dwrite.lib")

void Init()
{
    dxw::Log::Init();
    LOG_INFO("DLL_PROCESS_ATTACH called for process id: {}", GetCurrentProcessId());
    LOG_INFO("Calling process command: {}", GetCommandLineA());
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

