/**
******************************************************************************
* @file    ui.cpp
* @author  ARTHUR
* @version V1
* @date    2025-07-31 16:30:04
* @brief   
*
*
******************************************************************************
* @attention
*  [1] This file is saved in Unicode format in order to hold emojis.
*
******************************************************************************
*/

/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/

#include "./../framework.h" //include file for standard system include files,
#include <stdint.h> //to use int32_t type
#include <windows.h>
#include <commctrl.h> // For SetWindowSubclass, DefSubclassProc
#pragma comment(lib, "comctl32.lib")//DefSubclassProc is not part of the core Windows API (windows.h), but part of the Common Controls Library; This library provides the actual implementation of functions declared in commctrl.h.


#include "ui.h"

/*******************************************************************************
******************************** Private typedef *******************************
*******************************************************************************/

typedef struct {
    int                 run_cnt; // how many times ui has refreshed
    int                 car_box_y;          // the y position of cab box bottom line.
    int                 door_opening_width; // 0..40 pix width
    uint32_t ts_at_start;   // recorded ts at starting up time by ui82_c_get_u_run_ms().
    int car_mode;           // 0=simplex, 1=master, 2=slave1

    int col_car; // left pix of this column
    int col_log;
    int col_btn;

    int label1_cnt;                     // how many times does label1 update
    char label1_buf[LABEL_STRING_SIZE]; // string buffer for lable1, to check if the content changes.

    HFONT font1; //global font for this app

    HBRUSH hBrushGreen;
    HBRUSH hBrushGray;
    bool greenMode;
    
    bool is_enable;
    bool is_up;
    bool is_down;


} UI_CONTROL_STRU;

/*******************************************************************************
******************************** Private define ********************************
*******************************************************************************/

/*******************************************************************************
********************************* Private macro ********************************
*******************************************************************************/
typedef struct {
    const wchar_t* text;
    int id;    
    int is_clicked; //0=not, 1=clicked, used when this button is also used as a led

} BUTTON_STRU;

#define FILL_BUTTON_ID(name)          { #name, name, 0 }


/*******************************************************************************
******************************* Private variables ******************************
*******************************************************************************/
UI_CONTROL_STRU _ui_control_stru;

BUTTON_STRU button_group1[] = {
    { L"UP🔺", BUTTON_ID_UP, 0 },
    { L"DOWN🔻", BUTTON_ID_DOWN, 0 },
};


HWND hButtonUp, hButtonDown, hButtonLight;
bool up = false;
bool down = false;
bool light = false;

/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/

void ui10_apply_font_to_control(HWND hwndTarget, int pt);
void ui11_init_label(HWND hwnd);
void ui12_init_click_button(HWND hwnd, int x, int y);
void ui13_init_radio_group(HWND hwnd);
void ui30_draw_custom_button_led(LPDRAWITEMSTRUCT lpDrawItem, const wchar_t* text, bool state);
LRESULT CALLBACK ui41_ButtonSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
void ui40_create_press_buttons(HWND hwnd);



        

/*******************************************************************************
******************************* Private functions ******************************
*******************************************************************************/


/*******************************************************************************
******************************* Public Variables *******************************
*******************************************************************************/

/*******************************************************************************
******************************* Public functions *******************************
*******************************************************************************/




/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  hwndTarget, the window you want to apply the font, any window control
 * @param  pt, the font size, usually 9 or 12 in win11
 * @return xxxx
 *******************************************************************************/
