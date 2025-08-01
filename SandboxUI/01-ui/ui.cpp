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
 * @param  hwndTarget, the window you want to apply the font
 * @param  pt, the font size, usually 9 or 12 in win11
 * @return xxxx
 *******************************************************************************/
void ui12_apply_font_to_control(HWND hwndTarget, int pt) {
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
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui1_init_widgets(HWND hwnd) { // Labels


    int i = 0;

    HWND hStaticLabel = CreateWindow(L"STATIC", L"  -- INSPECTION -- ", WS_CHILD | WS_VISIBLE,
        20, 40+i*25, 300, 40, hwnd, NULL, NULL, NULL);

    ui12_apply_font_to_control(hStaticLabel, UI_FONT_12PT);


    i = 2;

    HWND hStaticLabel2 = CreateWindow(L"STATIC", L"  -- INSPECTION -- ", WS_CHILD | WS_VISIBLE,
    20, 40 + i * 40, 300, 40, hwnd, NULL, NULL, NULL);

    ui12_apply_font_to_control(hStaticLabel2, UI_FONT_9PT);
}
        

/********************************* end of file ********************************/

