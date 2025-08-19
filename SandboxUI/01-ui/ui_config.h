/**
********************************************************************************
* @file    ui_define.h
* @author  Arthur
* @version V1.0
* @date    2025-08-18 11:41:54
* @brief   definition for ui layout
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
#ifndef __UI_DEFINE_H
#define __UI_DEFINE_H
/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/
#include "../00-app/top_config.h"


/*******************************************************************************
********************************* Overall Settings *****************************
*******************************************************************************/


// main window
#define WINDOWS_START_X                 0
#define WINDOWS_START_Y                 0

#define WINDOWS_WIDTH                   1525
#define WINDOWS_HEIGHT                  (1000-35)

#define UI_PERIOD_MS                    30   // refresh ui data and display every 50ms

#define UI_DRAW_DEBUG_RED_BOX           0

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

//output device 
#define UI_VERSION_X                    COLUMN1_X
#define UI_VERSION_Y                    COLUMN_TOP_Y    
#define UI_VERSION_W                    200
#define UI_VERSION_H                    40//(COLUMN_BOTTOM_Y - COLUMN_TOP_Y)

//animation
#define UI_ANIMATION_X                  COLUMN1_X
#define UI_ANIMATION_Y                  (UI_VERSION_Y + UI_VERSION_H+ UI_GAP + UI_GAP) //add 2X gap above 
#define UI_ANIMATION_W                  200
#define UI_ANIMATION_H                  (COLUMN_BOTTOM_Y - UI_ANIMATION_Y)

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
#define UI_COP2_H                       235    

//cop3
#define UI_COP3_X                       COLUMN5_X
#define UI_COP3_Y                       (UI_COP2_Y+UI_COP2_H+UI_GAP)
#define UI_COP3_W                       190
#define UI_COP3_H                       195

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

/******************************************************************************
* For all Buttons
******************************************************************************/
#define UI_BUTTON_W                     85
#define UI_BUTTON_W_WIDE                160
#define UI_BUTTON_H                     23
#define UI_BUTTON_GAP_ROW               8 
#define UI_BUTTON_GAP_COLUMN            5//universal button size

/******************************************************************************
 * BLOCK ANIMATION
 ******************************************************************************/
#define UI_PIXEL_RATIO                  50 // pixel = length(in mm) * RATIO. I adopt: 1px=50mm, 60px=3000mm for a floor height
#define UI_GROUND_Y                     (UI_ANIMATION_Y+(SIM_SHAFT_LENGTH/UI_PIXEL_RATIO)+20)//(UI_ANIMATION_Y + UI_ANIMATION_H - 40)   // set where the ground (physical 0mm horizontal line)
#define UI_COLOR_GREEN                  0x00A000// 0xBBGGRR
#define UI_COLOR_RED                    0x0000FF// 0xBBGGRR
#define UI_COLOR_BLUE                   0xFFBF00// 0xBBGGRR
#define UI_COLOR_ORANGE                 0x00A5FF// 0xBBGGRR 

///used in ui_animation.cpp
// Shaft boxes
#define UI_SHAFT_X                      50
#define UI_SHAFT_W                      50
#define UI_FLOOR_H                      60
#define UI_FLOOR_LABEL_X                20 
// Final limits
#define UI_LIMIT_X                      110
#define UI_LIMIT_Y_BOTTOM               (UI_GROUND_Y - SIM_SHAFT_FINAL_BOTTOM / UI_PIXEL_RATIO)
#define UI_LIMIT_Y_TOP                  (UI_GROUND_Y - SIM_SHAFT_TOP_FINAL / UI_PIXEL_RATIO)
#define UI_LIMIT_CIRCLE_RADIUS          5 // pixel
// Car box
#define UI_CAR_X                        120
#define UI_CAR_W                        50
#define UI_CAR_H                        50
#define UI_CAR_FILL_COLOR               0x00d7ff // 0=No fill, other=color code in 0xBBGGRR, 0xffa080=pale blue, 0x00d7ff=gold
// Door boxes
#define UI_DOOR_W                       32
#define UI_DOOR_H                       40
#define UI_DOOR_FILL_COLOR              0x0000af // in 0xBBGGRR
// Ground line (horizon)
#define UI_GROUND_LINE_X_LEFT           0
#define UI_GROUND_LINE_X_RIGHT          (UI_GROUND_LINE_X_LEFT + 150) // full width of the block

/*******************************************************************************
********************************* Exported macro *******************************
*******************************************************************************/

/*******************************************************************************
********************************* Exported types *******************************
*******************************************************************************/

/*******************************************************************************
******************************* Exported constants *****************************
*******************************************************************************/

/*******************************************************************************
*************************** Exported global variables **************************
*******************************************************************************/

/*******************************************************************************
******************************* Exported functions *****************************
*******************************************************************************/



#endif /* __UI_DEFINE_H */
/********************************* end of file ********************************/

