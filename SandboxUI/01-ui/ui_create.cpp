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
// clang-format off

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

BUTTON_STRU button_output[] = {
    { L"MOTOR EN",  ID_MOTOR_EN, 0 },
    { L"BUZZER",    ID_BUZZER, 0 },    
    { L"E.B. PICK", ID_EB_PICK, 0 },
    { L"LATERN UP", ID_LATERN_UP, 0 },
    { L"CHIME",     ID_CHIME, 0 },
    { L"LATERN DN", ID_LATERN_DN, 0 },
};

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

BUTTON_STRU button_nonmanual[] = {
 //   { L"__SKIP", 0, 0 }, //'__SKIP' means the button invisiable
    { L"MOTOR ENABLED", ID_MOTOR_ENABLED, 0 },
    { L"MB_PICKED", ID_MB_PICKED, 0 },
    { L"EB_PICKED", ID_EB_PICKED, 0 },
    { L"DOOR_GATE_SWITCH",ID_DOOR_GATE_SWITCH, 0 },
    { L"DOOR_PHOTOEYE",ID_DOOR_PHOTOEYE, 0 },
    { L"DOOR_ZONE1",ID_DOOR_ZONE1, 0 },
    { L"DOOR_ZONE2",ID_DOOR_ZONE2, 0 },
    { L"DOOR_OPEN_LIMIT",ID_DOOR_OPEN_LIMIT, 0 },
    { L"CARTOP_ESCAPE",ID_CARTOP_ESCAPE, 0 },
    { L"CARTOP_SAFETIES",ID_CARTOP_SAFETIES, 0 },
    { L"PIT_SWITCH",ID_PIT_SWITCH, 0 },
    { L"BUFFER_HIT",ID_BUFFER_HIT, 0 },
    { L"FINAL_LIMIT_TOP",ID_FINAL_LIMIT_TOP, 0 },
    { L"FINAL_LIMIT_BOTTOM",ID_FINAL_LIMIT_BOTTOM, 0 },
    { L"SPEED_GOVERNOR",ID_SPEED_GOVERNOR, 0 },
    { L"INTERLOCK_TOP",ID_INTERLOCK_TOP, 0 },
    { L"INTERLOCK_MIDDLE",ID_INTERLOCK_MIDDLE, 0 },
    { L"INTERLOCK_BOTTOM",ID_INTERLOCK_BOTTOM, 0 },
    { L"SMOKE1_UPPER_FLOOR",ID_SMOKE1_UPPER_FLOOR, 0 },
    { L"SMOKE2_LOBBY",ID_SMOKE2_LOBBY, 0 },
    { L"SMOKE3_MACHINEROOM",ID_SMOKE3_MACHINEROOM, 0 },
    { L"SMOKE4_HOISTWAY",ID_SMOKE4_HOISTWAY, 0 },
    { L"SMOKE5_PIT",ID_SMOKE5_PIT, 0 },
};

