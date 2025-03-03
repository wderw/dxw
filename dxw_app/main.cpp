#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <tchar.h>

HINSTANCE hInst;
HWND hWndMain;
HINSTANCE hDLL;
HWND g_hDrawingPanel = nullptr;
HWND g_hDrawingPanel2 = nullptr;


// DELEGATES
// 3d
typedef void(__stdcall* DXW_D3D_ClearFunc)(float, float, float, float);

// 2d
typedef void(__stdcall* DXW_D2D_BeginDrawFunc)();
typedef void(__stdcall* DXW_D2D_EndDrawFunc)();
typedef void(__stdcall* DXW_D2D_ClearFunc)();
typedef void(__stdcall* DXW_D2D_SetScaleFunc)(float, float);
typedef void(__stdcall* DXW_D2D_SetRotationFunc)(float);
typedef void(__stdcall* DXW_D2D_SetTranslationFunc)(float, float);
typedef void(__stdcall* DXW_D2D_RecalculateTransformMatrixFunc)();
typedef void(__stdcall* DXW_D2D_ResetTransformMatrixFunc)();
typedef void(__stdcall* DXW_D2D_DrawTextFunc)(const WCHAR*, float, float, float, float);
typedef void(__stdcall* DXW_D2D_FillRoundedRectangleFunc)(float, float, float, float, float, float);
typedef void(__stdcall* DXW_D2D_DrawRoundedRectangleFunc)(float, float, float, float, float, float);
typedef void(__stdcall* DXW_D2D_FillRectangleFunc)(float, float, float, float);
typedef void(__stdcall* DXW_D2D_DrawRectangleFunc)(float, float, float, float);
typedef void(__stdcall* DXW_D2D_FillEllipseFunc)(float, float, float, float);
typedef void(__stdcall* DXW_D2D_DrawEllipseFunc)(float, float, float, float, float);

// general
typedef int(__stdcall*  DXW_InitWindowFunc)(HWND);
typedef void(__stdcall* DXW_SetTargetWindowFunc)(int);
typedef void(__stdcall* DXW_PresentFunc)(int);
typedef bool(__stdcall* DXW_IsInitializedFunc)();
typedef void(__stdcall* DXW_ResizeWindowFunc)(unsigned int, unsigned int);
typedef void(__stdcall* DXW_ReleaseDxwResourcesFunc)();

// demos
typedef void(__stdcall* DXW_DemoLinesFunc)(int);
typedef void(__stdcall* DXW_Demo3DFunc)();
typedef void(__stdcall* DXW_DemoNRTFunc)(float);
// DELEGATES END

// POINTERS
// 3d
DXW_D3D_ClearFunc DXW_D3D_Clear = nullptr;

// 2d
DXW_D2D_BeginDrawFunc DXW_D2D_BeginDraw = nullptr;
DXW_D2D_EndDrawFunc DXW_D2D_EndDraw = nullptr;
DXW_D2D_ClearFunc DXW_D2D_Clear = nullptr;
DXW_D2D_SetScaleFunc DXW_D2D_SetScale = nullptr;
DXW_D2D_SetRotationFunc DXW_D2D_SetRotation = nullptr;
DXW_D2D_SetTranslationFunc DXW_D2D_SetTranslation = nullptr;
DXW_D2D_RecalculateTransformMatrixFunc DXW_D2D_RecalculateTransformMatrix = nullptr;
DXW_D2D_ResetTransformMatrixFunc DXW_D2D_ResetTransformMatrix = nullptr;
DXW_D2D_DrawTextFunc DXW_D2D_DrawText = nullptr;
DXW_D2D_FillRoundedRectangleFunc DXW_D2D_FillRoundedRectangle = nullptr;
DXW_D2D_DrawRoundedRectangleFunc DXW_D2D_DrawRoundedRectangle = nullptr;
DXW_D2D_FillRectangleFunc DXW_D2D_FillRectangle = nullptr;
DXW_D2D_DrawRectangleFunc DXW_D2D_DrawRectangle = nullptr;
DXW_D2D_FillEllipseFunc DXW_D2D_FillEllipse = nullptr;
DXW_D2D_DrawEllipseFunc DXW_D2D_DrawEllipse = nullptr;

