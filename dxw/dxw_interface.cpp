#include <memory>

#include "dxw_interface.h"
#include "DxwWindow.h"
#include "DxwSharedContext.h"

namespace dxw
{

HRESULT DXW_InitWindow(HWND hwnd)
{
	auto window = std::make_shared<DxwWindow>();
	window->InitDirectX(hwnd);
	DxwSharedContext::GetInstance().RegisterWindow(window);
	return window->GetId();
}

}