void ui10_apply_font_to_control(HWND hwndTarget, int pt) {
    HFONT hf = CreateFont(
        -MulDiv(pt, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72), // height for 9pt
        0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI");

     if (hf && hwndTarget) {
        SendMessage(hwndTarget, WM_SETFONT, (WPARAM)hf, TRUE);
    }
}
        
        

/*******************************************************************************
 * @brief  called by SandboxUI.cpp/WM_CREATE event
 * @param  hwnd, parent windows
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui1_init_widgets(HWND hwnd) { // Labels
    ui11_init_label(hwnd);
    ui12_init_click_button(hwnd, 20, 120);
    ui40_create_press_buttons(hwnd);
}
        


/*******************************************************************************
 * @brief  called by main.cpp WM_COMMAND event.
 * @param  id : the button id 
 * @param  hwnd: the parent wnd
 * @return xxxx
 *******************************************************************************/
void ui2_button_action_callback(HWND hwnd, int id) {
    switch (id) {
    case BUTTON_ID_UP:
        OutputDebugStringA("UP pressed\n");
        _ui_control_stru.is_up = !(_ui_control_stru.is_up);
        InvalidateRect(GetDlgItem(hwnd, id), NULL, TRUE); // Force redraw

        break;
    case BUTTON_ID_DOWN:
        OutputDebugStringA("DOWN pressed\n");

        _ui_control_stru.is_down = !(_ui_control_stru.is_down);
        InvalidateRect(GetDlgItem(hwnd, id), NULL, TRUE); // force repaint
        break;
    case BUTTON_ID_ENABLE:
        OutputDebugStringA("ENABLE pressed\n");

        _ui_control_stru.is_enable = !(_ui_control_stru.is_enable);
        InvalidateRect(GetDlgItem(hwnd, id), NULL, TRUE); // Force redraw
        break;
        break;
    }
}
        

/*******************************************************************************
 * @brief  called by WM_DRAWITEM, originally triggered by InvalidateRect()
 * @param  lpDrawItem: which button to redraw
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui3_button_drawitem_callback(LPDRAWITEMSTRUCT lpDrawItem) {
    int id = lpDrawItem->CtlID;
    switch (id) {
    case BUTTON_ID_UP:
        ui30_draw_custom_button_led(lpDrawItem, L"UP", _ui_control_stru.is_up);

        break;
    case BUTTON_ID_DOWN:
        ui30_draw_custom_button_led(lpDrawItem, L"DOWN", _ui_control_stru.is_down);

        break;
    case BUTTON_ID_ENABLE:
        ui30_draw_custom_button_led(lpDrawItem, L"ENABLE🔼", _ui_control_stru.is_enable);

        break;
    default:break;
    }
}
        

/*******************************************************************************
 * @brief  originally called by WM_DRAWITEM event in main.cpp
 * @param  lpDrawItem: it contains button id, and other display information
 * @param  state: 0=defalt, 1=on, 2.. other states
 * @return xxxx
 *******************************************************************************/
void ui30_draw_custom_button_led(LPDRAWITEMSTRUCT lpDrawItem, const wchar_t* text, bool state) {
    HDC hdc = lpDrawItem->hDC;
    RECT rc = lpDrawItem->rcItem;

    // Background
    HBRUSH bgBrush = CreateSolidBrush(state ? RGB(0, 0, 0) : RGB(240, 240, 240));
    FillRect(hdc, &rc, bgBrush);
    DeleteObject(bgBrush);

    // Text
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, state ? RGB(255, 0, 0) : RGB(0, 0, 0));
    DrawText(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Optional: draw border
    DrawEdge(hdc, &rc, EDGE_RAISED, BF_RECT);
}
        

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  hwnd, parent windows
 * @return xxxx
 *******************************************************************************/
void ui11_init_label(HWND hwnd) {
    // create label, 12pt
    HWND hStaticLabel = CreateWindow(L"STATIC", L"  -- INSPECTION -- ", WS_CHILD | WS_VISIBLE,
        20, 40, 300, 40, hwnd, NULL, NULL, NULL);

    ui10_apply_font_to_control(hStaticLabel, UI_FONT_12PT);

    // create label, 9pt
    HWND hStaticLabel2 = CreateWindow(L"STATIC", L"  -- INSPECTION -- ", WS_CHILD | WS_VISIBLE,
        20, 80, 300, 40, hwnd, NULL, NULL, NULL);
    ui10_apply_font_to_control(hStaticLabel2, UI_FONT_9PT);
}
        

/*******************************************************************************
 * @brief  init a click-type button 
 * @param  hwnd, parent windows
 *         x, y: the starting left top pixel
 * @return xxxx
 *******************************************************************************/
void ui12_init_click_button(HWND hwnd, int x, int y) {

    HWND b1 = CreateWindow(L"BUTTON", L"UP", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
        x, y, 80, 30, hwnd, (HMENU)BUTTON_ID_UP, NULL, NULL);
    ui10_apply_font_to_control(b1, UI_FONT_9PT);


    HWND b2 = CreateWindow(L"BUTTON", L"DOWN", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
        x, y + 30, 80, 30, hwnd, (HMENU)BUTTON_ID_DOWN, NULL, NULL);
    ui10_apply_font_to_control(b2, UI_FONT_9PT);


    HWND b3 = CreateWindow(L"BUTTON", L"ENABLE", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
        x, y+60, 80, 30, hwnd, (HMENU)BUTTON_ID_ENABLE, NULL, NULL);
    ui10_apply_font_to_control(b3, UI_FONT_9PT);
}
       


/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  hwnd, parent windows
 * @return xxxx
 *******************************************************************************/
void ui13_init_radio_group(HWND hwnd) { // Radio group 1 (ON/OFF)
    CreateWindow(L"BUTTON", L"ON", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
        130, 40, 50, 20, hwnd, (HMENU)10, NULL, NULL);

    CreateWindow(L"BUTTON", L"OFF", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        190, 40, 50, 20, hwnd, (HMENU)11, NULL, NULL);
}
        

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui40_create_press_buttons(HWND hwnd) {
    hButtonUp = CreateWindow(L"BUTTON", L"Press Up", WS_VISIBLE | WS_CHILD,
        400, 20, 80, 30, hwnd, NULL, NULL, NULL);
    ui10_apply_font_to_control(hButtonUp, UI_FONT_9PT);

    hButtonDown = CreateWindow(L"BUTTON", L"Press Down", WS_VISIBLE | WS_CHILD,
        400, 60, 80, 30, hwnd, NULL, NULL, NULL);
    ui10_apply_font_to_control(hButtonDown, UI_FONT_9PT);

    hButtonLight = CreateWindow(L"BUTTON", L"Press Light", WS_VISIBLE | WS_CHILD,
        400, 100, 80, 30, hwnd, NULL, NULL, NULL);
    ui10_apply_font_to_control(hButtonLight, UI_FONT_9PT);

    // Subclass all buttons
    SetWindowSubclass(hButtonUp, ui41_ButtonSubclassProc, 1, 0);
    SetWindowSubclass(hButtonDown, ui41_ButtonSubclassProc, 2, 0);
    SetWindowSubclass(hButtonLight, ui41_ButtonSubclassProc, 3, 0);
}


/*******************************************************************************
 * @brief  Subclass procedure
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
LRESULT CALLBACK ui41_ButtonSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
    UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    switch (uMsg) {
    case WM_LBUTTONDOWN:
        if (hwnd == hButtonUp) {
            up = true;
            OutputDebugStringA("up=1.\n");
        }
           
        else if (hwnd == hButtonDown) {
            down = true;
            OutputDebugStringA("down=1.\n");

        }
        else if (hwnd == hButtonLight) {
            light = true;
            OutputDebugStringA("light=1.\n");
        }
        break;

    case WM_LBUTTONUP:
        if (hwnd == hButtonUp) {
            up = false;
            OutputDebugStringA("up=0.\n");
        }
        else if (hwnd == hButtonDown) {
            down = false;
            OutputDebugStringA("down=0.\n");
        }
        else if (hwnd == hButtonLight) {
            light = false;
            OutputDebugStringA("light=0.\n");
        }
        break;

    case WM_NCDESTROY:
        // Clean up subclass when control is destroyed
        RemoveWindowSubclass(hwnd, ui41_ButtonSubclassProc, uIdSubclass);
        break;
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam); // Forward to original
}

/********************************* end of file ********************************/

