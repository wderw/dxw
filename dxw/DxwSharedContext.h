#pragma once

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
        windows.emplace_back(window);
    }

private:
    DxwSharedContext()
    {
        LOG_INFO("Shared context created");
    }

    ~DxwSharedContext()
    {
        LOG_INFO("Shared context destroyed");
    }

    DxwSharedContext(const DxwSharedContext&) = delete;
    DxwSharedContext& operator=(const DxwSharedContext&) = delete;

    std::vector<std::shared_ptr<DxwWindow>> windows{};
};

}