// general
DXW_SetTargetWindowFunc     DXW_SetTargetWindow     = nullptr;
DXW_InitWindowFunc          DXW_InitWindow          = nullptr;
DXW_PresentFunc             DXW_Present             = nullptr;
DXW_IsInitializedFunc       DXW_IsInitialized       = nullptr;
DXW_ResizeWindowFunc        DXW_ResizeWindow        = nullptr;
DXW_ReleaseDxwResourcesFunc DXW_ReleaseDxwResources = nullptr;

// demos
DXW_DemoNRTFunc             DXW_DemoNRT             = nullptr;
DXW_DemoLinesFunc           DXW_DemoLines           = nullptr;
DXW_Demo3DFunc              DXW_Demo3D              = nullptr;
// POINTERS END

const std::wstring libraryName = L"dxw.dll";
void CreateDrawingPanels(HWND parentHwnd);

void SetupConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
}

void CreateDrawingPanels(HWND parentHwnd)
{
    g_hDrawingPanel = CreateWindowEx(
        0, L"STATIC", L"", WS_CHILD | WS_VISIBLE,
        10, 10, 800, 300, parentHwnd, nullptr, nullptr, nullptr
    );

    g_hDrawingPanel2 = CreateWindowEx(
        0, L"STATIC", L"", WS_CHILD | WS_VISIBLE,
        10, 320, 800, 370, parentHwnd, nullptr, nullptr, nullptr
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
        // 3d
        DXW_D3D_Clear = (DXW_D3D_ClearFunc)GetProcAddress(hDLL, "DXW_D3D_Clear");

        // 2d
        DXW_D2D_Clear = (DXW_D2D_ClearFunc)GetProcAddress(hDLL, "DXW_D2D_Clear");
        DXW_D2D_BeginDraw = (DXW_D2D_BeginDrawFunc)GetProcAddress(hDLL, "DXW_D2D_BeginDraw");
        DXW_D2D_EndDraw = (DXW_D2D_EndDrawFunc)GetProcAddress(hDLL, "DXW_D2D_EndDraw");
        DXW_D2D_SetScale = (DXW_D2D_SetScaleFunc)GetProcAddress(hDLL, "DXW_D2D_SetScale");
        DXW_D2D_SetRotation = (DXW_D2D_SetRotationFunc)GetProcAddress(hDLL, "DXW_D2D_SetRotation");
        DXW_D2D_SetTranslation = (DXW_D2D_SetTranslationFunc)GetProcAddress(hDLL, "DXW_D2D_SetTranslation");
        DXW_D2D_RecalculateTransformMatrix = (DXW_D2D_RecalculateTransformMatrixFunc)GetProcAddress(hDLL, "DXW_D2D_RecalculateTransformMatrix");
        DXW_D2D_ResetTransformMatrix = (DXW_D2D_ResetTransformMatrixFunc)GetProcAddress(hDLL, "DXW_D2D_ResetTransformMatrix");
        DXW_D2D_DrawText = (DXW_D2D_DrawTextFunc)GetProcAddress(hDLL, "DXW_D2D_DrawText");
        DXW_D2D_FillRoundedRectangle = (DXW_D2D_FillRoundedRectangleFunc)GetProcAddress(hDLL, "DXW_D2D_FillRoundedRectangle");
        DXW_D2D_DrawRoundedRectangle = (DXW_D2D_DrawRoundedRectangleFunc)GetProcAddress(hDLL, "DXW_D2D_DrawRoundedRectangle");
        DXW_D2D_FillRectangle = (DXW_D2D_FillRectangleFunc)GetProcAddress(hDLL, "DXW_D2D_FillRectangle");
        DXW_D2D_DrawRectangle = (DXW_D2D_DrawRectangleFunc)GetProcAddress(hDLL, "DXW_D2D_DrawRectangle");
        DXW_D2D_FillEllipse = (DXW_D2D_FillEllipseFunc)GetProcAddress(hDLL, "DXW_D2D_FillEllipse");
        DXW_D2D_DrawEllipse = (DXW_D2D_DrawEllipseFunc)GetProcAddress(hDLL, "DXW_D2D_DrawEllipse");

        // general
        DXW_InitWindow = (DXW_InitWindowFunc)GetProcAddress(hDLL, "DXW_InitWindow");
        DXW_Present = (DXW_PresentFunc)GetProcAddress(hDLL, "DXW_Present");
        DXW_SetTargetWindow = (DXW_SetTargetWindowFunc)GetProcAddress(hDLL, "DXW_SetTargetWindow");
        DXW_IsInitialized = (DXW_IsInitializedFunc)GetProcAddress(hDLL, "DXW_IsInitialized");
        DXW_ResizeWindow = (DXW_ResizeWindowFunc)GetProcAddress(hDLL, "DXW_ResizeWindow");
        DXW_ReleaseDxwResources = (DXW_ReleaseDxwResourcesFunc)GetProcAddress(hDLL, "DXW_ReleaseDxwResources");

        // demos
        DXW_DemoLines = (DXW_DemoLinesFunc)GetProcAddress(hDLL, "DXW_DemoLines");
        DXW_Demo3D = (DXW_Demo3DFunc)GetProcAddress(hDLL, "DXW_Demo3D");
        DXW_DemoNRT = (DXW_DemoNRTFunc)GetProcAddress(hDLL, "DXW_DemoNRT");
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

            static float fi = 0;
            fi += 1.0f;

            DXW_DemoNRT(fi);
            DXW_Present(1);
        }
        return 0;
    }
    case WM_CREATE:
    {
        CreateDrawingPanels(hWnd);
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
        DXW_ReleaseDxwResources();
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

    int id = DXW_InitWindow(g_hDrawingPanel);
    int id2 = DXW_InitWindow(g_hDrawingPanel2);
    std::cout << "Window allocated id was: " << id << std::endl;

    DXW_SetTargetWindow(id);
    DXW_DemoNRT(1);

    DXW_D2D_BeginDraw();

//    DXW_D2D_DrawRoundedRectangle(20.0f, 40.0f, 150.0f, 150.0f, 10.0f, 10.0f);
    //DXW_D2D_FillRectangle(10.0f, 10.0f, 100.0f, 100.0f);
    //DXW_D2D_DrawRectangle(40.0f, 40.0f, 120.0f, 120.0f);

    //DXW_D2D_FillEllipse(50, 50, 55.0f, 55.0f);
    //DXW_D2D_DrawEllipse(150, 150, 25.0f, 55.0f, 2.0f);



    DXW_D2D_SetScale(1.0f, 2.2f);
    DXW_D2D_SetRotation(35.0f);
    DXW_D2D_SetTranslation(100.0f, 50.0f);
    DXW_D2D_RecalculateTransformMatrix();
    //DXW_D2D_FillRoundedRectangle(10.0f, 10.0f, 100.0f, 100.0f, 15.0f, 10.0f);


    DXW_D2D_DrawText(L"AAAAAAAAA���AIEOU hehe", 0, 0, 250, 250);

    DXW_D2D_ResetTransformMatrix();
    DXW_D2D_SetRotation(-180.0f);
    DXW_D2D_SetTranslation(300.0f, 200.0f);
    DXW_D2D_RecalculateTransformMatrix();

    DXW_D2D_DrawText(L"AAAAAAAAA���AIEOU hehe", 0, 0, 250, 250);

    DXW_D2D_EndDraw();

    DXW_Present(1);


    //DXW_DemoLines(1000000);

    DXW_SetTargetWindow(id2);
    //DXW_DemoLines(1000000);
    //DXW_Demo3D();

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ReleaseWrapperDll();
    return (int)msg.wParam;
}
