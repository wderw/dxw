#include <thread>

#include "DxwWindow.h"

#include "Log.h"
#include "DxwSharedContext.h"
#include "Vertex.h"
#include "Utils.h"

namespace dxw
{

int DxwWindow::instanceCounter = 0;

DxwWindow::DxwWindow()
{
	id = instanceCounter++;
}

void DxwWindow::D3D_Clear()
{
	float colorDarkGray[4] = { 0.15f, 0.15f, 0.15f, 1.0f };
	pD3DDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), colorDarkGray);
}

void DxwWindow::D3D_SetScale(float x, float y, float z)
{
	scalingMatrix = DirectX::XMMatrixScaling(x, y, z);
}

void DxwWindow::D3D_SetRotation(float degreesX, float degreesY, float degreesZ)
{
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(degreesX), DirectX::XMConvertToRadians(degreesY), DirectX::XMConvertToRadians(degreesZ));
}

void DxwWindow::D3D_SetTranslation(float x, float y, float z)
{
	translationMatrix = DirectX::XMMatrixTranslation(x, y, z);
}

void DxwWindow::D3D_RecalculateTransformMatrix()
{
	transformMatrix = XMMatrixMultiply(scalingMatrix, rotationMatrix);
	transformMatrix = DirectX::XMMatrixMultiply(transformMatrix, translationMatrix);
}

void DxwWindow::D3D_SetPerspectiveProjectionMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
		fieldOfView,
		aspectRatio,
		nearPlane,
		farPlane
	);
}

void DxwWindow::D3D_ResetProjectionMatrix()
{
	projectionMatrix = DirectX::XMMatrixIdentity();
}

void DxwWindow::D3D_ResetTransformMatrix()
{
	transformMatrix = DirectX::XMMatrixIdentity();
}

void DxwWindow::D3D_UpdateMatrixSubresources()
{
	transformBufferData.transform = DirectX::XMMatrixTranspose(transformMatrix); // transpose needed for HLSL
	transformBufferData.projection = DirectX::XMMatrixTranspose(projectionMatrix);
	pD3DDeviceContext->UpdateSubresource(transformBuffer.Get(), 0, nullptr, &transformBufferData, 0, 0);
}

void DxwWindow::D3D_SetTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	pD3DDeviceContext->IASetPrimitiveTopology(primitiveTopology);
}

void DxwWindow::D3D_Draw(int vertexCount, int startVertexLocation)
{
	pD3DDeviceContext->Draw(vertexCount, startVertexLocation);
}

void DxwWindow::D2D_Clear()
{
	pD2DDeviceContext->Clear(D2D1::ColorF(0, 1, 0, 1));
}

void DxwWindow::D2D_BeginDraw()
{
	pD2DDeviceContext->BeginDraw();
}

void DxwWindow::D2D_EndDraw()
{
	pD2DDeviceContext->EndDraw();
}

void DxwWindow::DX_Present(int vsync = 1)
{
	// 1 = vsync enabled, 0 = immediate presentation
	pSwapChain->Present(vsync, 0);
}

DirectX::XMMATRIX DxwWindow::RecalculateTransformMatrix()
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
    return DirectX::XMMatrixMultiply(matrix, translationMatrix);
}

void DxwWindow::SetWindowSize(int width, int height)
{
	LOG_DEBUG("Setting new DxwWindow size: [{},{}]", width, height);
	windowWidth = width;
	windowHeight = height;
}

void DxwWindow::SetD3DViewport(int width, int height)
{
	LOG_DEBUG("Setting D3D Viewport");
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	pD3DDeviceContext->RSSetViewports(1, &viewport);
}

void DxwWindow::ResizeWindow(unsigned int w, unsigned int h)
{
	SetWindowSize(w, h);
	ResizeD3DSwapChain(w, h);
	SetD3DViewport(w, h);
}

