#include "Log.h"
#include "DxwWindow.h"
#include "DxwSharedContext.h"
#include "Vertex.h"

namespace dxw
{

int DxwWindow::instanceCounter = 0;

DxwWindow::DxwWindow()
{
	id = instanceCounter++;
}

void DxwWindow::InitDirect3D(HWND hWnd)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	UINT width = clientRect.right - clientRect.left;
	UINT height = clientRect.bottom - clientRect.top;

	// swap chain + 3d device context
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format for Direct2D
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // flag for Direct2D
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		pSwapChain.GetAddressOf(),
		pD3DDevice.GetAddressOf(),
		nullptr,
		pD3DDeviceContext.GetAddressOf()
	);
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create Swap Chain and D3D11 Device Context!");
		return;
	}


	// back buffer and render target
	ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to get back buffer!");
		return;
	}

	hr = pD3DDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create RenderTargetView!");
		return;
	}

	pD3DDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);

	// d3d viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = 800.0f;
	viewport.Height = 600.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	pD3DDeviceContext->RSSetViewports(1, &viewport);

	// buffer of vertex data
	std::vector<Vertex> verts{};
	verts.resize(DRAWLIB_COUNT);
	for (size_t i = 0; i < DRAWLIB_COUNT; ++i)
	{
		float x = ConvertPixelToNDCX(0, 800, 800.0f / 600.0f);
		float y = ConvertPixelToNDCY(0, 600);
		verts[i].position = DirectX::XMFLOAT3(x, y, 0.0f);
		verts[i].color = DirectX::XMFLOAT4(1, 0, 0, 1);
	}

	for (size_t i = 0; i < DRAWLIB_COUNT; i += 2)
	{
		float x = ConvertPixelToNDCX(std::rand() % 800, 800, 800.0f / 600.0f);
		float y = ConvertPixelToNDCY(std::rand() % 600, 600);
		verts[i].position = DirectX::XMFLOAT3(x, y, 0.0f);
		verts[i + 1].color = DirectX::XMFLOAT4(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), 1);
	}

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = DRAWLIB_COUNT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = verts.data();

	hr = pD3DDevice->CreateBuffer(&bufferDesc, &initData, pVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create vertex buffer!");
		return;
	}

	// memory layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);


	// shaders
	ComPtr<ID3DBlob> pVSBlob = nullptr;
	ComPtr<ID3DBlob> pPSBlob = nullptr;

	hr = D3DCompile(DxwSharedContext::GetInstance().vertexShaderSource, strlen(DxwSharedContext::GetInstance().vertexShaderSource), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &pVSBlob, nullptr);
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to compile vertex shader!");
		return;
	}

	hr = D3DCompile(DxwSharedContext::GetInstance().pixelShaderSource, strlen(DxwSharedContext::GetInstance().pixelShaderSource), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &pPSBlob, nullptr);
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to compile pixel shader!");
		return;
	}

	hr = pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, pVertexShader.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create vertex shader!");
		return;
	}

	hr = pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, pPixelShader.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create pixel shader!");
		return;
	}

	hr = pD3DDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), pInputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create input layout!");
		return;
	}

	// blend state
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ComPtr<ID3D11BlendState> blendState;
	hr = pD3DDevice->CreateBlendState(&blendDesc, blendState.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create blend state!");
		return;
	}
	pD3DDeviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);
}

void DxwWindow::InitDirect2D()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory1), nullptr, (void**)&pD2DFactory);
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create Direct2D factory.");
		return;
	}

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	hr = pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to get IDXGIDevice from D3D device.");
		return;
	}

	hr = pD2DFactory->CreateDevice(pDXGIDevice.Get(), pD2DDevice.GetAddressOf());
	if (FAILED(hr) || pD2DDevice == nullptr)
	{
		LOG_ERROR("Failed to create Direct2D device.");
		return;
	}

	hr = pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, pD2DDeviceContext.GetAddressOf());
	if (FAILED(hr) || pD2DDeviceContext == nullptr)
	{
		LOG_ERROR("Failed to create Direct2D device context.");
		return;
	}
	pD2DDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	ComPtr<ID3D11Texture2D> pBackBufferTexture = nullptr;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBufferTexture.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		ComPtr<IDXGISurface> pSurface = nullptr;
		hr = pBackBufferTexture->QueryInterface(__uuidof(IDXGISurface), (void**)pSurface.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			auto props = D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(
					DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

			hr = pD2DDeviceContext->CreateBitmapFromDxgiSurface(
				pSurface.Get(),
				props,
				pD2DBitmap.GetAddressOf()
			);
		}
	}

	if (pD2DBitmap)
	{
		pD2DDeviceContext->SetTarget(pD2DBitmap.Get());
	}
	else
	{
		LOG_ERROR("Direct2D Bitmap creation failed!");
	}
}

void DxwWindow::CreateTextResources()
{
	HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(pDWriteFactory.GetAddressOf()));

	hr = pDWriteFactory->CreateTextFormat(
		L"Arial",
		nullptr,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f, // font size
		L"en-us",
		pDefaultTextFormat.GetAddressOf()
	);

	if (SUCCEEDED(hr))
	{
		pDefaultTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pDefaultTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	}
}

void DxwWindow::InitDirectX(HWND hWnd)
{
	InitDirect3D(hWnd);
	InitDirect2D();
	CreateTextResources();
}

}