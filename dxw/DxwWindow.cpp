#include "DxwWindow.h"

namespace dxw
{

int DxwWindow::instanceCounter = 0;

DxwWindow::DxwWindow()
{
	id = instanceCounter++;
}

HRESULT DxwWindow::InitDirect3D(HWND hwnd)
{
	return E_NOTIMPL;
}

HRESULT DxwWindow::InitDirect2D()
{
	return E_NOTIMPL;
}

HRESULT DxwWindow::InitDirectX(HWND hwnd)
{
	InitDirect3D(hwnd);
	InitDirect2D();

	return E_NOTIMPL;
}

}