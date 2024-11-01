#include <memory>

#include "dxw_interface.h"
#include "Log.h"
#include "DxwWindow.h"
#include "DxwSharedContext.h"

namespace dxw
{

int DXW_InitWindow(HWND hWnd)
{
	uintptr_t hwndValue = reinterpret_cast<uintptr_t>(hWnd);
	LOG_INFO("Application called DXW_InitWindow for window handle (HWND): {}", hwndValue);
	auto window = std::make_shared<DxwWindow>();
	window->InitDirectX(hWnd);
	DxwSharedContext::GetInstance().RegisterWindow(window);

	const int id = window->GetId();
	LOG_INFO("DxwWindow with id: {} has been attached to HWND control: {}", id, hwndValue);
	return id;
}

void DXW_SetTargetWindow(int newId)
{
	LOG_DEBUG("Application called DXW_SetTargetWindow");
	DxwSharedContext::GetInstance().SetTargetId(newId);
}

void DXW_Present(int vsync = 1)
{
	LOG_DEBUG("Application called DXW_Present");
	DxwSharedContext::GetInstance().GetCurrentWindow()->DX_Present(vsync);
}

void DXW_D3D_Clear()
{
	LOG_DEBUG("Application called DXW_D3D_Clear");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D3D_Clear();
}

void DXW_D2D_Clear()
{
	LOG_DEBUG("Application called DXW_D2D_Clear");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_Clear();
}

void DXW_D2D_BeginDraw()
{
	LOG_DEBUG("Application called DXW_D2D_BeginDraw");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_BeginDraw();
}

void DXW_D2D_EndDraw()
{
	LOG_DEBUG("Application called DXW_D2D_EndDraw");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_EndDraw();
}

}