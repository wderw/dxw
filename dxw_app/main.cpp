#include <windows.h>
#include <cstdlib>
#include <iostream>

HINSTANCE hInst;
HWND hWndMain;
HINSTANCE hDLL;

const std::wstring libraryName = L"dxw.dll";

void SetupConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
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

    hWndMain = CreateWindow(L"DXWWindowClass", L"DirectX Wrapper test", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);
    if (!hWndMain) return -1;

    SetupConsole();

    if (!LoadWrapperDll())
    {
        std::cerr << "Could not load wrapper dll!" << std::endl;
        return -1;
    }

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ReleaseWrapperDll();
    return (int)msg.wParam;
}
