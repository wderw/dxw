#include "DxwSharedContext.h"

namespace dxw
{

DxwSharedContext& DxwSharedContext::GetInstance()
{
    static DxwSharedContext instance;
    return instance;
}

void DxwSharedContext::ReleaseDxwResources()
{
    windows.clear();
}

void DxwSharedContext::RegisterWindow(std::shared_ptr<DxwWindow> window)
{
    int id = window->GetId();
    auto result = windows.insert({ id, window });
    if (result.second)
    {
        LOG_INFO("Registered window with id: {}. Current number of windows: {}", id, windows.size());
    }
    else
    {
        LOG_WARN("Window with id: {} was not found!", id);
    }
    SetTargetId(id);
}

void DxwSharedContext::SetTargetId(int newId)
{
    LOG_DEBUG("Setting new drawing target. Old target id: {}, new target id: {}", targetId, newId);
    targetId = newId;
}

int DxwSharedContext::GetTargetId()
{
    return targetId;
}

std::shared_ptr<DxwWindow> DxwSharedContext::GetWindowByID(int id)
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

std::shared_ptr<DxwWindow> DxwSharedContext::GetCurrentWindow()
{
    return GetWindowByID(GetTargetId());
}

const char* DxwSharedContext::GetVertexShaderSource()
{
    return R"(
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
}

const char* DxwSharedContext::GetPixelShaderSource()
{
    return R"(
        struct PS_INPUT {
            float4 position : SV_POSITION;
            float4 color : COLOR;
        };

        float4 main(PS_INPUT input) : SV_TARGET {
            return input.color; // Output the color received from the vertex shader
        }
    )";
}

DxwSharedContext::DxwSharedContext()
{
    LOG_DEBUG("Creating Shared Context");
}

DxwSharedContext::~DxwSharedContext()
{
    LOG_DEBUG("Destroying Shared Context ");
}

}
