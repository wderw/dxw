#pragma once

#include <vector>
#include <sstream>

#include "Log.h"
#include "Vertex.h"

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
};
}
