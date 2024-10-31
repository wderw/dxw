#pragma once

#include <windows.h>

namespace dxw
{

class DxwWindow
{
public:
	DxwWindow();

	HRESULT InitDirectX(HWND);
	HRESULT InitDirect3D(HWND);
    HRESULT InitDirect2D();
	int GetId() { return id; }

private:
	int id{-1};
	static int instanceCounter;
};

}
