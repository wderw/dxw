#pragma once

#include <vector>
#include "Vertex.h"

namespace dxw
{

class Utils
{
public:
    static inline const char* ExtractBaseName(const char* full_path)
    {
        const char* base_name = strrchr(full_path, '\\');
        return base_name ? base_name + 1 : full_path;
    }

	static inline float ConvertPixelToNDCX(int pixelX, int width, float aspectRatio)
	{
		return ((2.0f * pixelX) / width - 1.0f) * aspectRatio;
	}

	static inline float ConvertPixelToNDCY(int pixelY, int height)
	{
		return 1.0f - (2.0f * pixelY) / height;
	}

	static inline float RandomFloat(float min, float max)
	{
		return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
	}

	static inline std::vector<Vertex> GenerateTetrahedron()
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

	static inline std::vector<Vertex> GenerateLines()
	{
		// lines
		constexpr static int DRAWLIB_COUNT{ 100000 };
		std::vector<Vertex> lineVerts{};
		lineVerts.resize(DRAWLIB_COUNT);
		for (size_t i = 0; i < DRAWLIB_COUNT; ++i)
		{
			float x = Utils::ConvertPixelToNDCX(0, 800, 800.0f / 600.0f);
			float y = Utils::ConvertPixelToNDCY(0, 600);
			lineVerts[i].position = DirectX::XMFLOAT3(x, y, 0.0f);
			lineVerts[i].color = DirectX::XMFLOAT4(1, 0, 0, 1);
		}

		for (size_t i = 0; i < DRAWLIB_COUNT; i += 2)
		{
			float x = Utils::ConvertPixelToNDCX(std::rand() % 800, 800, 800.0f / 600.0f);
			float y = Utils::ConvertPixelToNDCY(std::rand() % 600, 600);
			lineVerts[i].position = DirectX::XMFLOAT3(x, y, 0.0f);
			lineVerts[i + 1].color = DirectX::XMFLOAT4(Utils::RandomFloat(-1.0f, 1.0f), Utils::RandomFloat(-1.0f, 1.0f), Utils::RandomFloat(-1.0f, 1.0f), 1);
		}
		return lineVerts;
	}
};
}
