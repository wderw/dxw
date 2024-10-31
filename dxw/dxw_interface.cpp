#include <memory>

#include "dxw_interface.h"
#include "DxwWindow.h"
#include "DxwSharedContext.h"

namespace dxw
{

HRESULT DXW_InitWindow(HWND hWnd)
{
	auto window = std::make_shared<DxwWindow>();
	window->InitDirectX(hWnd);
	DxwSharedContext::GetInstance().RegisterWindow(window);
	return window->GetId();
}

}