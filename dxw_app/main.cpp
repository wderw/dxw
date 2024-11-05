#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <tchar.h>

HINSTANCE hInst;
HWND hWndMain;
HINSTANCE hDLL;
HWND g_hDrawingPanel = nullptr;

typedef int(__stdcall*  DXW_InitWindowFunc)(HWND);
typedef void(__stdcall* DXW_SetTargetWindowFunc)(int);
typedef void(__stdcall* DXW_D3D_ClearFunc)();
typedef void(__stdcall* DXW_D2D_BeginDrawFunc)();
typedef void(__stdcall* DXW_D2D_EndDrawFunc)();
typedef void(__stdcall* DXW_D2D_ClearFunc)();
typedef void(__stdcall* DXW_PresentFunc)(int);
typedef bool(__stdcall* DXW_IsInitializedFunc)();
typedef void(__stdcall* DXW_RunThreadedTestFunc)();
typedef void(__stdcall* DXW_DemoNRTFunc)();
typedef void(__stdcall* DXW_ResizeWindowFunc)(unsigned int, unsigned int);
typedef void(__stdcall* DXW_ReleaseDxwWindowsFunc)();

DXW_SetTargetWindowFunc         DXW_SetTargetWindow         = nullptr;
DXW_InitWindowFunc              DXW_InitWindow              = nullptr;
DXW_D3D_ClearFunc               DXW_D3D_Clear               = nullptr;
DXW_D2D_BeginDrawFunc           DXW_D2D_BeginDraw           = nullptr;
DXW_D2D_EndDrawFunc             DXW_D2D_EndDraw             = nullptr;
DXW_D2D_ClearFunc               DXW_D2D_Clear               = nullptr;
DXW_PresentFunc                 DXW_Present                 = nullptr;
DXW_IsInitializedFunc           DXW_IsInitialized           = nullptr;
DXW_RunThreadedTestFunc         DXW_RunThreadedTest         = nullptr;
DXW_DemoNRTFunc                 DXW_DemoNRT                 = nullptr;
DXW_ResizeWindowFunc            DXW_ResizeWindow            = nullptr;
DXW_ReleaseDxwWindowsFunc       DXW_ReleaseDxwWindows       = nullptr;

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
            _stprintf_s(errorMsg, _T("DXW_InitWindow GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_D3D_Clear = (DXW_D3D_ClearFunc)GetProcAddress(hDLL, "DXW_D3D_Clear");
        if (!DXW_D3D_Clear)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_D3D_Clear GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_D2D_Clear = (DXW_D2D_ClearFunc)GetProcAddress(hDLL, "DXW_D2D_Clear");
        if (!DXW_D2D_Clear)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_D2D_Clear GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_Present = (DXW_PresentFunc)GetProcAddress(hDLL, "DXW_Present");
        if (!DXW_Present)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_Present GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_D2D_BeginDraw = (DXW_D2D_BeginDrawFunc)GetProcAddress(hDLL, "DXW_D2D_BeginDraw");
        if (!DXW_D2D_BeginDraw)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_D2D_BeginDraw GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_D2D_EndDraw = (DXW_D2D_EndDrawFunc)GetProcAddress(hDLL, "DXW_D2D_EndDraw");
        if (!DXW_D2D_EndDraw)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_D2D_EndDraw GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_SetTargetWindow = (DXW_SetTargetWindowFunc)GetProcAddress(hDLL, "DXW_SetTargetWindow");
        if (!DXW_SetTargetWindow)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_SetTargetWindow GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_IsInitialized = (DXW_IsInitializedFunc)GetProcAddress(hDLL, "DXW_IsInitialized");
        if (!DXW_IsInitialized)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_IsInitialized GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_RunThreadedTest = (DXW_RunThreadedTestFunc)GetProcAddress(hDLL, "DXW_RunThreadedTest");
        if (!DXW_RunThreadedTest)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_RunThreadedTest GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_ResizeWindow = (DXW_ResizeWindowFunc)GetProcAddress(hDLL, "DXW_ResizeWindow");
        if (!DXW_ResizeWindow)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_ResizeWindow GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_DemoNRT = (DXW_DemoNRTFunc)GetProcAddress(hDLL, "DXW_DemoNRT");
        if (!DXW_DemoNRT)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_DemoNRT GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
        }

        DXW_ReleaseDxwWindows = (DXW_ReleaseDxwWindowsFunc)GetProcAddress(hDLL, "DXW_ReleaseDxwWindows");
        if (!DXW_ReleaseDxwWindows)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[256];
            _stprintf_s(errorMsg, _T("DXW_ReleaseDxwWindows GetProcAddress failed. Error code: %lu"), error);
            MessageBox(nullptr, errorMsg, _T("Error"), MB_OK);
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
    case WM_SIZE:
    {
        if (DXW_IsInitialized() == true)
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            DXW_ResizeWindow(width, height);

            DXW_DemoNRT();
        }
        return 0;
    }
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
    case WM_DESTROY:
        DXW_ReleaseDxwWindows();
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
    //dont redraw contents on resize
    //wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"DXWWindowClass";
    RegisterClassEx(&wcex);

    hWndMain = CreateWindow(L"DXWWindowClass", L"DirectX Wrapper test", WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, nullptr, nullptr, hInstance, nullptr);
    if (!hWndMain) return -1;

    SetupConsole();

    if (!LoadWrapperDll())
    {
        std::cerr << "Could not load wrapper dll!" << std::endl;
        return -1;
    }

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    //int id = DXW_InitWindow(g_hDrawingPanel);
    int id = DXW_InitWindow(hWndMain);
    std::cout << "Window allocated id was: " << id << std::endl;

    DXW_SetTargetWindow(id); // redundant but fine - target window is always the last added window
    DXW_RunThreadedTest();
    //DXW_DemoNRT();

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ReleaseWrapperDll();
    return (int)msg.wParam;
}
