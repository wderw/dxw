#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wrl.h> // ComPtr
#include <unordered_map>
#include <memory>
#include <functional>

using Microsoft::WRL::ComPtr;

namespace dxw
{

class DxwWindow
{
public:
	DxwWindow();

	void InitDirectX(HWND);
	void InitDirect3D(HWND);
	void InitDirect2D();
	void CreateTextResources();
	bool IsInitialized();

	int GetId() { return id; }

	void D3D_Clear();
	void D2D_Clear();
	void D2D_BeginDraw();
	void D2D_EndDraw();
	void DX_Present(int);
	void RunThreadedTest();

	bool operator==(const DxwWindow& other) const
	{
		return id == other.id;
	}

private:
	int id{-1};
	static int instanceCounter;
	constexpr static int DRAWLIB_COUNT{ 100000 };
	bool isDirectXInitialized{ false };

	ComPtr<ID3D11Device> pD3DDevice{ nullptr };
	ComPtr<ID3D11DeviceContext> pD3DDeviceContext{ nullptr };
	ComPtr<IDXGISwapChain> pSwapChain{ nullptr };
	ComPtr<ID3D11RenderTargetView> pRenderTargetView{ nullptr };
	ComPtr<ID2D1Factory1> pD2DFactory{ nullptr };
	ComPtr<ID2D1Device> pD2DDevice{ nullptr };
	ComPtr<ID2D1DeviceContext> pD2DDeviceContext{ nullptr };
	ComPtr<ID2D1Bitmap1> pD2DBitmap{ nullptr };
	ComPtr<IDWriteFactory> pDWriteFactory{ nullptr };
	ComPtr<IDWriteTextFormat> pDefaultTextFormat{ nullptr };
	ComPtr<ID2D1SolidColorBrush> pDefaultBrush{ nullptr };
	ComPtr<ID2D1SolidColorBrush> pDefaultBrush2{ nullptr };
	ComPtr<ID3D11Buffer> pVertexBuffer{ nullptr };
	ComPtr<ID3D11InputLayout> pInputLayout{ nullptr };
	ComPtr<ID3D11VertexShader> pVertexShader{ nullptr };
	ComPtr<ID3D11PixelShader> pPixelShader{ nullptr };
	ComPtr<ID3D11Texture2D> pDepthStencilBuffer{ nullptr };
	ComPtr<ID3D11DepthStencilView> pDepthStencilView{ nullptr };
};

}
