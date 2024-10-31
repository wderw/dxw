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
	void ReleaseResources()
	{
		//pD3DDevice.Reset();
		//pD3DDeviceContext.Reset();
		//pSwapChain.Reset();
		//pRenderTargetView.Reset();
		//pD2DFactory.Reset();
		//pD2DDevice.Reset();
		//pD2DDeviceContext.Reset();
		//pD2DBitmap.Reset();
		//pDWriteFactory.Reset();
		//pDefaultTextFormat.Reset();
		//pDefaultBrush.Reset();
		//pVertexBuffer.Reset();
		//pInputLayout.Reset();
		//pVertexShader.Reset();
		//pPixelShader.Reset();
	}
	int GetId() { return id; }

private:
	int id{-1};
	static int instanceCounter;
	constexpr static int DRAWLIB_COUNT{ 100000 };

	ID3D11Device* pD3DDevice{ nullptr };
	ID3D11DeviceContext* pD3DDeviceContext{ nullptr };
	IDXGISwapChain* pSwapChain{ nullptr };
	ID3D11RenderTargetView* pRenderTargetView{ nullptr };
	ID2D1Factory1* pD2DFactory{ nullptr };
	ID2D1Device* pD2DDevice{ nullptr };
	ID2D1DeviceContext* pD2DDeviceContext{ nullptr };
	ID2D1Bitmap1* pD2DBitmap{ nullptr };
	IDWriteFactory* pDWriteFactory{ nullptr };
	IDWriteTextFormat* pDefaultTextFormat{ nullptr };
	ID2D1SolidColorBrush* pDefaultBrush{ nullptr };
	ID3D11Buffer* pVertexBuffer{ nullptr };
	ID3D11InputLayout* pInputLayout{ nullptr };
	ID3D11VertexShader* pVertexShader{ nullptr };
	ID3D11PixelShader* pPixelShader{ nullptr };

	float ConvertPixelToNDCX(int pixelX, int width, float aspectRatio)
	{
		return ((2.0f * pixelX) / width - 1.0f) * aspectRatio;
	}

	float ConvertPixelToNDCY(int pixelY, int height)
	{
		return 1.0f - (2.0f * pixelY) / height;
	}

	float RandomFloat(float min, float max)
	{
		return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
	}
};

}
