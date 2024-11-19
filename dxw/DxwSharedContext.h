#pragma once

#include <unordered_map>

#include "Log.h"
#include "DxwWindow.h"

namespace dxw
{
class DxwSharedContext
{
public:
    static DxwSharedContext& GetInstance();
    void ReleaseDxwResources();
    void RegisterWindow(std::shared_ptr<DxwWindow> window);
    void SetTargetId(int newId);
    int GetTargetId();
    std::shared_ptr<DxwWindow> GetWindowByID(int id);
    std::shared_ptr<DxwWindow> GetCurrentWindow();
    void RegisterSolidBrush2D(const std::string& name, ComPtr<ID2D1Brush> brush);
    ComPtr<ID2D1Brush> GetSolidBrush2D(const std::string& name);

    const char* GetVertexShaderSource();
    const char* GetPixelShaderSource();

private:
   DxwSharedContext();
   ~DxwSharedContext();

    DxwSharedContext(const DxwSharedContext&) = delete;
    DxwSharedContext& operator=(const DxwSharedContext&) = delete;

    std::unordered_map<int, std::shared_ptr<DxwWindow>> windows;
    std::unordered_map<std::string, ComPtr<ID2D1Brush>> brushes2d;
    int targetId{ -1 };
};

}