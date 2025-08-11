/**
******************************************************************************
* @file    ui_create.cpp
* @author  ARTHUR
* @version V1
* @date    2025-08-11 08:39:10
* @brief   create ui blocks
*
*
******************************************************************************
* @attention
*
*
******************************************************************************
*/

/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/

#include <windows.h>


#include "../00-app/top_config.h"
#include "ui.h"
#include "ui_lowlevel.h"
#include "ui_logbox.h"

#include <commctrl.h>                // For SetWindowSubclass, DefSubclassProc
#pragma comment(lib, "comctl32.lib") // DefSubclassProc is not part of the core Windows API (windows.h), but part of the Common Controls Library; This library provides the actual implementation of functions declared in commctrl.h.


/*******************************************************************************
******************************** Private typedef *******************************
*******************************************************************************/

/*******************************************************************************
******************************** Private define ********************************
*******************************************************************************/

BUTTON_STRU button_hop[] = {
    { L"__SKIP", 0, 0 }, //'__SKIP' means the button invisiable
    { L"10🔻", H10DN, 0 },

    { L"9🔺", H9UP, 0 },
    { L"9🔻", H9DN, 0 },

    { L"8🔺", H8UP, 0 },
    { L"8🔻", H8DN, 0 },

    { L"7🔺", H7UP, 0 },
    { L"7🔻", H7DN, 0 },

    { L"6🔺", H6UP, 0 },
    { L"6🔻", H6DN, 0 },

    { L"5🔺", H5UP, 0 },
    { L"5🔻", H5DN, 0 },

    { L"4🔺", H4UP, 0 },
    { L"4🔻", H4DN, 0 },

    { L"3🔺", H3UP, 0 },
    { L"3🔻", H3DN, 0 },

    { L"2🔺", H2UP, 0 },
    { L"2🔻", H2DN, 0 },

    { L"1🔺", H1UP, 0 },
};

BUTTON_STRU button_cop1[] = {
    { L"12", CF12, 0 },
    { L"11", CF11, 0 },    
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

    { L"OPEN DOOR", D1_OPEN, 0 },
    { L"CLOSE DOOR ", D1_CLOSE, 0 },

};

/*******************************************************************************
********************************* Private macro ********************************
*******************************************************************************/

/*******************************************************************************
******************************* Private variables ******************************
*******************************************************************************/

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
            bw, bh, // button width and height
            hwnd, (HMENU)(INT_PTR)(button_hop[i].id), NULL, NULL);
        ui10_apply_font_to_control(b1, UI_FONT_9PT);
    }
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
    extern LRESULT CALLBACK ui_callback_type_continuous(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData); 
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
            hwnd, (HMENU)(INT_PTR)(button_cop1[i].id), NULL, NULL);
        ui10_apply_font_to_control(b1, UI_FONT_9PT);

        if (button_cop1[i].id == D1_CLOSE || button_cop1[i].id == D1_OPEN) {
            style = WS_VISIBLE | WS_CHILD;
            SetWindowSubclass(b1, ui_callback_type_continuous, 1, 0);
        }
    }
}



/*******************************************************************************
 * @brief  create cop2 input block
 * @param  x, y = the block box left/top position
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_create_cop2(HWND hwnd, int x, int y) {
    RADIO_2POS_STRU switch_hoist = { L"HOIST. ACCESS", L"ON", L"OFF", ID_COP2_HOIST_ACCESS_ON, ID_COP2_HOIST_ACCESS_OFF, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 20, &switch_hoist);
 
    RADIO_2POS_STRU switch_ind = { L"IND. SERVICE", L"ON", L"OFF", ID_COP2_IND_ON, ID_COP2_IND_OFF, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 60, &switch_ind);

    RADIO_2POS_STRU switch_runstop = { L"RUN/STOP", L"ON", L"OFF", ID_COP2_RUN_RUN, ID_COP2_RUN_STOP, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 100, &switch_runstop);

    
    ui_create_button_type_continuous(hwnd, UI_COP2_X+13, UI_COP2_Y + 140, L"UP", ID_COP2_UP);
    ui_create_button_type_lock(hwnd, UI_COP2_X + 13, UI_COP2_Y + 170, L"ENABLE", ID_COP2_ENABLE);
    ui_create_button_type_continuous(hwnd, UI_COP2_X+13, UI_COP2_Y + 200, L"DOWN", ID_COP2_DOWN);
}
        

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_create_cop3(HWND hwnd, int x, int y) {
    RADIO_3POS_STRU switch_fire2 = { L"FIRE PH2", L"OFF", L"HOLD", L"ON", ID_COP3_FIRE_PH2_OFF, ID_COP3_FIRE_PH2_HOLD, ID_COP3_FIRE_PH2_ON, 0 };
    ui_create_radio_type_3pos(hwnd, x + 5, y + 20, &switch_fire2);

    RADIO_2POS_STRU switch_runstop = { L"RUN/STOP", L"ON", L"OFF", ID_COP3_RUN_RUN, ID_COP3_RUN_STOP, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 60, &switch_runstop);

    ui_create_button_type_continuous(hwnd, x + 13, y + 100, L"OPEN DOOR ◀▶", ID_COP3_OPEN_DOOR);
    ui_create_button_type_continuous(hwnd, x + 13, y + 130, L"CLOSE DOOR ▶◀", ID_COP3_CLOSE_DOOR);
    ui_create_button_type_click(hwnd, x + 13, y + 160, L"CALL CANCEL", ID_COP3_CALL_CACEL);

}
        

/********************************* end of file ********************************/

