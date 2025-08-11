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
// clang-format off

/*******************************************************************************
********************************* Overall Settings *****************************
*******************************************************************************/
// main window
#define WINDOWS_START_X                 0
#define WINDOWS_START_Y                 0

#define WINDOWS_WIDTH                   1525
#define WINDOWS_HEIGHT                  1000-35

#define UI_PERIOD_MS                    30   // refresh ui data and display every 50ms

/*******************************************************************************
********************************* Higher layouts   *****************************
*******************************************************************************/
#define COLUMN1_X                       10
#define COLUMN2_X                       220
#define COLUMN3_X                       470    
#define COLUMN4_X                       720
#define COLUMN5_X                       920
#define COLUMN6_X                       1120
#define COLUMN7_X                       1320

#define COLUMN_TOP_Y                    0
#define COLUMN_BOTTOM_Y                 900 //the y inside the client area, it's ~100 smaller than WINDOWS_HEIGHT

#define UI_GAP                          10  //for columns and rows

//animation
#define UI_ANIMATION_X                  COLUMN1_X
#define UI_ANIMATION_Y                  COLUMN_TOP_Y    
#define UI_ANIMATION_W                  200
#define UI_ANIMATION_H                  (COLUMN_BOTTOM_Y - COLUMN_TOP_Y)

//label box
#define UI_LABELBOX_X                   COLUMN2_X
#define UI_LABELBOX_Y                   COLUMN_TOP_Y
#define UI_LABELBOX_W                   240
#define UI_LABELBOX_H                   260 

#define UI_LABELBOX2_X                  COLUMN3_X
#define UI_LABELBOX2_Y                  COLUMN_TOP_Y
#define UI_LABELBOX2_W                  240
#define UI_LABELBOX2_H                  260

//logbox
#define UI_LOGBOX_X                     COLUMN2_X
#define UI_LOGBOX_Y                     (COLUMN_TOP_Y + UI_LABELBOX_H + UI_GAP)
#define UI_LOGBOX_W                     490
#define UI_LOGBOX_H                     (COLUMN_BOTTOM_Y - UI_LOGBOX_Y)

//column4
//hops
#define UI_HOP_X                        COLUMN4_X
#define UI_HOP_Y                        COLUMN_TOP_Y
#define UI_HOP_W                        190
#define UI_HOP_H                        440

//hop2
#define UI_HOP2_X                       COLUMN4_X
#define UI_HOP2_Y                       (UI_HOP_Y+UI_HOP_H+UI_GAP)
#define UI_HOP2_W                       190
#define UI_HOP2_H                       180

//machineroom
#define UI_MACHINEROOM_X                COLUMN4_X
#define UI_MACHINEROOM_Y                (UI_HOP2_Y+UI_HOP2_H+UI_GAP)
#define UI_MACHINEROOM_W                190
#define UI_MACHINEROOM_H                (COLUMN_BOTTOM_Y-UI_MACHINEROOM_Y)

//column5
//cop1
#define UI_COP1_X                       COLUMN5_X
#define UI_COP1_Y                       COLUMN_TOP_Y
#define UI_COP1_W                       190
#define UI_COP1_H                       240

//cop2
#define UI_COP2_X                       COLUMN5_X
#define UI_COP2_Y                       (UI_COP1_Y+UI_COP1_H+UI_GAP)
#define UI_COP2_W                       190
#define UI_COP2_H                       240    

//cop3
#define UI_COP3_X                       COLUMN5_X
#define UI_COP3_Y                       (UI_COP2_Y+UI_COP2_H+UI_GAP)
#define UI_COP3_W                       190
#define UI_COP3_H                       190

//toc
#define UI_TOC_X                        COLUMN5_X
#define UI_TOC_Y                        (UI_COP3_Y+UI_COP3_H+UI_GAP)
#define UI_TOC_W                        (190)
#define UI_TOC_H                        200

//nonmanual
#define UI_NONMANUAL_X                  COLUMN6_X
#define UI_NONMANUAL_Y                  COLUMN_TOP_Y
#define UI_NONMANUAL_W                  190
#define UI_NONMANUAL_H                  (COLUMN_BOTTOM_Y - COLUMN_TOP_Y)

//debug
#define UI_DEGUG_X                      COLUMN7_X
#define UI_DEGUG_Y                      COLUMN_TOP_Y
#define UI_DEGUG_W                      190
#define UI_DEGUG_H                      (COLUMN_BOTTOM_Y - COLUMN_TOP_Y)

//universal button size
#define UI_BUTTON_WIDTH_SMALL           85
#define UI_BUTTON_WIDTH_WIDTH            160
#define UI_BUTTON_HEIGHT                23

/******************************************************************************
 * CAR COLUMN (LEFT1 IN MASTER)
 ******************************************************************************/
