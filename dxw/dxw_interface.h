#pragma once

#include <windows.h>

namespace dxw
{

extern "C" __declspec(dllexport) int  DXW_InitWindow(HWND hWnd);
extern "C" __declspec(dllexport) void DXW_D3D_Clear();
extern "C" __declspec(dllexport) void DXW_D2D_BeginDraw();
extern "C" __declspec(dllexport) void DXW_D2D_EndDraw();
extern "C" __declspec(dllexport) void DXW_D2D_Clear();
extern "C" __declspec(dllexport) void DXW_Present(int);

}
