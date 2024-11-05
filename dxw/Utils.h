#pragma once

#include <vector>
#include <sstream>
#include <d3d11.h>

#include "Log.h"
#include "Vertex.h"
#include "TransformBuffer.h"

namespace dxw
{

class Utils
{
public:
	static const char* ExtractBaseName(const char* full_path);
	static float ConvertPixelToNDCX(int pixelX, int width, float aspectRatio);
	static float ConvertPixelToNDCY(int pixelY, int height);
	static float RandomFloat(float min, float max);
	static std::vector<Vertex> GenerateTetrahedron();
	static std::vector<Vertex> GenerateLines(int width, int height);
	static void HandleHResultError(HRESULT hr);
	static D3D11_BUFFER_DESC VertexBufferDesc(const std::vector<Vertex>&);
	static D3D11_BUFFER_DESC TransformBufferDesc();
};
}
