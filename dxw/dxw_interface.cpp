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

void DXW_D2D_Clear(float r, float g, float b, float a)
{
	LOG_TRACE("DXW_D2D_Clear called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_Clear(r, g, b, a);
}

void DXW_D2D_DrawLine(float x0, float y0, float x1, float y1)
{
	LOG_TRACE("DXW_D2D_DrawLine called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_DrawLine(x0, y0, x1, y1);
}

void DXW_D2D_SetScale(float scaleX, float scaleY)
{
	LOG_TRACE("DXW_D2D_SetScale called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_SetScale(scaleX, scaleY);
}

void DXW_D2D_SetRotation(float degrees)
{
	LOG_TRACE("DXW_D2D_SetRotation called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_SetRotation(degrees);
}

void DXW_D2D_SetTranslation(float dx, float dy)
{
	LOG_TRACE("DXW_D2D_SetTranslation called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_SetTranslation(dx, dy);
}

void DXW_D2D_RecalculateTransformMatrix()
{
	LOG_TRACE("DXW_D2D_RecalculateTransformMatrix called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_RecalculateTransformMatrix();
}

void DXW_D2D_ResetTransformMatrix()
{
	LOG_TRACE("DXW_D2D_ResetTransformMatrix called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_ResetTransformMatrix();
}

void DXW_D2D_DrawText(const WCHAR* text, float left, float top, float right, float bottom)
{
	LOG_TRACE("DXW_D2D_RecalculateTransformMatrix called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_DrawText(text, D2D1::RectF(left, top, right, bottom));
}

void DXW_D2D_FillRoundedRectangle(float left, float top, float right, float bottom, float radiusX, float radiusY)
{
	LOG_TRACE("DXW_D2D_FillRoundedRectangle called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_FillRoundedRectangle(D2D1::RectF(left, top, right, bottom), radiusX, radiusY);
}

void DXW_D2D_DrawRoundedRectangle(float left, float top, float right, float bottom, float radiusX, float radiusY)
{
	LOG_TRACE("DXW_D2D_DrawRoundedRectangle called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_DrawRoundedRectangle(D2D1::RectF(left, top, right, bottom), radiusX, radiusY);
}

void DXW_D2D_FillRectangle(float left, float top, float right, float bottom)
{
	LOG_TRACE("DXW_D2D_FillRectangle called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_FillRectangle(D2D1::RectF(left, top, right, bottom));
}

void DXW_D2D_DrawRectangle(float left, float top, float right, float bottom)
{
	LOG_TRACE("DXW_D2D_DrawRectangle called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_DrawRectangle(D2D1::RectF(left, top, right, bottom));
}

void DXW_D2D_DrawEllipse(float centerX, float centerY, float radiusX, float radiusY, float strokeWidth)
{
	LOG_TRACE("DXW_D2D_DrawEllipse called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_DrawEllipse(D2D1::Ellipse(D2D1::Point2F(centerX, centerY), radiusX, radiusY), strokeWidth);
}

void DXW_D2D_FillEllipse(float centerX, float centerY, float radiusX, float radiusY)
{
	LOG_TRACE("DXW_D2D_FillEllipse called");
	DxwSharedContext::GetInstance().GetCurrentWindow()->D2D_FillEllipse(D2D1::Ellipse(D2D1::Point2F(centerX, centerY), radiusX, radiusY));
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