#define UI_PIXEL_RATIO                  50 // pixel = length(in mm) * RATIO. I adopt: 1px=50mm, 60px=3000mm for a floor height
#define UI_GROUND_Y                     550   // set where the ground (physical 0mm horizontal line)
// Shaft        
#define FLOOR_HEIGHT                    60
#define SHAFT_LEFT                      50 //(COLUMN1_X+50)
#define SHAFT_TOP                       20
#define SHAFT_WIDTH                     50
#define FLOOR_LABEL_LEFT                20        //(COLUMN1_X+20)
#define USE_COLORFUL_SHAFT_BOXES        0 // 0=use black boarders, 1=use color blocks
// Final limits
#define UI_LIMIT_LEFT                   110 //(COLUMN1_X+110)
#define UI_LIMIT_BOTTOM                 (UI_GROUND_Y - SIM_SHAFT_FINAL_BOTTOM / UI_PIXEL_RATIO)
#define UI_LIMIT_TOP                    (UI_GROUND_Y - SIM_SHAFT_TOP_FINAL / UI_PIXEL_RATIO)
#define UI_LIMIT_CIRCLE_RADIUS          5 // pixel
// Car
#define CAR_LEFT                        120 //(COLUMN1_X+120)
#define CAR_WIDTH                       50
#define CAR_HEIGHT                      50
#define UI_CAR_FILL_COLOR               0x00d7ff       // 0=No fill, other=color code in 0xBBGGRR, 0xffa080=pale blue, 0x00d7ff=gold
#define UI_CAR_FILL_COLOR_GREEN         0x00a000 // GREEN
#define UI_DOOR_FILL_COLOR              0x0000af      // in 0xBBGGRR
// Door
#define DOOR_WIDTH                      32
#define DOOR_HEIGHT                     40
#define DOOR_OPENING_WIDTH              31 // opens to 40 pixel, affects door_opening_width

// Ground line (horizon)
#define UI_GROUND_LINE_LEFT             3                            //(COLUMN1_X+3)       //left at x=0
#define UI_GROUND_LINE_RIGHT            (UI_GROUND_LINE_LEFT + 170) // right at x=200

/******************************************************************************
* BUTTON COLUMN (LEFT3 IN MASTER)
******************************************************************************/
//Buttons
#define UI_BUTTONS_LEFT                 0//(COLUMN3_LEFT)
#define UI_BUTTONS_TOP                  10      //group1
#define UI_BUTTONS_TOP2                 400-40  //group2
#define UI_BUTTON_WIDTH                 85
#define UI_BUTTON_HEIGHT                23
#define UI_BUTTON_GAP_ROW               5    //for group1
#define UI_BUTTON_HEIGHT2               23   //for group2
#define UI_BUTTON_GAP_ROW2              12   //for group2
#define UI_BUTTON_GAP_COLUMN            5
#define UI_BUTTON_PER_ROW               2    //3 buttons per row
#define UI_BUTTON_GROUP1_NUM            (3* SIM_FLOOR_NUM +2) //'2' for 'open door' and 'close door' on COP
#define UI_BUTTON_GROUP2_NUM            (3* 8)
#define UI_BUTTON_INVALID_ID            (BUTTON_BASE_ID+ SIM_FLOOR_NUM*3-3) //the last Nth button as F10UP is invalid, eg. N=10

/******************************************************************************
* LOGBOX
******************************************************************************/

//Logbox
#define UI_LOGBOX_LEFT                  0
#define UI_LOGBOX_TOP                   250
#define UI_LOGBOX_WIDTH                 490
#define UI_LOGBOX_HEIGHT                440
#define UI_LOGBOX_MAX_SAVE_BYTES        65536

/******************************************************************************
 * Label 1 and 2 COLUMN 
 ******************************************************************************/
#define UI_LABEL_LEFT                   COLUMN2_X // COLUMN2_X
#define UI_LABEL_WIDTH                  240
#define UI_LABEL_HEIGHT                 230
#define UI_LABEL_TOP                    10

#define UI_LABEL_LEFT2                  (10 + UI_LABEL_WIDTH) //(UI_LABEL_LEFT+10+UI_LABEL_WIDTH) //10 for the gap
#define LABEL_STRING_SIZE               512






// clang-format on
/******************************************************************************
 * Logbox
 ******************************************************************************/


/*******************************************************************************
********************************* UI local setting *****************************
*******************************************************************************/





/*******************************************************************************
********************************* Exported macro *******************************
*******************************************************************************/

/*******************************************************************************
********************************* Exported types *******************************
*******************************************************************************/

// outer layer, other module data sent to ui
typedef struct {
    int car1_cmd;
    int car1_speed_sp; // mm/s
    int car1_speed;    // mm/s
    int car1_height;   // mm, -1000... 30000
    int car1_direction;
    int car1_state;
    int car1_error;
    int car1_floor; // current floor of car1, 1...n

    int door1_cmd;      // door simulation input
    int door1_state;    // to see door simulation output
    int door1_position; // to see door simulation output

    int lv1_state;
    int lv2_state; // lv2 ENUM_MODE
    int lv3_state;
    int lv4_state;
    int lv5_state; // for door control state

    int is_core_enable;      // is this module enabled in app_run()
    int is_car_sim_enabled;  // is this module enabled in app_run()
    int is_simulator_enable; // for the whole app_run()

    int current_landing;     // 0=not at a landing, 1..N the landing number
    float current_landing_f; // float type current landing, display like '3.85'

    // for master to display slave data
    int slave1_is_idle;
    int slave1_current_landing;
    int slave1_door_state;
    int slave1_range2_dir;
    int slave1_range2_a;
    int slave1_range2_b;

} TO_UI_STRUCT;


// outer layer, data other module get data from ui
typedef struct {
    int i;
} FROM_UI_STRUCT;


// user ui widget model data
typedef struct {
    const wchar_t* text;
    int id;
    int is_clicked; // 0=not, 1=clicked, used when this button is also used as a led

} BUTTON_STRU;

/*******************************************************************************
******************************* Exported constants *****************************
*******************************************************************************/

/*******************************************************************************
*************************** Exported global variables **************************
*******************************************************************************/

/*******************************************************************************
******************************* Exported functions *****************************
*******************************************************************************/
void ui1_init_widgets(HWND hwnd);
void ui_callback_type_lock_step1(HWND hwnd, int id);
void ui_callback_type_lock_step2(LPDRAWITEMSTRUCT lpDrawItem);
void ui_callback_type_radio(int id);

//int ui64_get_ui_run_ms();




#endif /* __UI_H */
/********************************* end of file ********************************/

