#pragma once

#include <windows.h>

namespace dxw
{

extern "C" __declspec(dllexport) HRESULT DXW_InitWindow(HWND hWnd);
extern "C" __declspec(dllexport) void DXW_D3DDeviceContext_Clear();

}
