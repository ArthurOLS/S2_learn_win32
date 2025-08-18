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
#include <stdio.h>
#include <commctrl.h> // For SetWindowSubclass, DefSubclassProc
#pragma comment(lib, "comctl32.lib")//DefSubclassProc is not part of the core Windows API (windows.h), but part of the Common Controls Library; This library provides the actual implementation of functions declared in commctrl.h.


#include "../00-app/top_config.h"
#include "ui.h"
#include "ui_lowlevel.h"
#include "ui_logbox.h"
#include "ui_animation.h"

/*******************************************************************************
******************************** Private typedef *******************************
*******************************************************************************/


//inner layer, the contorl data ui uses itself
typedef struct {
    uint32_t ui_timer_cnt; //add in ui_test_loop()
    int         run_cnt; // how many times ui has refreshed
    int         car_box_y;          // the y position of cab box bottom line.
    int         door_opening_width; // 0..40 pix width
    uint32_t    ts_at_start;   // recorded ts at starting up time by ui82_c_get_u_run_ms().
    uint32_t    run_ms; //= now_ms() - ms_at_start
    int         car_mode;           // 0=simplex, 1=master, 2=slave1


} UI_CONTROL_STRU;

/*******************************************************************************
******************************** Private define ********************************
*******************************************************************************/

/*******************************************************************************
********************************* Private macro ********************************
*******************************************************************************/


//#define FILL_BUTTON_ID(name)          { #name, name, 0 }

#define ui_record_pin(ID, level) ui_dio_set_value(&(ui_input.pin[ID]), level);

#define UI_RECORD_CLICK_PIN(ID) \
    {                                          \
        ui_input.pin[ID].value = 1;            \
        ui_input.pin[ID].value_last_cycle = 0; \
    }

//#define UI_DIO_SET_NAME_AS_ITS_ID(ID) { ui_input.pin[ID].name = #ID;}
#define UI_DIO_SET_NAME_AS_ITS_ID(ID, VAL) \
    {                                      \
        ui_input.pin[ID].name = #ID;       \
        ui_input.pin[ID].value = VAL;      \
    }

/*******************************************************************************
******************************* Private variables ******************************
*******************************************************************************/
UI_CONTROL_STRU _ui_control_stru;
UI_INPUT_STRU ui_input;
DISP_STRU disp_stru;

