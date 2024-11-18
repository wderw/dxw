#pragma once

#include <windows.h>
#include <unordered_map>
#include <memory>
#include <functional>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wrl.h> // ComPtr

#include "TransformBuffer.h"

using Microsoft::WRL::ComPtr;

namespace dxw
{

class DxwWindow
{
public:
	DxwWindow();
	~DxwWindow();

	void InitDirectX(HWND);
	void InitDirect3D(HWND);
	void InitDirect2D();
	void CreateBrushResources();
	void CreateTextResources();
	bool IsInitialized() const;

	int GetId() const { return id; }
	DirectX::XMMATRIX RecalculateTransformMatrix();

	void D3D_Clear(float, float, float, float);
	void D3D_SetScale(float, float, float);
	void D3D_SetRotation(float, float, float);
	void D3D_SetTranslation(float, float, float);
	void D3D_RecalculateTransformMatrix();
	void D3D_SetPerspectiveProjectionMatrix(float, float, float, float);
	void D3D_ResetProjectionMatrix();
	void D3D_ResetTransformMatrix();
	void D3D_UpdateMatrixSubresources();
	void D3D_SetTopology(D3D11_PRIMITIVE_TOPOLOGY);
	void D3D_Draw(int, int);

	void D2D_Clear();
	void D2D_DrawLine(float, float, float, float);
	void D2D_BeginDraw();
	void D2D_EndDraw();
	void DX_Present(int);
	void DemoRT();
	void DemoNRT(float);
	void ResizeWindow(unsigned int, unsigned int);

	bool operator==(const DxwWindow& other) const
	{
		return id == other.id;
	}

private:
	void SetWindowSize(int, int);
	void SetD3DViewport(int, int);
	void ResizeD3DSwapChain(UINT, UINT);
	void PrepareConstantTransformBuffer();
	void PrintAdapterInfo();
	void PrintSystemInfo();

	int id{-1};
	static int instanceCounter;
	bool isDirectXInitialized{ false };
	int windowWidth{ 0 };
	int windowHeight{ 0 };

	DirectX::XMMATRIX scalingMatrix{ DirectX::XMMatrixIdentity() };
	DirectX::XMMATRIX rotationMatrix{ DirectX::XMMatrixIdentity() };
	DirectX::XMMATRIX translationMatrix{ DirectX::XMMatrixIdentity() };
	DirectX::XMMATRIX transformMatrix{ DirectX::XMMatrixIdentity() };
	DirectX::XMMATRIX projectionMatrix{ DirectX::XMMatrixIdentity() };

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
	ComPtr<ID3D11Buffer> pVertexBuffer{ nullptr };
	ComPtr<ID3D11InputLayout> pInputLayout{ nullptr };
	ComPtr<ID3D11VertexShader> pVertexShader{ nullptr };
	ComPtr<ID3D11PixelShader> pPixelShader{ nullptr };
	ComPtr<ID3D11Texture2D> pDepthStencilBuffer{ nullptr };
	ComPtr<ID3D11DepthStencilView> pDepthStencilView{ nullptr };
	ComPtr<ID3D11Buffer> pTransformBuffer{ nullptr };
	ComPtr<IDXGISurface> pSurface{ nullptr };

	ComPtr<ID3D11Buffer> pLineVertexBuffer{ nullptr };

	TransformBuffer transformBufferData{ DirectX::XMMatrixIdentity(), DirectX::XMMatrixIdentity() };
	D3D11_BUFFER_DESC transformBufferDesc{};
};

}
