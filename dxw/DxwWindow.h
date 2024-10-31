#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>

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
