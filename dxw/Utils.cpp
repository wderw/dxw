#include "Utils.h"

namespace dxw
{

const char* Utils::ExtractBaseName(const char* full_path)
{
	const char* base_name = strrchr(full_path, '\\');
	return base_name ? base_name + 1 : full_path;
}

float Utils::ConvertPixelToNDCX(int pixelX, int width, float aspectRatio)
{
	return ((2.0f * pixelX) / width - 1.0f) * aspectRatio;
}

float Utils::ConvertPixelToNDCY(int pixelY, int height)
{
	return 1.0f - (2.0f * pixelY) / height;
}

float Utils::RandomFloat(float min, float max)
{
	return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

std::vector<Vertex> Utils::GenerateTetrahedron()
{
	// tetrahedron
	const float factor = 0.1f;
	std::vector<Vertex> vertices =
	{
		{ DirectX::XMFLOAT3(1 * factor,  1 * factor,  1 * factor), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // A (Top)
		{ DirectX::XMFLOAT3(1 * factor, -1 * factor, -1 * factor), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // B (Bottom-Right)
		{ DirectX::XMFLOAT3(-1 * factor, 1 * factor, -1 * factor), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }, // C (Bottom-Left)
		{ DirectX::XMFLOAT3(-1 * factor, -1 * factor,  1 * factor), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) }  // D (Back)
	};

	std::vector<Vertex> tetrahedron;
	tetrahedron.reserve(12);
	std::array<int, 12> order = { 0,1,2,0,1,3,0,2,3,1,2,3 };
	for (int i : order)
	{
		tetrahedron.push_back(vertices[i]);
	}

	return tetrahedron;
}

std::vector<Vertex> Utils::GenerateLines(int width, int height)
{
	// lines
	constexpr static int DRAWLIB_COUNT{ 100000 };
	std::vector<Vertex> lineVerts{};
	lineVerts.resize(DRAWLIB_COUNT);
	for (size_t i = 0; i < DRAWLIB_COUNT; ++i)
	{
		float x = Utils::ConvertPixelToNDCX(0, width, static_cast<float>(width) / static_cast<float>(height));
		float y = Utils::ConvertPixelToNDCY(0, height);
		lineVerts[i].position = DirectX::XMFLOAT3(x, y, 0.0f);
		lineVerts[i].color = DirectX::XMFLOAT4(1, 0, 0, 1);
	}

	for (size_t i = 0; i < DRAWLIB_COUNT; i += 2)
	{
		float x = Utils::ConvertPixelToNDCX(std::rand() % width, width, static_cast<float>(width) / static_cast<float>(height));
		float y = Utils::ConvertPixelToNDCY(std::rand() % height, height);
		lineVerts[i].position = DirectX::XMFLOAT3(x, y, 0.0f);
		lineVerts[i + 1].color = DirectX::XMFLOAT4(Utils::RandomFloat(-1.0f, 1.0f), Utils::RandomFloat(-1.0f, 1.0f), Utils::RandomFloat(-1.0f, 1.0f), 1);
	}
	return lineVerts;
}

void Utils::HandleHResultError(HRESULT hr)
{
	std::stringstream ss;
	ss << "HRESULT Error: (HRESULT: 0x" << std::hex << hr << ")";
	LOG_ERROR("{}", ss.str().c_str());
}

D3D11_BUFFER_DESC Utils::VertexBufferDesc(const std::vector<Vertex>& vertices)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(vertices.size());
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	return bufferDesc;
}

D3D11_BUFFER_DESC Utils::TransformBufferDesc()
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(TransformBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	return bufferDesc;
}

}