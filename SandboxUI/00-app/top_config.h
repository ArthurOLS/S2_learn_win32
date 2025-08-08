/**
********************************************************************************
* @file    top_config.h
* @author  Arthur
* @version V1.0
* @date    2025-08-01 09:54:05
* @brief   Top level config for the whole project
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
#ifndef __TOP_CONFIG_H
#define __TOP_CONFIG_H
/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/
//nothing needs to be included in this file, this is on the top. 

/*******************************************************************************
********************************* Exported macro *******************************
*******************************************************************************/

/*******************************************************************************
********************************* Exported types *******************************
*******************************************************************************/

/*******************************************************************************
******************************* Exported constants *****************************
*******************************************************************************/

#define VERSION_CODE                    "2.0.2"
#define APP_MAIN_TITLE                  L"SandboxUI"


//clang-format off



#define VERSION_CODE "2.0.2"

// Elevator mode for the title bar
typedef enum {
    ELEVATOR_MODE_SIMPLEX = -1,
    ELEVATOR_MODE_MASTER = 0,
    ELEVATOR_MODE_SLAVE1 = 1,
    ELEVATOR_MODE_SLAVE2 = 2,
    ELEVATOR_MODE_SLAVE3 = 3,
} ELEVATOR_MODE_ENUM;

// Title texts
#define APP_MAIN_TITLE L"ElevatorSimulatorV2a"
#define APP_SUB_TITLE_SIMPLEX L"-Simplex"
#define APP_SUB_TITLE_MASTER L"-Master"
#define APP_SUB_TITLE_SLAVE1 L"-Slave1"
#define APP_SUB_TITLE_SLAVE2 L"-Slave2"
#define APP_SUB_TITLE_SLAVE3 L"-Slave3"
#define APP_SUB_TITLE_SLAVE4 L"-Slave4"  

// Simulation parameters (mm, Sec)

#define APP_FLOOR_NUM 8                // 2~8, if you want to have more than 9 floors, you need to modify ENUM_BUTTON_ID as well as g_button_id_stru_list[] in ui.cpp
#define APP_FLOOR_HEIGHT 3000          // mm
#define APP_PERIOD_MS 20               // ms, this is also the timer for Core.
#define CORE_PERIOD_MS (APP_PERIOD_MS) // heartbeat time for CORE
#define APP_CAR_NUM 2                  // max two car1 for group control
// Timer used in this program
#define IDT_TIMER_UI 1        // for UI module
#define IDT_TIMER_SIMULATOR 2 // for simulator module

/******************************************************************************
 * Core controller parameters
 ******************************************************************************/
// Motion Controlling
#define MOTION_INTECEPTION_DIST_MIN (0.8f) // The car can perform interception when it's this distance from stop point
// Door control
#define DOOR_STEP_WAIT1_MS (500)    // in ms
#define DOOR_STEP_HOLDING_MS (2000) // Door open wait time in MS


// Car speed presetting for button in manual mode.
#define DEF_CAR_SPEED1 200  // mm/s
#define DEF_CAR_SPEED2 1000 // mm/s
#define DEF_CAR_SPEED3 2000 // mm/s


//clang-format on

#endif /* __TOP_CONFIG_H */
/********************************* end of file ********************************/

