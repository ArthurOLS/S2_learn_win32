/**
******************************************************************************
* @file    ui_lowlevel.cpp
* @author  ARTHUR
* @version V1
* @date    2025-08-07 11:02:12
* @brief   low level function for ui module, elevator irrelavent
*
*
******************************************************************************
* @attention
*           This file has functions that are all independent from each other
*
******************************************************************************
*/

/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/

#include <Windows.h>
#include <stdio.h>
#include "ui_lowlevel.h"
#include <commctrl.h> // For SetWindowSubclass, DefSubclassProc
#include "ui.h"
#include "ui_logbox.h"

/*******************************************************************************
******************************** Private typedef *******************************
*******************************************************************************/

/*******************************************************************************
******************************** Private define ********************************
*******************************************************************************/


/*******************************************************************************
********************************* Private macro ********************************
*******************************************************************************/

/*******************************************************************************
******************************* Private variables ******************************
*******************************************************************************/
HFONT g_hfont9 = NULL; // Nicer font to replace GDI default font
HFONT g_hfont12 = NULL; // Nicer font to replace GDI default font

/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/

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
    int height; //negative value, -12=~9pt in my computer
    height = -MulDiv(pt, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);
    HFONT hf = CreateFont(
        height, 
        0, 0, 0, FW_NORMAL, // width, escapement, orientation, weight
        FALSE, FALSE, FALSE, // italic, underline, strikeout
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

void ui_create_font9() {
    int height; // negative value, -12=~9pt in my computer
    height = -MulDiv(9, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);
    g_hfont9 = CreateFont(
        height,
        0, 0, 0, FW_NORMAL,  // width, escapement, orientation, weight
        FALSE, FALSE, FALSE, // italic, underline, strikeout
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI");
}


/*******************************************************************************
 * @brief  creat 9pt lable in a location, (x,y,w,h), solid border
 * @param  hwnd, parent windows
 * @return xxxx
 *******************************************************************************/
//Unicode label
void ui11_create_label(HWND hwnd, const wchar_t* text, int x, int y, int w, int h) {
    // create label, 12pt
    HWND hStaticLabel = CreateWindow(L"STATIC", text, WS_CHILD | WS_BORDER | WS_VISIBLE,
        x, y, w, h, hwnd, NULL, NULL, NULL);

    ui10_apply_font_to_control(hStaticLabel, UI_FONT_9PT);
}
//Ascii label
void ui11_create_label(HWND hwnd, const char* text, int x, int y, int w, int h) {
    // create label, 12pt
    HWND hStaticLabel = CreateWindowA("STATIC", text, WS_CHILD | WS_VISIBLE,
        x, y, w, h, hwnd, NULL, NULL, NULL);

    ui10_apply_font_to_control(hStaticLabel, UI_FONT_9PT);
}

/*******************************************************************************
 * @brief  creat 9pt lable in a location, (x,y,w,h), solid border
 * @param  hwnd, parent windows
 * @return xxxx
 *******************************************************************************/
void ui11_create_label_no_border(HWND hwnd, const wchar_t* text, int x, int y, int w, int h) {
    // create label, 12pt
    HWND hStaticLabel = CreateWindow(L"STATIC", text, WS_CHILD | WS_VISIBLE,
        x, y, w, h, hwnd, NULL, NULL, NULL);

    ui10_apply_font_to_control(hStaticLabel, UI_FONT_9PT);
}



/*******************************************************************************
 * @brief  create radio buttons
 * @param  hwnd, parent windows
 * @return xxxx
 *******************************************************************************/
void ui_create_radio_type_3pos(HWND hwnd, int x, int y, RADIO_3POS_STRU* pr) {
    HWND hRadio0, hRadio1, hRadio2;

    ui11_create_label_no_border(hwnd, pr->label, x, y, 120, 20);
    int GAP = 15;
    hRadio0 = CreateWindow(L"BUTTON", pr->r0_label,
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, // Group style only on the **first** radio button
        x, y + GAP, 60, 20, hwnd, (HMENU)(INT_PTR)(pr->id0), NULL, NULL);
    ui10_apply_font_to_control(hRadio0, UI_FONT_9PT);

    hRadio1 = CreateWindow(L"BUTTON", pr->r1_label,
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        x + 60, y + GAP, 60, 20, hwnd, (HMENU)(INT_PTR)(pr->id1), NULL, NULL);
    ui10_apply_font_to_control(hRadio1, UI_FONT_9PT);

    hRadio2 = CreateWindow(L"BUTTON", pr->r2_label,
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        x + 120, y + GAP, 60, 20, hwnd, (HMENU)(INT_PTR)(pr->id2), NULL, NULL);
    ui10_apply_font_to_control(hRadio2, UI_FONT_9PT);

    // Set default checked radio button
    if (pr->state_default == 0)
        SendMessage(hRadio0, BM_SETCHECK, BST_CHECKED, 0); // default: position 0
    else if (pr->state_default == 1)
        SendMessage(hRadio1, BM_SETCHECK, BST_CHECKED, 0); // default: position 1
    else
        SendMessage(hRadio2, BM_SETCHECK, BST_CHECKED, 0); // default: position 2
}

void ui_create_radio_type_2pos(HWND hwnd, int x, int y, RADIO_2POS_STRU* pr) {
    HWND hRadio0, hRadio1;

    ui11_create_label_no_border(hwnd, pr->label, x, y, 120, 20);
    int GAP = 15;
    hRadio0 = CreateWindow(L"BUTTON", pr->r0_label,
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, // Group style only on the **first** radio button
        x, y + GAP, 50, 20, hwnd, (HMENU)(INT_PTR)(pr->id0), NULL, NULL);
    ui10_apply_font_to_control(hRadio0, UI_FONT_9PT);

    hRadio1 = CreateWindow(L"BUTTON", pr->r1_label,
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        x + 60, y + GAP, 50, 20, hwnd, (HMENU)(INT_PTR)(pr->id1), NULL, NULL);
    ui10_apply_font_to_control(hRadio1, UI_FONT_9PT);


    // Set default checked radio button
    if (pr->state_default == 0)
        SendMessage(hRadio0, BM_SETCHECK, BST_CHECKED, 0); // default: position 0
    else 
        SendMessage(hRadio1, BM_SETCHECK, BST_CHECKED, 0); // default: position 1
}


/*******************************************************************************
 * @brief  originally called by WM_DRAWITEM event in main.cpp
 * @param  lpDrawItem: it contains button id, and other display information
 * @param  text: button text, if NULL, use its own
 * @param  state: 0=defalt, 1=on, 2.. other states
 * @return xxxx
 *******************************************************************************/
void ui30_draw_button_led_black(LPDRAWITEMSTRUCT lpDrawItem, const wchar_t* text, bool state) {
    HDC hdc = lpDrawItem->hDC;
    RECT rc = lpDrawItem->rcItem;
    HWND hButton = lpDrawItem->hwndItem;

    // Background
    HBRUSH bgBrush = CreateSolidBrush(state ? RGB(0, 0, 0) : RGB(240, 240, 240));
    FillRect(hdc, &rc, bgBrush);
    DeleteObject(bgBrush);

    // Text
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, state ? RGB(255, 0, 0) : RGB(0, 0, 0));
    if (text == NULL) {
        wchar_t textBuffer[256];
        GetWindowText(hButton, textBuffer, _countof(textBuffer)); // Get the button's current text
        DrawText(hdc, textBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    else {
        DrawText(hdc, text,       -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // Optional: draw border
    DrawEdge(hdc, &rc, EDGE_RAISED, BF_RECT);
}

/*******************************************************************************
 * @brief  originally called by WM_DRAWITEM event in main.cpp
 * @param  lpDrawItem: it contains button id, and other display information
 * @param  text: button text, if NULL, use its own
 * @param  state: 0=defalt, 1=on, 2.. other states
 * @param  int color: color when active (state !=0)  e.g. UI_COLOR_GREEN 
 * @return xxxx
 *******************************************************************************/
void ui30_draw_button_led_color(LPDRAWITEMSTRUCT lpDrawItem, const wchar_t* text, bool state, int color) {
    HDC hdc = lpDrawItem->hDC;
    RECT rc = lpDrawItem->rcItem;
    HWND hButton = lpDrawItem->hwndItem;

    // Background
    HBRUSH bgBrush = CreateSolidBrush(state ? color : RGB(240, 240, 240));
    FillRect(hdc, &rc, bgBrush);
    DeleteObject(bgBrush);

    // Text
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));
    if (text == NULL) {
        wchar_t textBuffer[256];
        GetWindowText(hButton, textBuffer, _countof(textBuffer)); // Get the button's current text
        DrawText(hdc, textBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    else {
        DrawText(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // Optional: draw border
    DrawEdge(hdc, &rc, EDGE_RAISED, BF_RECT);
}


/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui30_draw_custom_button_trigger_redraw(HWND hwnd, int id) {
    InvalidateRect(GetDlgItem(hwnd, id), NULL, TRUE); // Force redraw
}



/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_create_button_type_lock(HWND hwnd, int x, int y, const wchar_t* text, int id) {
    HWND b1 = CreateWindow(L"BUTTON", text, 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER | BS_OWNERDRAW,
        x, y, UI_BUTTON_W_WIDE, UI_BUTTON_H, hwnd, (HMENU)(INT_PTR)id, NULL, NULL);
    ui10_apply_font_to_control(b1, UI_FONT_9PT);
}

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_create_button_type_click(HWND hwnd, int x, int y, const wchar_t* text, int id) {
    HWND b1 = CreateWindow(L"BUTTON", text, 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
        x, y, UI_BUTTON_W_WIDE, UI_BUTTON_H, hwnd, (HMENU)(INT_PTR)id, NULL, NULL);
    ui10_apply_font_to_control(b1, UI_FONT_9PT);
}
        

void ui_create_button_type_continuous(HWND hwnd, int x, int y, const wchar_t*name,int id) {
    extern LRESULT CALLBACK ui_callback_type_continuous(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

    HWND b1;
    DWORD style = WS_VISIBLE | WS_CHILD | WS_BORDER;

    b1 = CreateWindow(L"BUTTON", name, style, x, y,
        UI_BUTTON_W_WIDE, UI_BUTTON_H, // button width and height
        hwnd, (HMENU)(INT_PTR)(id), NULL, NULL);

    ui10_apply_font_to_control(b1, UI_FONT_9PT);
    SetWindowSubclass(b1, ui_callback_type_continuous, 1, 0);
}







/********************************* end of file ********************************/

