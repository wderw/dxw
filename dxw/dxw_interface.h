#pragma once

#include <windows.h>

namespace dxw
{
// 3d-specific
extern "C" __declspec(dllexport) void DXW_D3D_Clear(float, float, float, float);

// 2d-specific
extern "C" __declspec(dllexport) void DXW_D2D_BeginDraw();
extern "C" __declspec(dllexport) void DXW_D2D_EndDraw();
extern "C" __declspec(dllexport) void DXW_D2D_Clear(float, float, float, float);
extern "C" __declspec(dllexport) void DXW_D2D_DrawLine(float, float, float, float);
extern "C" __declspec(dllexport) void DXW_D2D_SetScale(float, float);
extern "C" __declspec(dllexport) void DXW_D2D_SetRotation(float);
extern "C" __declspec(dllexport) void DXW_D2D_SetTranslation(float, float);
extern "C" __declspec(dllexport) void DXW_D2D_RecalculateTransformMatrix();
extern "C" __declspec(dllexport) void DXW_D2D_ResetTransformMatrix();
extern "C" __declspec(dllexport) void DXW_D2D_DrawText(const WCHAR * string, float left, float top, float right, float bottom);

// dx-general
extern "C" __declspec(dllexport) void DXW_Present(int);
extern "C" __declspec(dllexport) bool DXW_IsInitialized();
extern "C" __declspec(dllexport) void DXW_ReleaseDxwResources();
extern "C" __declspec(dllexport) int  DXW_InitWindow(HWND hWnd);
extern "C" __declspec(dllexport) void DXW_ResizeWindow(unsigned int, unsigned int);
extern "C" __declspec(dllexport) void DXW_SetTargetWindow(int);

// demos
extern "C" __declspec(dllexport) void DXW_DemoNRT(float);
extern "C" __declspec(dllexport) void DXW_DemoLines(int);
extern "C" __declspec(dllexport) void DXW_Demo3D();

}
