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


#include "../00-app/top_config.h"
#include "ui.h"
#include "ui_lowlevel.h"
#include "ui_logbox.h"

/*******************************************************************************
******************************** Private typedef *******************************
*******************************************************************************/



//inner layer, the contorl data ui uses itself
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


//#define FILL_BUTTON_ID(name)          { #name, name, 0 }


/*******************************************************************************
******************************* Private variables ******************************
*******************************************************************************/
UI_CONTROL_STRU _ui_control_stru;


/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/
extern void ui_create_button_hop(HWND hwnd, int gx, int gy, int bw, int bh);
extern void ui_create_button_cop1(HWND hwnd, int gx, int gy, int bw, int bh);
extern void ui_create_cop2(HWND hwnd, int x, int y);
extern void ui_create_cop3(HWND hwnd, int x, int y);
extern void ui_create_hop2(HWND hwnd, int x, int y);
extern void ui_create_machineroom(HWND hwnd, int x, int y);
extern void ui_create_toc(HWND hwnd, int x, int y);



/*******************************************************************************
******************************* Private functions ******************************
*******************************************************************************/


     

/*******************************************************************************
 * @brief  callback for continuous-press buttons
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
LRESULT CALLBACK ui_callback_type_continuous(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    int bid = GetDlgCtrlID(hwnd);
    switch (uMsg) {
    case WM_LBUTTONDOWN:
        switch (bid) {
        case D1_OPEN:
            ui_internal_printf("COP open door =1.");
            break;
        case D1_CLOSE:
            ui_internal_printf("COP close door =1.");
            break;

        case ID_COP2_UP:
            ui_internal_printf("COP2-UP =1.");
            break;
        case ID_COP2_DOWN:
            ui_internal_printf("COP2-DOWN =1.");
            break;

        case ID_TOC_UP:
            ui_internal_printf("TOC-UP =1.");
            break;
        case ID_TOC_DOWN:
            ui_internal_printf("TOC-DOWN =1.");
            break;

        case ID_MACHINEROOM_UP:
            ui_internal_printf("M.R.-UP =1.");
            break;
        case ID_MACHINEROOM_DOWN:
            ui_internal_printf("M.R.-DOWN =1.");
            break;

        case ID_COP3_OPEN_DOOR:
            ui_internal_printf("COP3 OPEN DOOR =1.");
            break;
        case ID_COP3_CLOSE_DOOR:
            ui_internal_printf("COP3 CLOSE DOOR =1.");
            break;

        default:break;
        }
        break;

    case WM_LBUTTONUP:
        switch (bid) {
        case D1_OPEN:
            ui_internal_printf("COP open door =0.");
            break;
        case D1_CLOSE:
            ui_internal_printf("COP close door =0.");
            break;

        case ID_COP3_OPEN_DOOR:
            ui_internal_printf("COP3 OPEN DOOR =0.");
            break;
        case ID_COP3_CLOSE_DOOR:
            ui_internal_printf("COP3 CLOSE DOOR =0.");
            break;

        case ID_COP2_UP:
            ui_internal_printf("COP2-UP =0.");
            break;
        case ID_COP2_DOWN:
            ui_internal_printf("COP2-DOWN =0.");
            break;


        case ID_TOC_UP:
            ui_internal_printf("TOC-UP =0.");
            break;
        case ID_TOC_DOWN:
            ui_internal_printf("TOC-DOWN =0.");
            break;

        case ID_MACHINEROOM_UP:
            ui_internal_printf("M.R.-UP =0.");
            break;
        case ID_MACHINEROOM_DOWN:
            ui_internal_printf("M.R.-DOWN =0.");
            break;

        default:break;
        }
        break;

    case WM_NCDESTROY:
        
        RemoveWindowSubclass(hwnd, ui_callback_type_continuous, uIdSubclass);// Clean up subclass when control is destroyed
        break;
    default:break;
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam); // Forward to original
}





/*******************************************************************************
 * @brief  called by WndProc on WM_COMMAND event, Only need to be accessed by button id.
 * @param  id: from LOWORD(wParam)
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_callback_type_radio(int id) {
    switch (id) // LOWORD = control ID
    {
    case ID_COP3_FIRE_PH2_OFF: 
        ui_internal_printf("switch to PH2-OFF.");
        break;
    case ID_COP3_FIRE_PH2_HOLD: 
        ui_internal_printf("switch to PH2-HOLD.");
        break;
    case ID_COP3_FIRE_PH2_ON: 
        ui_internal_printf("switch to PH2-ON.");
        break;
    }
}

bool __button_machineroom_enable = 0;//0=initial off, 1=on
bool __cop2_enabled = 0;
bool __button_toc_enable = 0;

/*******************************************************************************
 * @brief  called by main.cpp WM_COMMAND event.
 * @param  id : the button id
 * @param  hwnd: the parent wnd
 * @return xxxx
 *******************************************************************************/
void ui_callback_type_lock_step1(HWND hwnd, int id) {
    switch (id) {

    case ID_MACHINEROOM_ENABLE:
        __button_machineroom_enable = !__button_machineroom_enable;
        ui_internal_printf("M.R. ENABLE=%d", __button_machineroom_enable);
        ui30_draw_custom_button_trigger_redraw(hwnd, id); // Force redraw
        break;

    case ID_COP2_ENABLE:
        __cop2_enabled = !__cop2_enabled;
        ui_internal_printf("COP2 ENABLE=%d", __cop2_enabled);
        ui30_draw_custom_button_trigger_redraw(hwnd, id); // Force redraw
        break;

    case ID_TOC_ENABLE:
        __button_toc_enable = !__button_toc_enable;
        ui_internal_printf("TOC ENABLE=%d", __button_toc_enable);
        ui30_draw_custom_button_trigger_redraw(hwnd, id); // Force redraw
        break;

    default:
        break;
    }
}


