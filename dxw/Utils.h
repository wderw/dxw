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
};
}
