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
#include "ui.h"
#include "button_id.h"

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

} UI_CONTROL_STRU;

/*******************************************************************************
******************************** Private define ********************************
*******************************************************************************/

/*******************************************************************************
********************************* Private macro ********************************
*******************************************************************************/
typedef struct {
    const char* name;
    int id;
} BUTTON_ID_NAME_STRU;

#define BUTTON_ID_DEFINE(name)          { #name, name }


/*******************************************************************************
******************************* Private variables ******************************
*******************************************************************************/
UI_CONTROL_STRU _ui_control_stru;
/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/

void ui10_apply_font_to_control(HWND hwndTarget, int pt);
void ui11_init_label(HWND hwnd);
void ui12_init_click_button(HWND hwnd, int x, int y);
void ui13_init_radio_group(HWND hwnd);

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

}
        


/*******************************************************************************
 * @brief  called by main.cpp WM_COMMAND event.
 * @param  id : the button id 
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui2_button_callback(int id) {
    switch (id) {
    case BUTTON_ID_UP:
        OutputDebugStringA("UP pressed\n");
        break;
    case BUTTON_ID_DOWN:
        OutputDebugStringA("DOWN pressed\n");
        break;
    case BUTTON_ID_ENABLE:
        OutputDebugStringA("ENABLE pressed\n");
        break;
        break;
    }
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

    HWND b1 = CreateWindow(L"BUTTON", L"UP", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, 80, 30, hwnd, (HMENU)BUTTON_ID_UP, NULL, NULL);
    ui10_apply_font_to_control(b1, UI_FONT_9PT);


    HWND b2 = CreateWindow(L"BUTTON", L"DOWN", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y + 30, 80, 30, hwnd, (HMENU)BUTTON_ID_DOWN, NULL, NULL);
    ui10_apply_font_to_control(b2, UI_FONT_9PT);


    HWND b3 = CreateWindow(L"BUTTON", L"ENABLE", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
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
        


/********************************* end of file ********************************/

