#pragma once

#include <windows.h>

#define DXW_INTERFACE_API extern "C" __declspec(dllexport)

namespace dxw
{
// 3d-specific
DXW_INTERFACE_API void DXW_D3D_Clear(float, float, float, float);

// 2d-specific
DXW_INTERFACE_API void DXW_D2D_BeginDraw();
DXW_INTERFACE_API void DXW_D2D_EndDraw();
DXW_INTERFACE_API void DXW_D2D_Clear(float, float, float, float);
DXW_INTERFACE_API void DXW_D2D_DrawLine(float, float, float, float);
DXW_INTERFACE_API void DXW_D2D_SetScale(float, float);
DXW_INTERFACE_API void DXW_D2D_SetRotation(float);
DXW_INTERFACE_API void DXW_D2D_SetTranslation(float, float);
DXW_INTERFACE_API void DXW_D2D_RecalculateTransformMatrix();
DXW_INTERFACE_API void DXW_D2D_ResetTransformMatrix();
DXW_INTERFACE_API void DXW_D2D_DrawText(const WCHAR * string, float left, float top, float right, float bottom);
DXW_INTERFACE_API void DXW_D2D_FillRoundedRectangle(float left, float top, float right, float bottom, float radiusX, float radiusY);

// dx-general
DXW_INTERFACE_API void DXW_Present(int);
DXW_INTERFACE_API bool DXW_IsInitialized();
DXW_INTERFACE_API void DXW_ReleaseDxwResources();
DXW_INTERFACE_API int  DXW_InitWindow(HWND hWnd);
DXW_INTERFACE_API void DXW_ResizeWindow(unsigned int, unsigned int);
DXW_INTERFACE_API void DXW_SetTargetWindow(int);

// demos
DXW_INTERFACE_API void DXW_DemoNRT(float);
DXW_INTERFACE_API void DXW_DemoLines(int);
DXW_INTERFACE_API void DXW_Demo3D();

}

#undef DXW_INTERFACE_API