/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/

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
        case D1_CLOSE:
        case ID_COP2_UP:
        case ID_COP2_DOWN:
        case ID_TOC_UP:
        case ID_TOC_DOWN:
        case ID_MACHINEROOM_UP:
        case ID_MACHINEROOM_DOWN:
        case ID_COP3_OPEN_DOOR:
        case ID_COP3_CLOSE_DOOR:
            ui_record_pin(bid, 1);
            ui_internal_printf("continuous-type: %s=1.", ui_input.pin[bid].name);
            break;


        default:break;
        }
        break;

    case WM_LBUTTONUP:
        switch (bid) {
        case D1_OPEN:
        case D1_CLOSE:
        case ID_COP2_UP:
        case ID_COP2_DOWN:
        case ID_TOC_UP:
        case ID_TOC_DOWN:
        case ID_MACHINEROOM_UP:
        case ID_MACHINEROOM_DOWN:
        case ID_COP3_OPEN_DOOR:
        case ID_COP3_CLOSE_DOOR:
            ui_record_pin(bid, 0);
            ui_internal_printf("continuous-type: %s=0.", ui_input.pin[bid].name);
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
 * @brief  called by WM_DRAWITEM, originally triggered by InvalidateRect()
 * @param  lpDrawItem: which button to redraw
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_callback_type_led(LPDRAWITEMSTRUCT lpDrawItem) {
    int id = lpDrawItem->CtlID;
    switch (id) {

    case ID_MOTOR_EN:
        ui30_draw_button_led_color(lpDrawItem, NULL, true, UI_COLOR_ORANGE);
        break;

    case ID_BUZZER:
        ui30_draw_button_led_color(lpDrawItem, NULL, false, UI_COLOR_GREEN);
        break;

    case ID_EB_PICK:
        ui30_draw_button_led_color(lpDrawItem, NULL, true, UI_COLOR_GREEN);
        break;

    case ID_LATERN_UP:
        ui30_draw_button_led_color(lpDrawItem, NULL, false, UI_COLOR_GREEN);
        break;

    case ID_LATERN_DN:
        ui30_draw_button_led_color(lpDrawItem, NULL, true, UI_COLOR_RED);
        break;

    case ID_CHIME:
        ui30_draw_button_led_color(lpDrawItem, NULL, true, UI_COLOR_BLUE);
        break;

    default:
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
    case ID_COP2_ENABLE:
    case ID_TOC_ENABLE:
        ui_input.pin[id].value = (ui_input.pin[id].value == 0) ? 1 : 0;
        ui_internal_printf("lock-type pin: %s=%d", ui_input.pin[id].name, ui_input.pin[id].value);
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
 * @note   works as drawing leds.
 *******************************************************************************/
void ui_callback_type_lock_step2(LPDRAWITEMSTRUCT lpDrawItem) {
    int id = lpDrawItem->CtlID;
    switch (id) {

    case ID_MACHINEROOM_ENABLE:
    case ID_COP2_ENABLE:
    case ID_TOC_ENABLE:
        ui30_draw_button_led_black(lpDrawItem, NULL, ui_input.pin[id].value);
        break;

    default:
        break;
    }
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
    case ID_HOP2_FIRE_RECALL_RESET:
        ui_internal_printf("switch to 'Fire PH1-RESET'.");
        ui_record_pin(ID_HOP2_FIRE_RECALL, 0);
        break;
    case ID_HOP2_FIRE_RECALL_OFF:
        ui_internal_printf("switch to 'Fire PH1-OFF'.");
        ui_record_pin(ID_HOP2_FIRE_RECALL, 1);
        break;
    case ID_HOP2_FIRE_RECALL_ON:
        ui_internal_printf("switch to 'Fire PH1-ON'.");
        ui_record_pin(ID_HOP2_FIRE_RECALL, 2);
        break;

    case ID_HOP2_INSP_TOP_UP:
        ui_internal_printf("switch to 'HOP2/INSP TOP/UP'.");
        ui_record_pin(ID_HOP2_INSP_TOP, 0);
        break;
    case ID_HOP2_INSP_TOP_OFF:
        ui_internal_printf("switch to 'HOP2/INSP TOP/OFF'.");
        ui_record_pin(ID_HOP2_INSP_TOP, 1);
        break;
    case ID_HOP2_INSP_TOP_DOWN:
        ui_internal_printf("switch to 'HOP2/INSP TOP/DOWN'.");
        ui_record_pin(ID_HOP2_INSP_TOP, 2);
        break;


    case ID_HOP2_INSP_BOTTOM_UP:
        ui_internal_printf("switch to 'HOP2/INSP BOTTOM/UP'.");
        ui_record_pin(ID_HOP2_INSP_BOTTOM, 0);
        break;
    case ID_HOP2_INSP_BOTTOM_OFF:
        ui_internal_printf("switch to 'HOP2/INSP BOTTOM/OFF'.");
        ui_record_pin(ID_HOP2_INSP_BOTTOM, 1);
        break;
    case ID_HOP2_INSP_BOTTOM_DOWN:
        ui_internal_printf("switch to 'HOP2/INSP BOTTOM/DOWN'.");
        ui_record_pin(ID_HOP2_INSP_BOTTOM, 2);
        break;

    case ID_MACHINEROOM_INSP_ON:
        ui_internal_printf("switch to 'MACHINEROOM/INSP/ON'.");
        ui_record_pin(ID_MACHINEROOM_INSP, 0);
        break;    
    case ID_MACHINEROOM_INSP_OFF:
        ui_internal_printf("switch to 'MACHINEROOM/INSP/OFF'.");
        ui_record_pin(ID_MACHINEROOM_INSP, 1);
        break;

    case ID_MACHINEROOM_CAR_DOOR_BYPASS_ON:
        ui_internal_printf("switch to 'M.R./CAR DOOR BYPASS/ON'.");
        ui_record_pin(ID_MACHINEROOM_CAR_DOOR_BYPASS, 0);
        break;
    case ID_MACHINEROOM_CAR_DOOR_BYPASS_OFF:
        ui_internal_printf("switch to 'M.R./CAR DOOR BYPASS/OFF'.");
        ui_record_pin(ID_MACHINEROOM_CAR_DOOR_BYPASS, 1);
        break;

    case ID_MACHINEROOM_HALL_DOOR_BYPASS_ON:
        ui_internal_printf("switch to 'M.R./HALL DOOR BYPASS/ON'.");
        ui_record_pin(ID_MACHINEROOM_HALL_DOOR_BYPASS, 0);
        break;
    case ID_MACHINEROOM_HALL_DOOR_BYPASS_OFF:
        ui_internal_printf("switch to 'M.R./HALL DOOR BYPASS/OFF'.");
        ui_record_pin(ID_MACHINEROOM_HALL_DOOR_BYPASS, 1);
        break;    
    
    //cop2
    case ID_COP2_HOIST_ACCESS_ON:
        ui_internal_printf("switch to 'COP2/HOIST.ACCESS/ON'.");
        ui_record_pin(ID_COP2_HOIST_ACCESS, 0);
        break;
    case ID_COP2_HOIST_ACCESS_OFF:
        ui_internal_printf("switch to 'COP2/HOIST.ACCESS/OFF'.");
        ui_record_pin(ID_COP2_HOIST_ACCESS, 1);
        break;


    case ID_COP2_IND_ON:
        ui_internal_printf("switch to 'COP2/IND. SERVICE/ON'.");
        ui_record_pin(ID_COP2_IND, 0);
        break;
    case ID_COP2_IND_OFF:
        ui_internal_printf("switch to 'COP2/IND. SERVICE/OFF'.");
        ui_record_pin(ID_COP2_IND, 1);
        break;
            

    case ID_COP2_RUN_STOP:
        ui_internal_printf("switch to 'COP2/RUN/STOP'.");
        ui_record_pin(ID_COP2_RUN, 0);
        break;    
    case ID_COP2_RUN_RUN:
        ui_internal_printf("switch to 'COP2/RUN/RUN'.");
        ui_record_pin(ID_COP2_RUN, 1);
        break;

    //COP3
    case ID_COP3_FIRE_PH2_OFF:
        ui_internal_printf("switch to 'COP3/PH2/OFF'.");
        ui_record_pin(ID_COP3_FIRE_PH2, 0);
        break;
    case ID_COP3_FIRE_PH2_HOLD:
        ui_internal_printf("switch to 'COP3/PH2/HOLD'.");
        ui_record_pin(ID_COP3_FIRE_PH2, 1);
        break;            
    case ID_COP3_FIRE_PH2_ON:
        ui_internal_printf("switch to 'COP3/PH2/ON'.");
        ui_record_pin(ID_COP3_FIRE_PH2, 2);
        break;

    case ID_COP3_RUN_STOP:
        ui_internal_printf("switch to 'COP3/RUN/STOP'.");
        ui_record_pin(ID_COP3_RUN, 0);
        break;
    case ID_COP3_RUN_RUN:
        ui_internal_printf("switch to 'COP3/RUN/RUN'.");
        ui_record_pin(ID_COP3_RUN, 1);
        break;



    //TOC
    case ID_TOC_INSP_ON:
        ui_internal_printf("switch to 'TOC/INSP./ON'.");
        ui_record_pin(ID_TOC_INSP, 0);
        break;
    case ID_TOC_INSP_OFF:
        ui_internal_printf("switch to 'TOC/INSP./OFF'.");
        ui_record_pin(ID_TOC_INSP, 1);
        break;

    case ID_TOC_RUN_STOP:
        ui_internal_printf("switch to 'TOC/RUN/STOP'.");
        ui_record_pin(ID_TOC_RUN, 0);
        break;    
    case ID_TOC_RUN_RUN:
        ui_internal_printf("switch to 'TOC/RUN/RUN'.");
        ui_record_pin(ID_TOC_RUN, 1);
        break;


    // case X:
    //     ui_internal_printf("switch to 'X'.");
    //     ui_record_pin(X, X);
    //     break;
    default:
        break;
    }
}



/*******************************************************************************
 * @brief  for the common click buttons, called by WndProc on WM_COMMAND event
 * @param  id: from LOWORD(wParam)
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_callback_type_click(int id) {
    if (H1UP <= id && id <= CF12) {
        UI_RECORD_CLICK_PIN(id); //simulate a pin level change
    }
    if (id == ID_SHOW_IO_LIST) {
    #define BUFFER_SIZE 1000
        char buf[BUFFER_SIZE] = "Showing IO list:";
        //show all io pins status in logbox
        for (int i = 0; i < TOTAL_BUTTION_NUM; i++) {
            char temp[16] = "";
            if (ui_input.pin[i].value != 0) {
                sprintf_s(temp, "%d", i);
                strncat_s(buf, BUFFER_SIZE, temp, 16);
            }


            if ((i % 10) == 0) { // Add newline for every 4 up/down/car elements and not at end
                strncat_s(buf, BUFFER_SIZE, "\r\n", 2);
            }
            else {
                strncat_s(buf, BUFFER_SIZE, ", ", 2);
            }
        }
        ui_internal_printf("%s", buf);
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
    extern void ui_create_led_output(HWND hwnd, int gx, int gy, int bw, int bh);
    extern void ui_create_button_hop(HWND hwnd, int gx, int gy, int bw, int bh);
    extern void ui_create_button_cop1(HWND hwnd, int gx, int gy, int bw, int bh);
    extern void ui_create_cop2(HWND hwnd, int x, int y);
    extern void ui_create_cop3(HWND hwnd, int x, int y);
    extern void ui_create_hop2(HWND hwnd, int x, int y);
    extern void ui_create_machineroom(HWND hwnd, int x, int y);
    extern void ui_create_toc(HWND hwnd, int x, int y);
    extern void ui_create_button_nonmanual(HWND hwnd, int gx, int gy);
    extern void ui_create_button_debug(HWND hwnd, int gx, int gy);

    ui_create_font9();

    //[1] create main window top layout
    char buf1[50];
    sprintf_s(buf1, "Version: %s\nBuild: %s, %s",  VERSION_CODE, __DATE__, __TIME__);
    ui11_create_label(hwnd, buf1, UI_OUTPUT_X, UI_OUTPUT_Y, UI_OUTPUT_W, UI_OUTPUT_H);
    //ui11_create_label(hwnd, L" Simulator", UI_LABELBOX_X, UI_LABELBOX_Y, UI_LABELBOX_W, UI_LABELBOX_H);
    //ui11_create_label(hwnd, L" ElevatorCore", UI_LABELBOX2_X, UI_LABELBOX2_Y, UI_LABELBOX2_W, UI_LABELBOX2_H);
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

    //[2]create each block
    //leds
    //ui_create_led_output(hwnd, UI_OUTPUT_X + 7, UI_OUTPUT_Y + 20, UI_BUTTON_W, UI_BUTTON_H);
    //hop
    ui_create_button_hop(hwnd, UI_HOP_X + 7, UI_HOP_Y + 20, UI_BUTTON_W, UI_BUTTON_H);
    //cop1
    ui_create_button_cop1(hwnd, UI_COP1_X + 7, UI_COP1_Y + 20, UI_BUTTON_W, UI_BUTTON_H);
    //logbox
    ui_create_logbox(hwnd, UI_LOGBOX_X, UI_LOGBOX_Y, UI_LOGBOX_W, UI_LOGBOX_H, 0);
    RECT rc;
    GetClientRect(hwnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    ui_internal_printf("client area window size w=%d,h=%d.", width, height);

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

    //non manual
    ui_create_button_nonmanual(hwnd, UI_NONMANUAL_X, UI_NONMANUAL_Y);

    //debug
    ui_create_button_debug(hwnd, UI_DEGUG_X, UI_DEGUG_Y);

    //[3] UI timer
    SetTimer(hwnd, IDT_TIMER_UI, UI_PERIOD_MS, NULL);                       // 50ms(20Hz timer)
    _ui_control_stru.ts_at_start = static_cast<uint32_t>(GetTickCount64()); // ui82_c_get_u_run_ms();

    //type-continuous
    UI_DIO_SET_NAME_AS_ITS_ID(D1_OPEN, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(D1_CLOSE, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP2_UP, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP2_DOWN, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP3_OPEN_DOOR, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP3_CLOSE_DOOR, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_TOC_UP, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_TOC_DOWN, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_MACHINEROOM_UP, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_MACHINEROOM_DOWN, 0);
    
    //type-lock, 3 in total
    UI_DIO_SET_NAME_AS_ITS_ID(ID_MACHINEROOM_ENABLE, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP2_ENABLE, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_TOC_ENABLE, 0);
    //radio groups
    UI_DIO_SET_NAME_AS_ITS_ID(ID_HOP2_FIRE_RECALL, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_HOP2_INSP_TOP, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_HOP2_INSP_BOTTOM, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_MACHINEROOM_INSP, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_MACHINEROOM_CAR_DOOR_BYPASS, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_MACHINEROOM_HALL_DOOR_BYPASS, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP2_HOIST_ACCESS, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP2_IND, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP2_RUN, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP3_FIRE_PH2, 0);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_COP3_RUN, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_TOC_INSP, 1);
    UI_DIO_SET_NAME_AS_ITS_ID(ID_TOC_RUN, 1);

}



#if 1
/*******************************************************************************
 * @brief  update two labelboxes, Called by draw_all() in WndProc() Timer
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui31_draw_labels(HDC hdc, DISP_STRU* disp, UINT64 ms) {
#define LABEL_STRING_SIZE 512 // lable printf buzzer size

    char buf[LABEL_STRING_SIZE];  // main text buffer which holds the entire string of a label window
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

        extern HFONT hFont9;
        SelectObject(hdc, hFont9);
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

int car_y_px = UI_GROUND_Y;
bool car_is_idle = true;

/*******************************************************************************
 * @brief  called in WM_PAINT event by WndProc()
 * @param  hdc, where to draw
 * @return xxxx
 *******************************************************************************/
void ui03_draw_all(HDC hdc) {


    _ui_control_stru.run_cnt++;

    ui_draw_floors(hdc, UI_ANIMATION_X, UI_GROUND_Y);
    ui34_draw_final_limits(hdc, UI_ANIMATION_X);
    ui31_draw_labels(hdc, &disp_stru, _ui_control_stru.run_cnt);
    ui32_draw_cab_box(hdc, UI_ANIMATION_X, _ui_control_stru.car_box_y, car_is_idle);
    ui36_draw_door(hdc, UI_ANIMATION_X, _ui_control_stru.car_box_y, _ui_control_stru.door_opening_width);

}




/*******************************************************************************
 * @brief  this is a test example show how to process events.
 * @note   called by UI timer
 * @return xxxx
 *******************************************************************************/
void ui_test_loop() {
    _ui_control_stru.ui_timer_cnt++;
    ui_input_process_1of2();//events are generated here

    //your code to read the events and process your business logics
    //  update all pixel data for all the ui_draw_xxx() functions.
    // called by IDT_TIMER_UI event in WndProc().
    // void ui02_update_ui_model_data_in_timer(DISP_STRU * d)
    {

        // update cab position pixel
        // int positionMM = disp_stru.car1_height;
        {
            disp_stru.car1_height += 20;
            if (disp_stru.car1_height > SIM_SHAFT_TOP_FINAL) {
                disp_stru.car1_height = SIM_SHAFT_FINAL_BOTTOM;
            }
        }
        _ui_control_stru.car_box_y = ui_calc_car_y_pix(disp_stru.car1_height);

        // update door position
        {
            disp_stru.door1_position += 1;
            if (disp_stru.door1_position > 100) {
                disp_stru.door1_position = 0;
            }
        }
        _ui_control_stru.door_opening_width = ui_calc_door_opening(disp_stru.door1_position);
    }

    ui_input_process_2of2();//update last value for next cycle
}


/*******************************************************************************
 * @brief  two-step event processing
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_input_process_1of2(void) {
    int cnt_event_this_cycle = 0;
    for (int i = 0; i < TOTAL_BUTTION_NUM; i++) {
        if(ui_input.pin[i].value_last_cycle != ui_input.pin[i].value){
            ui_input.pin[i].event_flag_this_cycle = 1;
            ui_input.pin[i].event_cnt++;
            cnt_event_this_cycle++;
            ui_internal_printf("---Event id=%d", i);
        }
    }
    if (cnt_event_this_cycle>0) {
        ui_internal_printf("---sees %d events in loop %d.", cnt_event_this_cycle, _ui_control_stru.ui_timer_cnt);
    }
}
void ui_input_process_2of2(void) {
    for (int i = 0; i < TOTAL_BUTTION_NUM; i++) {
        ui_input.pin[i].value_last_cycle = ui_input.pin[i].value;
    }
}





/*******************************************************************************
 * @brief  get ui run tim in ms, it use the global ts_at_start variable
 * @param  xxxx
 * @return ms since app started
 * @note   overflows after 500 million years
 *******************************************************************************/
UINT64 ui64_get_ui_run_ms() {
    UINT64 ts = (int)GetTickCount64() - _ui_control_stru.ts_at_start;
    return ts; // overflow after ~49 days
}


/********************************* end of file ********************************/

