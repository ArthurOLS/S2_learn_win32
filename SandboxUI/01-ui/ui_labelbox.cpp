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

#include <stdint.h> //to use int32_t type
#include <windows.h> //to use HWND
#include <stdio.h> //use sprintf()

#include "00-app/top.h"
#include "button_id.h"
#include "ui.h" //to use ui datatype
#include "ui_config.h"//UI_LABELBOX_X

/*******************************************************************************
******************************** Private typedef *******************************
*******************************************************************************/

/*******************************************************************************
******************************** Private define ********************************
*******************************************************************************/
#define LABEL_STRING_SIZE               512 // lable printf buzzer size

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
 * @brief  format a string for latched hall and car calls
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void _print_binary_array(char* buf, size_t buf_size, const int call_table[][3]) {
    buf[0] = '\0'; // Start with an empty string

    for (int i = 0; i < APP_FLOOR_NUM; i++) {
        char temp[16];
        // Format index (1-based), then convert the value to a 3-digit binary string
        char c0 = (call_table[i][0]) ? 'U' : '_';
        char c1 = (call_table[i][1]) ? 'D' : '_';
        char c2 = (call_table[i][2]) ? 'C' : '_';
        snprintf(temp, sizeof(temp), " %02d:%c%c%c", i + 1, c0, c1, c2);

        if (i > 0) {            // if it's not the first element, add a comma
            if ((i % 4) == 0) { // Add newline for every 4 up/down/car elements and not at end
                strncat_s(buf, buf_size, "\r\n", 2);
            }
            else {
                strncat_s(buf, buf_size, ",", 2);
            }
        }
        strncat_s(buf, buf_size, temp, 8);
    }
}



/*******************************************************************************
 * @brief  get a string like '03:18:25' from u64 ts
 * @param  char time_str[]: where to store
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void _get_formatted_clock_string(char time_str[], UINT64 ms) {
    int total_seconds = (int)(ms / 1000);
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    // char time_str[25] = { 0 };
    sprintf_s(time_str, 9, "%02d:%02d:%02d", hours, minutes, seconds);
}


/*******************************************************************************
******************************* Public Variables *******************************
*******************************************************************************/

/*******************************************************************************
******************************* Public functions *******************************
*******************************************************************************/

/*******************************************************************************
 * @brief  update variables in labelbox, Called by draw_all() in WndProc() Timer
 * @param  disp: the display structure
 * @param  run_ms: time since app starts
 * @return xxxx
 *******************************************************************************/
void ui31_draw_labelbox1(HDC hdc, DISP_STRU* disp, UINT64 run_ms) {
    char buf[LABEL_STRING_SIZE]; // main text buffer which holds the entire string of a label window
    char time_str[20] = "";

    const char* format_str =
        "     --- [CAR SIMULATION] ---\r\n"
        "-Run: %s\r\n"
        "-Cmd: %s \r\n"
        "-APS Speed: %+05d/%04d(sp)mm/s\r\n"
        "-APS Height: %05dmm\r\n"
        "-Dir: %s\r\n"
        "-State: %s\r\n"
        "-Error: %s \r\n\n"
        "     --- [DOOR SIMULATION] ---\r\n"
        "-Cmd: %s\r\n"
        "-State: %s\n"
        "-Opening: %d%% \n\n"
        "-Enabled: %s, %s, %s.";

    //[1] format the display
    _get_formatted_clock_string(time_str, run_ms);

        sprintf_s(buf, format_str,
        time_str, //00:01:12
        sim31_get_cmd_string(disp->car1_cmd), // CMD_STOP
        disp->car1_speed,
        disp->car1_speed_sp,
        disp->car1_height,
        sim32_get_dir_string(disp->car1_direction),
        sim30_get_state_string(disp->car1_state),
        sim33_get_err_string(disp->car1_error),
        sim_door_get_cmd_string(disp->door1_cmd),
        sim_door_get_state_string(disp->door1_state),
        disp->door1_position,
        (disp->is_simulator_enable ? "APP" : "X-APP"),
        (disp->is_core_enable ? "CORE" : "X-CORE"),
        (disp->is_car_sim_enabled ? "SIM" : "X-SIM"));

    //[2] draw the label
    RECT labelRect1 = { UI_LABELBOX_X, UI_LABELBOX_Y, UI_LABELBOX_X + UI_LABELBOX_W, UI_LABELBOX_Y + UI_LABELBOX_H };
    extern HFONT g_hfont9_Consolas;
    SelectObject(hdc, g_hfont9_Consolas);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0)); // black
    Rectangle(hdc, labelRect1.left, labelRect1.top, labelRect1.right, labelRect1.bottom);
    DrawTextA(hdc, buf, -1, &labelRect1, DT_LEFT | DT_TOP | DT_NOPREFIX);
}

