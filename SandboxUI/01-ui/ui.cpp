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
typedef struct {
    const wchar_t* text;
    int id;    
    int is_clicked; //0=not, 1=clicked, used when this button is also used as a led

} BUTTON_STRU;

//#define FILL_BUTTON_ID(name)          { #name, name, 0 }


/*******************************************************************************
******************************* Private variables ******************************
*******************************************************************************/
UI_CONTROL_STRU _ui_control_stru;

BUTTON_STRU button_hop[] = {
    { L"__SKIP",     0,          0 },//'__SKIP' means the button invisiable
    { L"10🔻",       H10DN,      0 },

    { L"9🔺",        H9UP,       0 },
    { L"9🔻",        H9DN,       0 },

    { L"8🔺",        H8UP,       0 },
    { L"8🔻",        H8DN,       0 },

    { L"7🔺",        H7UP,       0 },
    { L"7🔻",        H7DN,       0 },

    { L"6🔺",        H6UP,       0 },
    { L"6🔻",        H6DN,       0 },

    { L"5🔺",        H5UP,       0 },
    { L"5🔻",        H5DN,       0 },

    { L"4🔺",        H4UP,       0 },
    { L"4🔻",        H4DN,       0 },

    { L"3🔺",        H3UP,       0 },
    { L"3🔻",        H3DN,       0 },

    { L"2🔺",        H2UP,       0 },
    { L"2🔻",        H2DN,       0 },

    { L"1🔺",        H1UP,       0 },
};

BUTTON_STRU button_cop1[] = {
    { L"10", CF10, 0 },
    { L"9", CF9, 0 },
    { L"8", CF8, 0 },
    { L"7", CF7, 0 },
    { L"6", CF6, 0 },
    { L"5", CF5, 0 },
    { L"4", CF4, 0 },
    { L"3", CF3, 0 },
    { L"2", CF2, 0 },
    { L"1", CF1, 0 },

    { L"OPEN DOOR",     D1_OPEN,    0 },
    { L"CLOSE DOOR ",   D1_CLOSE,   0 },

};


HWND hButtonUp, hButtonDown, hButtonLight;
bool up = false;
bool down = false;
bool light = false;

/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/

 
/*******************************************************************************
******************************* Private functions ******************************
*******************************************************************************/


/*******************************************************************************
 * @brief  create HOPs buttons panel in a block(using a label)
 * @param  gx,gy: parent group position
 * @param  bw,bh: button width and height
 * How it works? if you have button_hop = {1,2,3,4,5 .... }
 *         the buttons are displayed in the order of:
 *         [1] [2] --> BUTTON_PER_ROW = 2
 *         [3] [4]
 *         [5] ... 
 * @return xxxx
 *******************************************************************************/
void ui_create_button_hop(HWND hwnd, int gx, int gy, int bw, int bh) {
    int num = (sizeof(button_hop)) / sizeof(BUTTON_STRU);

    for (int i = 0; i < num; i++) {
        if (i == 0) {
            continue;
        }
        HWND b1 = CreateWindow(L"BUTTON", button_hop[i].text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            gx + (i % UI_BUTTON_PER_ROW) * (bw + UI_BUTTON_GAP_COLUMN),
            gy + ((i / UI_BUTTON_PER_ROW)) * (bh + UI_BUTTON_GAP_ROW),
            bw, bh, //button width and height
            hwnd, (HMENU)(button_hop[i].id), NULL, NULL);
        ui10_apply_font_to_control(b1, UI_FONT_9PT);
    }
}
     

/*******************************************************************************
 * @brief  Subclass procedure
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
LRESULT CALLBACK ui41_ButtonSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    int bid = GetDlgCtrlID(hwnd);
    switch (uMsg) {
    case WM_LBUTTONDOWN:
        switch (bid) {
        case D1_OPEN:
            ui_internal_printf("open=1.");
            break;
        case D1_CLOSE:
            ui_internal_printf("close=1.");

            break;
        default:
            break;
        }
        break;

    case WM_LBUTTONUP:
        switch (bid) {
        case D1_OPEN:
            ui_internal_printf("open=0.");
            break;
        case D1_CLOSE:
            ui_internal_printf("close=0.");

            break;
        default:
            break;
        }
        break;

    case WM_NCDESTROY:
        // Clean up subclass when control is destroyed
        RemoveWindowSubclass(hwnd, ui41_ButtonSubclassProc, uIdSubclass);
        break;
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam); // Forward to original
}


/*******************************************************************************
 * @brief  create COP1 buttons panel in a block(using a label)
 * @param  gx,gy: parent group position
 * @param  bw,bh: button width and height
 * How it works? if you have button_hop = {1,2,3,4,5 .... }
 *         the buttons are displayed in the order of:
 *         [1] [2] --> BUTTON_PER_ROW = 2
 *         [3] [4]
 *         [5] ...
 *         [NOTE] the last 2 buttons are press type, not click type, they also need
 *                their own callback function.
 * @return xxxx
 *******************************************************************************/
