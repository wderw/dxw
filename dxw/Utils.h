#pragma once

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
};
}