void ui31_draw_labelbox2(HDC hdc, DISP_STRU* disp) {
    char buf[LABEL_STRING_SIZE];  // holds the entire string
    char buf2[LABEL_STRING_SIZE]; // hold part of label string

    //[1.1] show register table
    sprintf_s(buf, "     --- [CORE SERVICE] ---\r\n-Calls (cnt=%d):\n", disp->cnt_total_calls); // call landing cnt);

    _print_binary_array(buf2, 100, disp->calltable);
    strncat_s(buf, LABEL_STRING_SIZE, buf2, LABEL_STRING_SIZE / 2); // add buf2's content into buf1

    //[1.2] Show simplex status
    const char* format_str1 = 
        "\r\n-Landing: %d (%.2f)--> Dest:%d\r\n"
        "-Lv1: %s, \r\n"
        "-Lv2: %s, \r\n"
        "-Lv3: %s, \r\n"
        "-Lv4: %s, \r\n"
        "-Lv5: %s";

    sprintf_s(buf2, format_str1,
        disp->current_landing,
        disp->current_landing_f,
        disp->determined_next_landing,
        fsm_lv1_get_state_string(disp->lv1_state),
        core_interface_get_lv2_state_string(disp->lv2_state),
        fsm_lv3_get_state_string(disp->lv3_state),
        fsm_lv4_get_state_string(disp->lv4_state),
        lv3_get_door_state_name(disp->lv5_state));
    strncat_s(buf, LABEL_STRING_SIZE, buf2, LABEL_STRING_SIZE / 2); // add buf2's content into buf1

    //[1.3] show group contorl data
    if (disp->car_mode == ELEVATOR_MODE_MASTER) {
        const char* format_str2 =
            "\r\n\n     --- [SLAVE1] ---\r\n"
            "-State : %s\r\n"
            "-Landing: %d\r\n"
            "-Door: %s\r\n"
            "-Service: [%d, %s, %d].";
        sprintf_s(buf2, format_str2,
            (disp->slave1_is_idle ? "IDLE" : "SERVING"),
            disp->slave1_current_landing,
            lv3_get_door_state_name(disp->slave1_door_state),
            disp->slave1_range2_a,
            sim32_get_dir_string(disp->slave1_range2_dir),
            disp->slave1_range2_b);
        strncat_s(buf, LABEL_STRING_SIZE, buf2, LABEL_STRING_SIZE / 2); // add buf2's content into buf1
    }

    //[2] draw the lable
    { // draw label2 text// SetWindowTextA(hLabel2, buf);
        RECT labelRect2 = { UI_LABELBOX2_X, UI_LABELBOX2_Y, UI_LABELBOX2_X + UI_LABELBOX2_W, UI_LABELBOX2_Y + UI_LABELBOX2_H };
        Rectangle(hdc, labelRect2.left, labelRect2.top, labelRect2.right, labelRect2.bottom);
        DrawTextA(hdc, buf, -1, &labelRect2, DT_LEFT | DT_TOP | DT_NOPREFIX);
    }
}


/********************************* end of file ********************************/

