/**
******************************************************************************
* @file    ui_labelbox.cpp
* @author  ARTHUR
* @version V1
* @date    2025-08-18 15:46:14
* @brief   show variables in labelbox 1 and2.
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
#include "ui.h"
#include "ui_lowlevel.h"
#include <stdio.h>

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

#if 1
/*******************************************************************************
 * @brief  update variables in labelbox, Called by draw_all() in WndProc() Timer
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui31_draw_labelbox(HDC hdc, DISP_STRU* disp, UINT64 ms) {
#define LABEL_STRING_SIZE 512 // lable printf buzzer size

    char buf[LABEL_STRING_SIZE];          // main text buffer which holds the entire string of a label window
    char buf2[LABEL_STRING_SIZE] = "010"; // hold part of label string
    char time_str[20];
    int buf_size = sizeof(buf);
    ui65_get_formatted_clock_string(time_str, ms);

    sprintf_s(buf, "V%s (%s %s)\r\n  *** CAR SIMULATION ***\r\nRun:%s\r\nCmd: %s \r\nAPS Speed: %+05d/%04d(sp)mm/s\r\nAPS Height: %05dmm\r\nDir: %s\r\nState: %s\r\nError: %s \r\n\n  *** DOOR SIMULATION ***\r\nCmd: %s\r\nState: %s\nOpening:%d%% \n\nEnabled: %s,%s,%s.",
        VERSION_CODE,
        __DATE__,
        __TIME__,
        time_str,
        "car1_cmd",
        disp->car1_speed,
        disp->car1_speed_sp,
        disp->car1_height,
        "car1_direction",
        "car1_state",  // sim30_get_state_string(disp->car1_state),
        "car1_error",  //,sim33_get_err_string(disp->car1_error),
        "door1-cmd",   // sim_door_get_cmd_string(disp->door1_cmd),
        "door1-state", // sim_door_get_state_string(disp->door1_state),
        disp->door1_position,
        (disp->is_simulator_enable ? "APP" : "X-APP"),
        (disp->is_core_enable ? "CORE" : "X-CORE"),
        (disp->is_car_sim_enabled ? "SIM" : "X-SIM"));

    { // draw text
        // strcpy_s(_ui_control_stru.label1_buf, LABEL_STRING_SIZE, buf);
        // SetWindowTextA(hLabel1, buf);

        // set label position rects
        RECT labelRect1 = { UI_LABELBOX_X, UI_LABELBOX_Y, UI_LABELBOX_X + UI_LABELBOX_W, UI_LABELBOX_Y + UI_LABELBOX_H };

        extern HFONT g_hfont9;
        SelectObject(hdc, g_hfont9);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0)); // black
        Rectangle(hdc, labelRect1.left, labelRect1.top, labelRect1.right, labelRect1.bottom);
        DrawTextA(hdc, buf, -1, &labelRect1, DT_LEFT | DT_TOP | DT_NOPREFIX);
    }

    // Label2

    sprintf_s(buf, "  *** CORE SERVICE STATUS ***\r\nCalls:");
    //==============_ui31_print_binary_array(buf2, 100, service_control_stru.calltable);
    strncat_s(buf, buf_size, buf2, buf_size / 2); // add buf2's content into buf1



    { // draw label2 text
        // SetWindowTextA(hLabel2, buf);
        RECT labelRect2 = { UI_LABELBOX2_X, UI_LABELBOX2_Y, UI_LABELBOX2_X + UI_LABELBOX2_W, UI_LABELBOX2_Y + UI_LABELBOX2_H };
        DrawTextA(hdc, buf, -1, &labelRect2, DT_LEFT | DT_TOP | DT_NOPREFIX);
    }
}

#endif
/********************************* end of file ********************************/