/*******************************************************************************
 * @brief  called by WM_DRAWITEM, originally triggered by InvalidateRect()
 * @param  lpDrawItem: which button to redraw
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_callback_type_lock_step2(LPDRAWITEMSTRUCT lpDrawItem) {
    int id = lpDrawItem->CtlID;
    switch (id) {

    case ID_MACHINEROOM_ENABLE:
        ui30_draw_custom_button(lpDrawItem, NULL, __button_machineroom_enable);
        break;


    case ID_COP2_ENABLE:
        ui30_draw_custom_button(lpDrawItem, NULL, __cop2_enabled);
        break;

    case ID_TOC_ENABLE:
        ui30_draw_custom_button(lpDrawItem, NULL, __button_toc_enable);
        break;

    default:
        break;
    }
}

/*******************************************************************************
******************************* Public Variables *******************************
*******************************************************************************/

/*******************************************************************************
******************************* Public functions *******************************
*******************************************************************************/

  
        

/*******************************************************************************
 * @brief  called by SandboxUI.cpp/WM_CREATE event
 * @param  hwnd, parent windows
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui1_init_widgets(HWND hwnd) { // Labels
   
////create main window top layout
    ui11_create_label(hwnd, L" Animation", UI_ANIMATION_X, UI_ANIMATION_Y, UI_ANIMATION_W, UI_ANIMATION_H);
    ui11_create_label(hwnd, L" Simulator", UI_LABELBOX_X, UI_LABELBOX_Y, UI_LABELBOX_W, UI_LABELBOX_H);
    ui11_create_label(hwnd, L" ElevatorCore", UI_LABELBOX2_X, UI_LABELBOX2_Y, UI_LABELBOX2_W, UI_LABELBOX2_H);
    ui_create_logbox(hwnd, COLUMN2_X, UI_LOGBOX_Y, UI_LOGBOX_W, UI_LOGBOX_H, 0);
    //column4
    ui11_create_label(hwnd, L" HOPs", UI_HOP_X, UI_HOP_Y, UI_HOP_W, UI_HOP_H);
    ui11_create_label(hwnd, L" HOP2 (Fire Service)", UI_HOP2_X, UI_HOP2_Y, UI_HOP2_W, UI_HOP2_H);
    ui11_create_label(hwnd, L" Machineroom", UI_MACHINEROOM_X, UI_MACHINEROOM_Y, UI_MACHINEROOM_W, UI_MACHINEROOM_H);

    //column5
    ui11_create_label(hwnd, L" COP1 (Passenger Panel)", UI_COP1_X, UI_COP1_Y, UI_COP1_W, UI_COP1_H);
    ui11_create_label(hwnd, L" COP2 (Inspection)", UI_COP2_X, UI_COP2_Y, UI_COP2_W, UI_COP2_H);
    ui11_create_label(hwnd, L" COP3 (Fire Service)", UI_COP3_X, UI_COP3_Y, UI_COP3_W, UI_COP3_H);
    ui11_create_label(hwnd, L" TOC", UI_TOC_X, UI_TOC_Y, UI_TOC_W, UI_TOC_H);

    ui11_create_label(hwnd, L" Non-manual Input Devices", UI_NONMANUAL_X, UI_NONMANUAL_Y, UI_NONMANUAL_W, UI_NONMANUAL_H);
    ui11_create_label(hwnd, L" Debug", UI_DEGUG_X, UI_DEGUG_Y, UI_DEGUG_W, UI_DEGUG_H);
    
////create each block
    //hop
    ui_create_button_hop(hwnd, UI_HOP_X+7, UI_HOP_Y+20, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT);
    //cop1
    ui_create_button_cop1(hwnd, UI_COP1_X + 7, UI_COP1_Y + 20, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT);
    //logbox
    ui_create_logbox(hwnd, UI_LOGBOX_X, UI_LOGBOX_Y, UI_LOGBOX_W, UI_LOGBOX_H, 0);
    ui_internal_printf("print what you want");

    //hop2
    ui_create_hop2(hwnd, UI_HOP2_X, UI_HOP2_Y);

    //machineroom
    ui_create_machineroom(hwnd, UI_MACHINEROOM_X, UI_MACHINEROOM_Y);

    //cop2
    ui_create_cop2(hwnd, UI_COP2_X, UI_COP2_Y);

    //cop3
    ui_create_cop3(hwnd, UI_COP3_X, UI_COP3_Y);

    //top
    ui_create_toc(hwnd, UI_TOC_X, UI_TOC_Y);
}
        


        
 



/*******************************************************************************
 * @brief  get ui run tim in ms, it use the global ts_at_start variable
 * @param  xxxx
 * @return ms since app started
 * @note   overflows after ~49 days
 *******************************************************************************/
//int ui64_get_ui_run_ms() {
//    int ts = (int)GetTickCount64() - _ui_control_stru.ts_at_start;
//    return ts; // overflow after ~49 days
//}


/********************************* end of file ********************************/