void DxwWindow::ResizeD3DSwapChain(UINT width, UINT height)
{
	LOG_DEBUG("Requested new swap chain width: {} height: {}", width, height);

	pD2DBitmap.Reset();
	pD2DDeviceContext.Reset();

	// RenderTargetView and backBuffer must be released
	// before attempting to resize the swap chain buffer!
	pRenderTargetView.Reset();
	pDepthStencilBuffer.Reset();
	pDepthStencilView.Reset();

	HRESULT hr = pSwapChain->ResizeBuffers(
		0,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0
	);

	if (FAILED(hr))
	{
		LOG_ERROR("Could not resize swap chain buffer!");
		Utils::HandleHResultError(hr);
		return;
	}

	// recreate the render target view from the new back buffer
	ComPtr<ID3D11Texture2D> backBuffer;
	hr = pSwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	if (FAILED(hr))
	{
		LOG_ERROR("Could not get back buffer!");
		return;
	}

	hr = pD3DDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, pRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Could not create render target view!");
		return;
	}

	// Recreate the depth stencil buffer and view if needed
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = pD3DDevice->CreateTexture2D(&depthDesc, nullptr, pDepthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Could not create depth stencil buffer!");
		return;
	}

	hr = pD3DDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), nullptr, pDepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Could not create depth stencil view!");
		return;
	}

	// Set the render target and depth stencil view to the output merger stage
	pD3DDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());

	// reinitialize required direct2d resources
	InitDirect2D();
}

void DxwWindow::NRTDemo()
{
	float fi = 0;

	std::vector<Vertex> lineVerts = Utils::GenerateLines(windowWidth, windowHeight);
	std::vector<Vertex> tetrahedronVerts = Utils::GenerateTetrahedron();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(tetrahedronVerts.size());
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = tetrahedronVerts.data();

	LOG_DEBUG("Creating vertex buffer");
	HRESULT hr = pD3DDevice->CreateBuffer(&bufferDesc, &initData, pVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create vertex buffer!");
		return;
	}

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pD3DDeviceContext->IASetInputLayout(pInputLayout.Get());
	pD3DDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	pD3DDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
	pD3DDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(TransformBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	pD3DDevice->CreateBuffer(&bufferDesc, nullptr, transformBuffer.GetAddressOf());
	pD3DDeviceContext->VSSetConstantBuffers(0, 1, transformBuffer.GetAddressOf());
	D3D_UpdateMatrixSubresources();

	pD2DDeviceContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0, 1, 0, 1.0f)),
		pDefaultBrush.GetAddressOf()
	);

	pD2DDeviceContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(1, 1, 1, 0.3f)),
		pDefaultBrush2.GetAddressOf()
	);

	fi += 1.0f;
	D3D_Clear();

	D2D_BeginDraw();
	pD2DDeviceContext->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(250, 250, 600, 400), 15.0f, 15.0f), pDefaultBrush2.Get());
	D2D_EndDraw();

	pD3DDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	D3D_SetPerspectiveProjectionMatrix(DirectX::XM_PIDIV4, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.01f, 100.0f);
	D3D_SetTranslation(0, 0, 1);
	D3D_SetScale(1.5f, 1.5f, 1.5f);
	D3D_SetRotation(fi, fi + fi / 2, 0);
	D3D_RecalculateTransformMatrix();
	D3D_UpdateMatrixSubresources();

	D3D_SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	D3D_Draw(12, 0);

	D3D_SetScale(1.2f, 1.2f, 1.2f);
	D3D_SetRotation(fi / 3, fi * 1.2f, fi / 2);
	D3D_RecalculateTransformMatrix();
	D3D_UpdateMatrixSubresources();

	D3D_SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	D3D_Draw(12, 0);

	//D2D_BeginDraw();

	//wchar_t fpsText[80] = L"TEST test za¿ó³æ gêœl¹ jaŸñ The quick brown fox jumps over the lazy dog";
	//D2D1_RECT_F textRect = D2D1::RectF(0, 0, 250, 50);

	//pD2DDeviceContext->DrawTextW(
	//	fpsText,
	//	wcslen(fpsText),
	//	pDefaultTextFormat.Get(),
	//	textRect,
	//	pDefaultBrush.Get()
	//);
	//pD2DDeviceContext->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(80, 80, 400, 500), 15.0f, 15.0f), pDefaultBrush2.Get());

	//D2D_EndDraw();

	DX_Present(1);
}

