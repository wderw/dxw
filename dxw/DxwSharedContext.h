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

    void ReleaseDxwResources()
    {
        windows.clear();
        brushes2d.clear();
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
        SetTargetId(id);
    }

    void SetTargetId(int newId)
    {
        LOG_DEBUG("Setting new drawing target. Old target id: {}, new target id: {}", targetId, newId);
        targetId = newId;
    }

    int GetTargetId()
    {
        return targetId;
    }

    std::shared_ptr<DxwWindow> GetWindowByID(int id)
    {
        LOG_TRACE("Attempting to retrieve window by id: {}", id);
        auto it = windows.find(id);

        if (it != windows.end())
        {
            return it->second;
        }
        else
        {
            LOG_WARN("Window with id: {} was not found!", id);
            return nullptr;
        }
    }

    std::shared_ptr<DxwWindow> GetCurrentWindow()
    {
        return GetWindowByID(GetTargetId());
    }

    void AddSolidBrush2D(const std::string& name, ComPtr<ID2D1Brush> brush)
    {
        brushes2d.insert({ name, brush });
    }

    ComPtr<ID2D1Brush> GetSolidBrush2D(const std::string& name)
    {
        auto it = brushes2d.find(name);

        if (it != brushes2d.end())
        {
            return it->second;
        }
        LOG_WARN("SolidBrush2D with name: {} was not found!", name);
    }

private:
    DxwSharedContext()
    {
        LOG_DEBUG("Creating Shared Context");
    }

    ~DxwSharedContext()
    {
        LOG_DEBUG("Destroying Shared Context ");
    }

    DxwSharedContext(const DxwSharedContext&) = delete;
    DxwSharedContext& operator=(const DxwSharedContext&) = delete;

    std::unordered_map<int, std::shared_ptr<DxwWindow>> windows;
    std::unordered_map<std::string, ComPtr<ID2D1Brush>> brushes2d;
    int targetId{ -1 };

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