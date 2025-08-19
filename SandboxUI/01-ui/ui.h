/**
********************************************************************************
* @file    ui.h
* @author  Arthur
* @version V1.0
* @date    2025-07-31 16:29:50
* @brief   Public functions and datatypes from this function
*
*  
********************************************************************************
* @attention
*
*
********************************************************************************
*/
/*******************************************************************************
********************* Define to prevent recursive inclusion ********************
*******************************************************************************/
#ifndef __UI_H
#define __UI_H
/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/
#include <Windows.h>
#include "button_id.h"
#include "../00-app/top_config.h"
#include "../00-app/top_datatype.h"
#include "ui_config.h"
// clang-format off


/*******************************************************************************
********************************* Exported macro *******************************
*******************************************************************************/

/*******************************************************************************
********************************* Exported types *******************************
*******************************************************************************/



//[1] outer layer, other module data sends to ui
typedef struct {
    //[Simulator: car1 and door1]
    int         car1_cmd;
    int         car1_speed_sp; // mm/s
    int         car1_speed;    // mm/s
    int         car1_height;   // mm, -1000... 30000
    int         car1_direction;
    int         car1_state;
    int         car1_error;
    int         car1_floor; // current floor of car1, 1...n

    int         door1_cmd;      // door simulation input
    int         door1_state;    // to see door simulation output
    int         door1_position; // [0, 100], 0=Fully Closed, 100=Fully 0pen

    //[App Control]
    int         is_core_enable;      
    int         is_car_sim_enabled;  
    int         is_simulator_enable; 

    //[Core]
    int         car_mode;//0=simplex, 1=master, 2=slave1
    int         lv1_state;
    int         lv2_state; // lv2 ENUM_MODE
    int         lv3_state;
    int         lv4_state;
    int         lv5_state; // for door control state    
    int         calltable[APP_FLOOR_NUM][3];
    int         current_landing;     // 0=not at a landing, 1..N the landing number
    float       current_landing_f; // float type current landing, display like '3.85'
    int         determined_next_landing; //current target landing
    int         cnt_total_calls;

    //[Group Control] for master to display slave data
    int         slave1_is_idle;
    int         slave1_current_landing;
    int         slave1_door_state;
    int         slave1_range2_dir;
    int         slave1_range2_a;
    int         slave1_range2_b;

} DISP_STRU;


//[2] outer layer, data other module gets data from ui
typedef struct {
    int cnt;
    DIO_STRUCT pin[TOTAL_BUTTION_NUM]; //all the input pins, each has an unique pin id

} UI_INPUT_STRU;



/*******************************************************************************
******************************* Exported constants *****************************
*******************************************************************************/

/*******************************************************************************
*************************** Exported global variables **************************
*******************************************************************************/

extern DISP_STRU disp_stru;

/*******************************************************************************
******************************* Exported functions *****************************
*******************************************************************************/

void ui_callback_type_lock_step1(HWND hwnd, int id);
void ui_callback_type_lock_step2(LPDRAWITEMSTRUCT lpDrawItem);
void ui_callback_type_led(LPDRAWITEMSTRUCT lpDrawItem);
void ui_callback_type_radio(int id);
void ui_callback_type_click(int id);
LRESULT CALLBACK ui_callback_type_continuous(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);


void ui2_init(HWND hwnd);
void ui3_draw_all(HDC hdc);
void ui4_test_loop_example();





// clang-format on

#endif /* __UI_H */
/********************************* end of file ********************************/