BUTTON_STRU button_debug[] = {
    { L"X_APP",X_APP, 0 },  // Simulator Stop
    { L"X_CORE",X_CORE, 0 }, // Simulator Start
    { L"X_SIM",X_SIM, 0 },
    { L"Show IO list",ID_SHOW_IO_LIST, 0 },

    { L"COP_UP",COP_UP, 0 },
    { L"COP_DN",COP_DN, 0 },
    { L"COP_STOP",COP_STOP, 0 },

    { L"MOTOR_UP",MOTOR_UP, 0 },
    { L"MOTOR_DN",MOTOR_UP, 0 },
    { L"MOTOR_STOP", MOTOR_ST, 0 },

    { L"SPEED 0.2M/S",SPEED1,  0 },// 0.2 M/S
    { L"SPEED 1.0M/S",SPEED2, 0 }, // 1.0 M/S
    { L"SPEED 2.0M/S",SPEED3,  0 },// 2.0 M/S

    { L"NORMAL",NORMAL, 0 },
    { L"INSP",INSP, 0 },
    { L"INDP",INDP, 0 },

    { L"GOTO -1M",Go_n1m,  0 },// test traction to goto -1m abs
    { L"GOTO 1M",Go_1m, 0 },  // test traction to goto 1m abs
    { L"GOTO 5M",Go_5m,  0 }, // test traction to goto 2m abs

    { L"RESET",RESET,  0 },// this will reset and automatically start logging.
    { L"STOP_LOG",STOP_LOG, 0 },
    { L"SAVE_PRINT",SAVE_PRINT, 0 }, // save the printf logbox to a file
    { L"OPEN_DOOR",OPEN_DOOR, 0 },
    { L"CLOSE_DOOR",CLOSE_DOOR, 0 },
    { L"HOLD_DOOR",HOLD_DOOR, 0 },
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
* @brief  create OUTPUT led (buttons in fact)
 * @param  gx,gy: parent group position
 * @param  bw,bh: button width and height
 * How it works? if you have button_hop = {1,2,3,4,5 .... }
 *         the buttons are displayed in the order of:
 *         [1] [2] --> BUTTON_PER_ROW = 2
 *         [3] [4]
 *         [5] ...
*******************************************************************************/
void ui_create_led_output(HWND hwnd, int gx, int gy, int bw, int bh) {
    int num = (sizeof(button_output)) / sizeof(BUTTON_STRU);
    int buttons_per_row = 2;
    for (int i = 0; i < num; i++) {
        HWND b1 = CreateWindow(L"BUTTON", button_output[i].text, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            gx + (i % buttons_per_row) * (bw + UI_BUTTON_GAP_COLUMN),
            gy + ((i / buttons_per_row)) * (bh + UI_BUTTON_GAP_ROW),
            bw, bh, // button width and height
            hwnd, (HMENU)(INT_PTR)(button_output[i].id), NULL, NULL);
        ui10_apply_font_to_control(b1, UI_FONT_9PT);
    }
}
        

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
    int buttons_per_row = 2;
    
    for (int i = 0; i < num; i++) {
        if (i == 0) { //skip first one
            continue;
        }
        HWND b1 = CreateWindow(L"BUTTON", button_hop[i].text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            gx + (i % buttons_per_row) * (bw + UI_BUTTON_GAP_COLUMN),
            gy + ((i / buttons_per_row)) * (bh + UI_BUTTON_GAP_ROW),
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
    int buttons_per_row = 2;

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
            gx + (i % buttons_per_row) * (bw + UI_BUTTON_GAP_COLUMN),
            gy + ((i / buttons_per_row)) * (bh + UI_BUTTON_GAP_ROW),
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
 * @return xxxx
 *******************************************************************************/
void ui_create_cop2(HWND hwnd, int x, int y) {
    RADIO_2POS_STRU switch_hoist = { L"HOIST. ACCESS", L"ON", L"OFF", ID_COP2_HOIST_ACCESS_ON, ID_COP2_HOIST_ACCESS_OFF, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 20, &switch_hoist);
 
    RADIO_2POS_STRU switch_ind = { L"IND. SERVICE", L"ON", L"OFF", ID_COP2_IND_ON, ID_COP2_IND_OFF, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 60, &switch_ind);

    RADIO_2POS_STRU switch_runstop = { L"RUN/STOP", L"STOP", L"RUN", ID_COP2_RUN_STOP, ID_COP2_RUN_RUN, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 100, &switch_runstop);

    
    ui_create_button_type_continuous(hwnd, UI_COP2_X+13, UI_COP2_Y + 140, L"UP", ID_COP2_UP);
    ui_create_button_type_lock(hwnd, UI_COP2_X + 13, UI_COP2_Y + 170, L"ENABLE", ID_COP2_ENABLE);
    ui_create_button_type_continuous(hwnd, UI_COP2_X+13, UI_COP2_Y + 200, L"DOWN", ID_COP2_DOWN);
}
        

/*******************************************************************************
 * @brief  create cop3 input block
 * @param  x, y = the block box left/top position
 * @return xxxx
 *******************************************************************************/
void ui_create_cop3(HWND hwnd, int x, int y) {
    RADIO_3POS_STRU switch_fire2 = { L"FIRE PH2", L"OFF", L"HOLD", L"ON", ID_COP3_FIRE_PH2_OFF, ID_COP3_FIRE_PH2_HOLD, ID_COP3_FIRE_PH2_ON, 0 };
    ui_create_radio_type_3pos(hwnd, x + 5, y + 20, &switch_fire2);

    RADIO_2POS_STRU switch_runstop = { L"RUN/STOP", L"STOP", L"RUN", ID_COP3_RUN_STOP, ID_COP3_RUN_RUN, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 60, &switch_runstop);

    ui_create_button_type_continuous(hwnd, x + 13, y + 100, L"OPEN DOOR", ID_COP3_OPEN_DOOR);// ◀▶
    ui_create_button_type_continuous(hwnd, x + 13, y + 130, L"CLOSE DOOR", ID_COP3_CLOSE_DOOR);// ▶◀
    ui_create_button_type_click(hwnd, x + 13, y + 160, L"CALL CANCEL", ID_COP3_CALL_CACEL);

}
        

/*******************************************************************************
 * @brief  create hop2 input block
 * @param  x, y = the block box left/top position
 * @return xxxx
 *******************************************************************************/
void ui_create_hop2(HWND hwnd, int x, int y) {
    RADIO_3POS_STRU switch_fire1 = { L"FIRE PH1", L"RESET", L"OFF", L"ON", ID_HOP2_FIRE_RECALL_RESET, ID_HOP2_FIRE_RECALL_OFF, ID_HOP2_FIRE_RECALL_ON, 1 };
    ui_create_radio_type_3pos(hwnd, x + 5, y + 20, &switch_fire1);

    RADIO_3POS_STRU switch_insp_top = { L"INSP. ACCESS TOP", L"UP", L"OFF", L"DOWN", ID_HOP2_INSP_TOP_UP, ID_HOP2_INSP_TOP_OFF, ID_HOP2_INSP_TOP_DOWN, 1 };
    ui_create_radio_type_3pos(hwnd, x + 5, y + 60, &switch_insp_top);

    RADIO_3POS_STRU switch_insp_bottom = { L"INSP. ACCESS BOTTOM", L"UP", L"OFF", L"DOWN", ID_HOP2_INSP_BOTTOM_UP, ID_HOP2_INSP_BOTTOM_OFF, ID_HOP2_INSP_BOTTOM_DOWN, 1 };
    ui_create_radio_type_3pos(hwnd, x + 5, y + 100, &switch_insp_bottom);
}
    

/*******************************************************************************
 * @brief  create machineroom input block
 * @param  x, y = the block box left/top position
 * @return xxxx
 *******************************************************************************/
void ui_create_machineroom(HWND hwnd, int x, int y) {
    RADIO_2POS_STRU switch_insp = { L"INSPECTION", L"ON", L"OFF", ID_MACHINEROOM_INSP_ON, ID_MACHINEROOM_INSP_OFF, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 20, &switch_insp);

    RADIO_2POS_STRU switch_car_door = { L"CAR DOOR BYPASS", L"ON", L"OFF", ID_MACHINEROOM_CAR_DOOR_BYPASS_ON, ID_MACHINEROOM_CAR_DOOR_BYPASS_OFF, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 60, &switch_car_door);

    RADIO_2POS_STRU switch_hall_door = { L"HALL DOOR BYPASS", L"ON", L"OFF", ID_MACHINEROOM_HALL_DOOR_BYPASS_ON, ID_MACHINEROOM_HALL_DOOR_BYPASS_OFF, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 100, &switch_hall_door);


    ui_create_button_type_continuous(hwnd, x + 15, y + 140, L"UP", ID_MACHINEROOM_UP);
    ui_create_button_type_lock(hwnd, x + 15, y + 170, L"ENABLE", ID_MACHINEROOM_ENABLE);
    ui_create_button_type_continuous(hwnd, x + 15, y + 200, L"DOWN", ID_MACHINEROOM_DOWN);
}

/*******************************************************************************
 * @brief  create TOC input block
 * @param  x, y = the block box left/top position
 * @return xxxx
 *******************************************************************************/
void ui_create_toc(HWND hwnd, int x, int y) {
    RADIO_2POS_STRU switch_insp = { L"INSPECTION", L"ON", L"OFF", ID_TOC_INSP_ON, ID_TOC_INSP_OFF, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 20, &switch_insp);

    RADIO_2POS_STRU switch_run = { L"RUN/STOP", L"STOP", L"RUN", ID_TOC_RUN_STOP, ID_TOC_RUN_RUN, 1 };
    ui_create_radio_type_2pos(hwnd, x + 5, y + 60, &switch_run);

    ui_create_button_type_continuous(hwnd, x + 15, y + 100, L"UP", ID_TOC_UP);
    ui_create_button_type_lock(hwnd, x + 15, y + 130, L"ENABLE", ID_TOC_ENABLE);
    ui_create_button_type_continuous(hwnd, x + 15, y + 160, L"DOWN", ID_TOC_DOWN);
}

/*******************************************************************************
 * @brief  create NON-MANUAL buttons panel in a block, 1XN array patterm
 * @param  gx,gy: parent group position
 * @return xxxx
 *******************************************************************************/
void ui_create_button_nonmanual(HWND hwnd, int gx, int gy) {
    int num = (sizeof(button_nonmanual)) / sizeof(BUTTON_STRU);

    for (int i = 0; i < num; i++) {
        HWND b1 = CreateWindow(L"BUTTON", button_nonmanual[i].text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            gx + 15,
            gy + i * (UI_BUTTON_H+UI_BUTTON_GAP_ROW)+20,
            UI_BUTTON_W_WIDE, UI_BUTTON_H, // button width and height
            hwnd, (HMENU)(INT_PTR)(button_nonmanual[i].id), NULL, NULL);
        ui10_apply_font_to_control(b1, UI_FONT_9PT);
    }
}


/*******************************************************************************
 * @brief  create DEBUG buttons panel in a block, 1XN array patterm
 * @param  gx,gy: parent group position
 * @return xxxx
 *******************************************************************************/
void ui_create_button_debug(HWND hwnd, int gx, int gy) {
    int num = (sizeof(button_debug)) / sizeof(BUTTON_STRU);

    for (int i = 0; i < num; i++) {
        HWND b1 = CreateWindow(L"BUTTON", button_debug[i].text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            gx + 15,
            gy + i * (UI_BUTTON_H+UI_BUTTON_GAP_ROW)+20,
            UI_BUTTON_W_WIDE, UI_BUTTON_H, // button width and height
            hwnd, (HMENU)(INT_PTR)(button_debug[i].id), NULL, NULL);
        ui10_apply_font_to_control(b1, UI_FONT_9PT);
    }
}



        

/********************************* end of file ********************************/