void DxwWindow::RunThreadedTest()
{
	//ResizeWindow(400, 300);

	std::thread([&]()
		{
			float fi = 0;

			std::vector<Vertex> lineVerts = Utils::GenerateLines(windowWidth, windowHeight);
			std::vector<Vertex> tetrahedronVerts = Utils::GenerateTetrahedron();

			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(tetrahedronVerts.size());
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = tetrahedronVerts.data();

			LOG_DEBUG("Creating vertex buffer");
			HRESULT hr = pD3DDevice->CreateBuffer(&bufferDesc, &initData, pVertexBuffer.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("Failed to create vertex buffer!");
				return;
			}

			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			pD3DDeviceContext->IASetInputLayout(pInputLayout.Get());
			pD3DDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

			pD3DDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
			pD3DDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

			ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(TransformBuffer);
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;

			pD3DDevice->CreateBuffer(&bufferDesc, nullptr, transformBuffer.GetAddressOf());
			pD3DDeviceContext->VSSetConstantBuffers(0, 1, transformBuffer.GetAddressOf());
			D3D_UpdateMatrixSubresources();

			//pD2DDeviceContext->CreateSolidColorBrush(
			//	D2D1::ColorF(D2D1::ColorF(0, 1, 0, 1.0f)),
			//	pDefaultBrush.GetAddressOf()
			//);

			pD2DDeviceContext->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(1, 1, 1, 0.3f)),
				pDefaultBrush2.GetAddressOf()
			);

			while (true)
			{
				fi += 1.0f;
				D3D_Clear();

				D2D_BeginDraw();
				pD2DDeviceContext->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(250, 250, 600, 400), 15.0f, 15.0f), pDefaultBrush2.Get());
				D2D_EndDraw();

				pD3DDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

				D3D_SetPerspectiveProjectionMatrix(DirectX::XM_PIDIV4, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.01f, 100.0f);
				D3D_SetTranslation(0, 0, 1);
				D3D_SetScale(1.5f, 1.5f, 1.5f);
				D3D_SetRotation(fi, fi + fi / 2, 0);
				D3D_RecalculateTransformMatrix();
				D3D_UpdateMatrixSubresources();

				D3D_SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
				D3D_Draw(12, 0);

				D3D_SetScale(1.2f, 1.2f, 1.2f);
				D3D_SetRotation(fi / 3, fi * 1.2f, fi / 2);
				D3D_RecalculateTransformMatrix();
				D3D_UpdateMatrixSubresources();

				D3D_SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
				D3D_Draw(12, 0);

				//D2D_BeginDraw();

				//wchar_t fpsText[80] = L"TEST test za¿ó³æ gêœl¹ jaŸñ The quick brown fox jumps over the lazy dog";
				//D2D1_RECT_F textRect = D2D1::RectF(0, 0, 250, 50);

				//pD2DDeviceContext->DrawTextW(
				//	fpsText,
				//	wcslen(fpsText),
				//	pDefaultTextFormat,
				//	textRect,
				//	pDefaultBrush
				//);
				//pD2DDeviceContext->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(80, 80, 400, 500), 15.0f, 15.0f), pDefaultBrush2);

				//D2D_EndDraw();

				DX_Present(1);
			}
		}).detach();
}

