// SandboxUI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SandboxUI.h"
/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/
#include "00-app/top.h"
#include "01-ui/button_id.h"

#include "01-ui/ui.h" //to use ui datatype
#include "01-ui/ui_animation.h"
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
   // #########################################
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       0 /*WINDOWS_START_X*/, 0 /*WINDOWS_START_Y*/, 
       WINDOWS_WIDTH+12, 
       WINDOWS_HEIGHT+5, 
       nullptr, nullptr, hInstance, nullptr);
   // #########################################
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
    // #########################################
    int car_height_mm = disp_stru.car1_height;
    // #########################################
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections
            // #########################################
            if (HIWORD(wParam) == BN_CLICKED) {
                ui_callback_type_click(wmId);
                ui_callback_type_radio(wmId);
                ui_callback_type_lock_step1(hWnd, wmId); //there're only 3 lock-type buttons
            }
            // #########################################

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
        //#########################################
        ui_callback_type_lock_step2((LPDRAWITEMSTRUCT)lParam);
        ui_callback_type_led((LPDRAWITEMSTRUCT)lParam);
        // #########################################
        return TRUE; // always return TRUE
    }
    case WM_PAINT:
        // #########################################
        ui3_doublebuffer_paint(hWnd, car_height_mm);
        // #########################################
        break;

    case WM_CREATE: 
        // #########################################
        ui2_init(hWnd);
        // #########################################
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_TIMER: // All timers go here.
        switch (wParam) {
        case IDT_TIMER_UI: {
            // #########################################
            // ui_internal_printf("Invalidate %d, %d, %d, %d.", rec_car_region.left, rec_car_region.top, rec_car_region.right, rec_car_region.bottom);
            ui4_test_loop_example(); //handling model changes
            ui_ani_invaldate_rec(hWnd, car_height_mm);
            // #########################################
        }

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

/********************************* end of file ********************************/
