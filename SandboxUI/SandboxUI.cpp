// SandboxUI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SandboxUI.h"
/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/
#include "01-ui/ui.h"
#include "00-app/top_config.h"
#include "01-ui/ui_logbox.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

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
    LoadStringW(hInstance, IDC_SANDBOXUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SANDBOXUI));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SANDBOXUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SANDBOXUI);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       0 /*WINDOWS_START_X*/, 0 /*WINDOWS_START_Y*/, WINDOWS_WIDTH+12, WINDOWS_HEIGHT+5, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
    int car_bottom_y = UI_GROUND_Y;
    const RECT rec_car_region = {
        UI_ANIMATION_X + UI_CAR_X - 10,
        car_bottom_y - UI_CAR_H - 5,
        UI_ANIMATION_X + UI_CAR_X + UI_CAR_W + 5,
        car_bottom_y + 5
    };


    // label redraw region
    const RECT rec_labels_region = {
        UI_LABELBOX_X-2,
        UI_LABELBOX_Y-2,
        UI_LABELBOX_X + UI_LABELBOX_W + UI_GAP + UI_LABELBOX2_W+2, 
        UI_LABELBOX_Y + UI_LABELBOX_H + 2
    };


    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections
            ui_callback_type_radio(wmId);
            ui_callback_type_lock_step1(hWnd, wmId);

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
    case WM_DRAWITEM: { //wParam:the control's ID (like BUTTON_ID_UP); lParam:a pointer to a DRAWITEMSTRUCT struct
        ui_callback_type_lock_step2((LPDRAWITEMSTRUCT)lParam);
        ui_callback_type_led((LPDRAWITEMSTRUCT)lParam);
        return TRUE; // always return TRUE
    }
    case WM_PAINT:
        {
        extern void ui39_draw_invalidate_rect_area_debug(HDC hdc, CONST RECT * lpRect);
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            //ui03_draw_all(hdc);
            if(1){ // double 
                // get entire client area - full window
                RECT rc;
                GetClientRect(hWnd, &rc);
                int width = rc.right - rc.left;
                int height = rc.bottom - rc.top;

                // create a compatible memory device context
                HDC memDC = CreateCompatibleDC(hdc);                            // buffer
                HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height); // bit map
                HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);    // use this bitmap
                FillRect(memDC, &rc, (HBRUSH)(COLOR_WINDOW + 1));               // fill background in memory device context

                // draw everything into the memory device context
                ui03_draw_all(memDC);
                // draw debug rectangles into memory device context
                ui39_draw_invalidate_rect_area_debug(memDC, &rec_car_region); // DEBUG TOOL
                ui39_draw_invalidate_rect_area_debug(memDC, &rec_labels_region); // DEBUG TOOL

                BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY); // blit the memory device context to the screen
                SelectObject(memDC, oldBitmap);                         // restore old bit map
                DeleteObject(memBitmap);                                // delete memBitmap
                DeleteDC(memDC);                                        // delete memory device context
            }
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_CREATE: //************
        ui1_init_widgets(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_TIMER: // All timers go here.
        switch (wParam) {
        case IDT_TIMER_UI:
            //ui_internal_printf("Invalidate %d, %d, %d, %d.", rec_car_region.left, rec_car_region.top, rec_car_region.right, rec_car_region.bottom);
            InvalidateRect(hWnd, &rec_car_region, TRUE);       // True=erase background,
            //InvalidateRect(hWnd, &rec_labels_region, FALSE); // only invalidate the custom label box area
            break;
        case IDT_TIMER_SIMULATOR:
            //app_run();
            break;
        }
        break;

    case WM_ERASEBKGND:
        return 1; // skip the background erase

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
