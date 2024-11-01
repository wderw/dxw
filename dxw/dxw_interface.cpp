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
	LOG_INFO("DXW_InitWindow called for window or control handle (HWND): {}", hwndValue);
	auto window = std::make_shared<DxwWindow>();
	window->InitDirectX(hWnd);
	DxwSharedContext::GetInstance().RegisterWindow(window);

	const int id = window->GetId();
	LOG_INFO("DxwWindow with id: {} has been attached to HWND control: {}", id, hwndValue);
	return id;
}

void DXW_Present(int vsync = 1)
{
	LOG_DEBUG("DXW_Present called");
	DxwSharedContext::GetInstance().GetWindowByIndex(0)->DX_Present(vsync);
}

void DXW_D3D_Clear()
{
	LOG_DEBUG("DXW_D3D_Clear called");
	DxwSharedContext::GetInstance().GetWindowByIndex(0)->D3D_Clear();
}

void DXW_D2D_Clear()
{
	LOG_DEBUG("DXW_D2D_Clear called");
	DxwSharedContext::GetInstance().GetWindowByIndex(0)->D2D_Clear();
}

void DXW_D2D_BeginDraw()
{
	LOG_DEBUG("DXW_D2D_BeginDraw called");
	DxwSharedContext::GetInstance().GetWindowByIndex(0)->D2D_BeginDraw();
}

void DXW_D2D_EndDraw()
{
	LOG_DEBUG("DXW_D2D_EndDraw called");
	DxwSharedContext::GetInstance().GetWindowByIndex(0)->D2D_EndDraw();
}

}