void ui_create_button_cop1(HWND hwnd, int gx, int gy, int bw, int bh) {

    int num = (sizeof(button_cop1)) / sizeof(BUTTON_STRU);

    for (int i = 0; i < num; i++) {
        HWND b1;
        DWORD style;
        if (button_cop1[i].id == D1_CLOSE || button_cop1[i].id == D1_OPEN) {
            style = WS_VISIBLE | WS_CHILD | WS_BORDER;
        }
        else {
            style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
        }
        b1 = CreateWindow(L"BUTTON", button_cop1[i].text, style,
            gx + (i % UI_BUTTON_PER_ROW) * (bw + UI_BUTTON_GAP_COLUMN),
            gy + ((i / UI_BUTTON_PER_ROW)) * (bh + UI_BUTTON_GAP_ROW),
            bw, bh, // button width and height
            hwnd, (HMENU)(button_cop1[i].id), NULL, NULL);
        ui10_apply_font_to_control(b1, UI_FONT_9PT);

        if (button_cop1[i].id == D1_CLOSE || button_cop1[i].id == D1_OPEN) {
            style = WS_VISIBLE | WS_CHILD;
            SetWindowSubclass(b1, ui41_ButtonSubclassProc, 1, 0);
        }
    }

}



/*******************************************************************************
 * @brief  called by WndProc on WM_COMMAND event, Only need to be accessed by button id.
 * @param  id: from LOWORD(wParam)
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui51_radio_group_callback(int id) {
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

bool __cop3_enabled = 0;

/*******************************************************************************
 * @brief  called by main.cpp WM_COMMAND event.
 * @param  id : the button id
 * @param  hwnd: the parent wnd
 * @return xxxx
 *******************************************************************************/
void ui2_lock_button_action_callback(HWND hwnd, int id) {
    switch (id) {
    case BUTTON_ID_UP:
        OutputDebugStringA("UP pressed\n");
        _ui_control_stru.is_up = !(_ui_control_stru.is_up);
        ui30_draw_custom_button_trigger_redraw(hwnd, id); // Force redraw

        break;
    case BUTTON_ID_DOWN:
        OutputDebugStringA("DOWN pressed\n");

        _ui_control_stru.is_down = !(_ui_control_stru.is_down);
        ui30_draw_custom_button_trigger_redraw(hwnd, id); // Force redraw
        break;
    case BUTTON_ID_ENABLE:
        OutputDebugStringA("ENABLE pressed\n");
        _ui_control_stru.is_enable = !(_ui_control_stru.is_enable);
        ui30_draw_custom_button_trigger_redraw(hwnd, id); // Force redraw
        break;

    case ID_COP3_ENABLE:
        __cop3_enabled = !__cop3_enabled;
        ui_internal_printf("COP3 ENABLE=%d", __cop3_enabled);
        ui30_draw_custom_button_trigger_redraw(hwnd, id); // Force redraw

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
void ui3_lock_button_draw_callback(LPDRAWITEMSTRUCT lpDrawItem) {
    int id = lpDrawItem->CtlID;
    switch (id) {
    case BUTTON_ID_UP:
        ui30_draw_custom_button(lpDrawItem, NULL, _ui_control_stru.is_up);

        break;
    case BUTTON_ID_DOWN:
        ui30_draw_custom_button(lpDrawItem, NULL, _ui_control_stru.is_down);

        break;
    case BUTTON_ID_ENABLE:
        ui30_draw_custom_button(lpDrawItem, NULL, _ui_control_stru.is_enable);
        break;

    case ID_COP3_ENABLE:
        ui30_draw_custom_button(lpDrawItem, NULL, __cop3_enabled);
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
   
    //create main window top layout
    ui11_create_label(hwnd, L" Animation", UI_ANIMATION_X, UI_ANIMATION_Y, UI_ANIMATION_W, UI_ANIMATION_H);
    ui11_create_label(hwnd, L" Simulator", UI_LABELBOX_X, UI_LABELBOX_Y, UI_LABELBOX_W, UI_LABELBOX_H);
    ui11_create_label(hwnd, L" ElevatorCore", UI_LABELBOX2_X, UI_LABELBOX2_Y, UI_LABELBOX2_W, UI_LABELBOX2_H);
    ui_logbox_create(hwnd, COLUMN2_X, UI_LOGBOX_Y, UI_LOGBOX_W, UI_LOGBOX_H, 0);
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
    

    ui_create_button_hop(hwnd, UI_HOP_X+7, UI_HOP_Y+20, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT);
    ui_create_button_cop1(hwnd, UI_COP1_X + 7, UI_COP1_Y + 20, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT);


    ui_logbox_create(hwnd, UI_LOGBOX_X, UI_LOGBOX_Y, UI_LOGBOX_W, UI_LOGBOX_H, 0);
    ui_internal_printf("print what you want");



    RADIO_3POS_STRU switch_fire2 = { L"FIRE PH2", L"OFF", L"HOLD", L"ON", ID_COP3_FIRE_PH2_OFF, ID_COP3_FIRE_PH2_HOLD, ID_COP3_FIRE_PH2_ON, 0 };
    ui50_create_3pos_switch(hwnd, UI_COP3_X + 5, UI_COP3_Y + 20, &switch_fire2);
    
    RADIO_2POS_STRU switch_runstop = { L"RUN/STOP", L"RUN", L"STOP", ID_COP3_RUN_RUN, ID_COP3_RUN_STOP, 1};
    ui50_create_2pos_switch(hwnd, UI_COP3_X + 5, UI_COP3_Y + 60, &switch_runstop);

    ui_lock_button_create(hwnd, UI_COP3_X + 13, UI_COP3_Y + 100, L"ENABLE", ID_COP3_ENABLE);
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

