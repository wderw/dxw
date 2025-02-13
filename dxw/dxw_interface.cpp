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
	LOG_INFO("DXW_InitWindow called for window handle (HWND): {}", hwndValue);
	auto window = std::make_shared<DxwWindow>();
	window->InitDirectX(hWnd);
	DxwSharedContext::GetInstance().RegisterWindow(window);

	const int id = window->GetId();
	LOG_INFO("DxwWindow with id: {} has been attached to HWND control: {}", id, hwndValue);
	return id;
}

void DXW_ResizeWindow(unsigned int width, unsigned int height)
{
	LOG_TRACE("DXW_ResizeWindow called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->ResizeWindow(width, height);
}

void DXW_SetTargetWindow(int newId)
{
	LOG_TRACE("DXW_SetTargetWindow called");
	DxwSharedContext::GetInstance().SetTargetId(newId);
}

void DXW_DemoRT()
{
	LOG_TRACE("DXW_DemoRT called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->DemoRT();
}

void DXW_DemoNRT(float fi)
{
	LOG_TRACE("DXW_NRTDemo called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->DemoNRT(fi);
}

void DXW_DemoLines(int n)
{
	LOG_TRACE("DXW_DemoLines called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->DemoLines(n);
}

void DXW_Demo3D()
{
	LOG_TRACE("DXW_Demo3D called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->Demo3D();
}

void DXW_Present(int vsync = 1)
{
	LOG_TRACE("DXW_Present called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->DX_Present(vsync);
}

bool DXW_IsInitialized()
{
	LOG_TRACE("DXW_IsInitialized called");
	if (DxwSharedContext::GetInstance().GetCurrentWindow() != nullptr)
	{
		return DxwSharedContext::GetInstance().GetCurrentWindow()->IsInitialized();
	}

	LOG_WARN("Current window id not registered in SharedContext!");
	return false;
}

void DXW_ReleaseDxwResources()
{
	LOG_TRACE("DXW_ReleaseDxwResources called");
	DxwSharedContext::GetInstance().ReleaseDxwResources();
}

void DXW_D3D_Clear(float r, float g, float b, float a)
{
	LOG_TRACE("DXW_D3D_Clear called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D3D_Clear(r, g, b, a);
}

void DXW_D2D_Clear()
{
	LOG_TRACE("DXW_D2D_Clear called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_Clear();
}

void DXW_D2D_DrawLine(float x0, float y0, float x1, float y1, const char* brushName)
{
	LOG_TRACE("DXW_D2D_DrawLine called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_DrawLine(x0, y0, x1, y1, brushName);
}

void DXW_D2D_BeginDraw()
{
	LOG_TRACE("DXW_D2D_BeginDraw called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_BeginDraw();
}

void DXW_D2D_EndDraw()
{
	LOG_TRACE("DXW_D2D_EndDraw called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_EndDraw();
}

}