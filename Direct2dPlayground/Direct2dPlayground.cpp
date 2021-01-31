// Direct2dPlayground.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Direct2dPlayground.h"
#include <d2d1_1.h>
#include <wrl.h>
#include <d3d11.h>
#include "WICImageLoader.h"


#pragma comment (lib,"d2d1")
#pragma comment (lib,"d3d11")
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hwnd;

Microsoft::WRL::ComPtr<ID2D1Device> d2d1Device;
Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d1DeviceContext;

Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11DeviceContext;

Microsoft::WRL::ComPtr<ID2D1Factory1> d2d1Factory;
Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> renderTarget;
Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> lightSlateGrayBrush;
ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

Direct2dHelpers::WICImageLoader imageLoader;
Microsoft::WRL::ComPtr<ID2D1Bitmap> anime;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECT2DPLAYGROUND, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT2DPLAYGROUND));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT2DPLAYGROUND));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECT2DPLAYGROUND);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hwnd)
   {
      return FALSE;
   }

   HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2d1Factory.GetAddressOf());
   DX::ThrowIfFailed(hr);

   RECT rc;
   GetClientRect(hwnd, &rc);

   hr = d2d1Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), 
       renderTarget.GetAddressOf());
   DX::ThrowIfFailed(hr);

   D3D_FEATURE_LEVEL featureLevels[] =
   {
       D3D_FEATURE_LEVEL_11_1,
       D3D_FEATURE_LEVEL_11_0,
       D3D_FEATURE_LEVEL_10_1,
       D3D_FEATURE_LEVEL_10_0,
       D3D_FEATURE_LEVEL_9_3,
       D3D_FEATURE_LEVEL_9_2,
       D3D_FEATURE_LEVEL_9_1
   };

   hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, d3d11Device.GetAddressOf(), nullptr, d3d11DeviceContext.GetAddressOf());
   DX::ThrowIfFailed(hr);

   Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
   
   hr = d3d11Device->QueryInterface(dxgiDevice.GetAddressOf());
   DX::ThrowIfFailed(hr);
   
   hr = d2d1Factory->CreateDevice(dxgiDevice.Get(), d2d1Device.GetAddressOf());
   DX::ThrowIfFailed(hr);

   hr = d2d1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2d1DeviceContext.GetAddressOf());
   DX::ThrowIfFailed(hr);


   auto size = renderTarget->GetSize();
   anime = imageLoader.LoadBitmapFromFile(renderTarget.Get(), L"E:\\Source\\Direct2dPlayground\\pictures\\animeOpacityMap3.png", size.width, size.height);
   ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            renderTarget->BeginDraw();
            auto size = renderTarget->GetSize();
            auto bitmapSize = anime->GetSize();
            renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Crimson));
            renderTarget->DrawBitmap(anime.Get(), D2D1::RectF(0, 0, size.width, size.height), 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0,0,bitmapSize.width, bitmapSize.height));
            renderTarget->EndDraw();

            //HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            //EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
