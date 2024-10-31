#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <tchar.h>

HINSTANCE hInst;
HWND hWndMain;
HINSTANCE hDLL;
HWND g_hDrawingPanel = nullptr;

typedef void(__stdcall* DXW_InitWindowFunc)(HWND);
typedef void(__stdcall* DXW_D3DDeviceContext_ClearFunc)();
DXW_InitWindowFunc DXW_InitWindow = nullptr;
DXW_D3DDeviceContext_ClearFunc DXW_D3DDeviceContext_Clear = nullptr;

const std::wstring libraryName = L"dxw.dll";
void CreateDrawingPanel(HWND parentHwnd);

void SetupConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
}

void CreateDrawingPanel(HWND parentHwnd)
{
    // Create a child window (panel) for drawing
    g_hDrawingPanel = CreateWindowEx(
        0, L"STATIC", L"", WS_CHILD | WS_VISIBLE,
        80, 80, 800, 600, parentHwnd, nullptr, nullptr, nullptr
    );
}

bool LoadWrapperDll()
{
    hDLL = LoadLibrary(libraryName.c_str());
    if (!hDLL)
    {
        std::wstring errorString = L"Failed to load:" + libraryName;
        MessageBox(NULL, errorString.c_str(), L"Error", MB_OK);
        return false;
    }

    if (hDLL)
    {
        DXW_InitWindow = (DXW_InitWindowFunc)GetProcAddress(hDLL, "DXW_InitWindow");
        if (!DXW_InitWindow)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }
        else
        {
            MessageBox(nullptr, L"DXW_InitWindow Loaded Correctly!", _T("Error"), MB_OK);
        }

        DXW_D3DDeviceContext_Clear = (DXW_D3DDeviceContext_ClearFunc)GetProcAddress(hDLL, "DXW_D3DDeviceContext_Clear");
        if (!DXW_D3DDeviceContext_Clear)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }
        else
        {
            MessageBox(nullptr, L"DXW_D3DDeviceContext_Clear Loaded Correctly!", _T("Error"), MB_OK);
        }

    }

    std::cout << "Wrapper loaded successfully!" << std::endl;
    return true;
}

void ReleaseWrapperDll()
{
    if (hDLL)
    {
        FreeLibrary(hDLL);
        hDLL = nullptr;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        CreateDrawingPanel(hWnd);
        CreateWindow(
            L"BUTTON",
            L"Start Drawing",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10,         // x position
            10,         // y position
            150,        // Button width
            60,         // Button height
            hWnd,       // Parent window
            (HMENU)1,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            nullptr);
        return 0;
    }
    case WM_MOVE:
    case WM_SIZE:
    case WM_MOUSEMOVE:
    case WM_SHOWWINDOW:
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"DXWWindowClass";
    RegisterClassEx(&wcex);

    hWndMain = CreateWindow(L"DXWWindowClass", L"DirectX Wrapper test", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, nullptr, nullptr, hInstance, nullptr);
    if (!hWndMain) return -1;

    SetupConsole();

    if (!LoadWrapperDll())
    {
        std::cerr << "Could not load wrapper dll!" << std::endl;
        return -1;
    }

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    DXW_InitWindow(g_hDrawingPanel);
    DXW_D3DDeviceContext_Clear();

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ReleaseWrapperDll();
    return (int)msg.wParam;
}
