#pragma once

#include <unordered_map>

#include "Log.h"
#include "DxwWindow.h"

namespace dxw
{
class DxwSharedContext
{
public:
    static DxwSharedContext& GetInstance()
    {
        static DxwSharedContext instance;
        return instance;
    }

    void RegisterWindow(std::shared_ptr<DxwWindow> window)
    {
        int id = window->GetId();
        auto result = windows.insert({ id, window });
        if (result.second)
        {
            LOG_INFO("Registered window with id: {}. Current number of windows: {}", id, windows.size());
        }
        else
        {
            LOG_ERROR("DxwWindow with id {} was already registered!", id);
        }
    }

    std::shared_ptr<DxwWindow> GetWindowByID(int id)
    {
        return windows[id];
    }

private:
    DxwSharedContext()
    {
        LOG_DEBUG("Shared context created");
    }

    ~DxwSharedContext()
    {
        LOG_DEBUG("Shared context destroyed");
    }

    DxwSharedContext(const DxwSharedContext&) = delete;
    DxwSharedContext& operator=(const DxwSharedContext&) = delete;

    std::unordered_map<int, std::shared_ptr<DxwWindow>> windows;

public:
    constexpr static const char* vertexShaderSource = R"(
        struct VS_INPUT {
            float4 position : POSITION;
            float4 color : COLOR;
        };

        struct VS_OUTPUT {
            float4 position : SV_POSITION;
            float4 color : COLOR;
        };

        cbuffer TransformBuffer : register(b0) {
            float4x4 transform;  // World transformation matrix
            float4x4 projection; // Projection matrix
        };

        VS_OUTPUT main(VS_INPUT input) {
            VS_OUTPUT output;
            output.position = mul(mul(input.position, transform), projection); // Apply transformation and projection
            output.color = input.color;
            return output;
        }
    )";

    constexpr static const char* pixelShaderSource = R"(
        struct PS_INPUT {
            float4 position : SV_POSITION;
            float4 color : COLOR;
        };

        float4 main(PS_INPUT input) : SV_TARGET {
            return input.color; // Output the color received from the vertex shader
        }
    )";
};

}