void DxwWindow::InitDirect3D(HWND hWnd)
{
	LOG_DEBUG("Direct3D initialization started");
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	SetWindowSize(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	LOG_DEBUG("Initializing Swap Chain and D3D11 Device Context");
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

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

	LOG_DEBUG("Initializing back buffer");
	ComPtr<ID3D11Texture2D> backBuffer{ nullptr };
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to get back buffer!");
		return;
	}

	LOG_DEBUG("Creating RenderTargetView");
	hr = pD3DDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, pRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create RenderTargetView!");
		return;
	}

	pD3DDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);

	LOG_DEBUG("Creating D3D11 Viewport");
	SetD3DViewport(windowWidth, windowHeight);

	// Create the depth buffer description
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = windowWidth;
	depthStencilDesc.Height = windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Common depth format
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// Create the depth buffer
	hr = pD3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, pDepthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Could not create texture 2D for depth buffer!");
		return;
	}

	// Create the depth stencil view
	pD3DDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), nullptr, pDepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Could not create the depth stencil view!");
		return;
	}

	// Bind the depth stencil view to the output merger stage
	pD3DDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());

	// memory layout
	LOG_DEBUG("Preparing buffer memory layout");
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	// shaders
	LOG_DEBUG("Compiling shaders");
	ComPtr<ID3DBlob> pVSBlob{ nullptr };
	ComPtr<ID3DBlob> pPSBlob{ nullptr };

	hr = D3DCompile(DxwSharedContext::GetInstance().vertexShaderSource, strlen(DxwSharedContext::GetInstance().vertexShaderSource), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, pVSBlob.ReleaseAndGetAddressOf(), nullptr);
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to compile vertex shader!");
		return;
	}

	hr = D3DCompile(DxwSharedContext::GetInstance().pixelShaderSource, strlen(DxwSharedContext::GetInstance().pixelShaderSource), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, pPSBlob.ReleaseAndGetAddressOf(), nullptr);
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

	LOG_DEBUG("Creating Blend State");
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

	ComPtr<ID3D11BlendState> blendState{ nullptr };
	hr = pD3DDevice->CreateBlendState(&blendDesc, blendState.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create blend state!");
		return;
	}
	pD3DDeviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);
	LOG_INFO("Direct3D initialization complete");
}

void DxwWindow::InitDirect2D()
{
	LOG_DEBUG("Direct2D initialization started");

	LOG_DEBUG("Creating D2D1Factory");
	if (pD2DFactory == nullptr)
	{
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory1), nullptr, (void**)pD2DFactory.GetAddressOf());
		if (FAILED(hr))
		{
			LOG_ERROR("Failed to create Direct2D factory.");
			return;
		}
	}

	if (pD2DDevice == nullptr)
	{
		LOG_DEBUG("Creating IDXGIDevice from D3D device");
		ComPtr<IDXGIDevice> pDXGIDevice{ nullptr };
		HRESULT hr = pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
		if (FAILED(hr))
		{
			LOG_ERROR("Failed to get IDXGIDevice from D3D device.");
			return;
		}

		LOG_DEBUG("Creating Direct2D Device");
		hr = pD2DFactory->CreateDevice(pDXGIDevice.Get(), pD2DDevice.GetAddressOf());
		if (FAILED(hr) || pD2DDevice == nullptr)
		{
			LOG_ERROR("Failed to create Direct2D device.");
			return;
		}
	}

	LOG_DEBUG("Creating Direct2D device context");
	HRESULT hr = pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, pD2DDeviceContext.GetAddressOf());
	if (FAILED(hr) || pD2DDeviceContext == nullptr)
	{
		LOG_ERROR("Failed to create Direct2D device context.");
		return;
	}
	pD2DDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	LOG_DEBUG("Creating bitmap from DxgiSurface");

	ComPtr<ID3D11Texture2D> pBackBufferTexture{ nullptr };
	ComPtr<IDXGISurface> pSurface{ nullptr };

	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBufferTexture.GetAddressOf());
	if (SUCCEEDED(hr))
	{
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

	LOG_INFO("Direct2D initialization complete");
}

void DxwWindow::CreateTextResources()
{
	LOG_DEBUG("Creating text resources");
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
	else
	{
		LOG_ERROR("Error creating text format!");
	}
}

bool DxwWindow::IsInitialized() const
{
	return isDirectXInitialized;
}

void DxwWindow::InitDirectX(HWND hWnd)
{
	LOG_DEBUG("DirectX initialization started");
	InitDirect3D(hWnd);
	InitDirect2D();
	//CreateTextResources();
	isDirectXInitialized = true;
	LOG_INFO("DirectX initialization complete");

//#if defined(DEBUG) || defined(_DEBUG)
//	ComPtr<ID3D11Debug> pDebugDevice;
//	if (SUCCEEDED(pD3DDevice->QueryInterface(pDebugDevice.GetAddressOf())))
//	{
//		pDebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//	}
//#endif
}

}