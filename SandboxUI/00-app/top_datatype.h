/**
********************************************************************************
* @file    top_datatype.h
* @author  Arthur
* @version V1.0
* @date    2025-08-15 08:43:25
* @brief   top level datatype definition, project-wide
*
*
********************************************************************************
* @attention
*   can be included by any user files
*
********************************************************************************
*/
/*******************************************************************************
********************* Define to prevent recursive inclusion ********************
*******************************************************************************/
#ifndef __TOP_DATATYPE_H
#define __TOP_DATATYPE_H
/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/
#include <stdint.h>

/*******************************************************************************
********************************* Exported macro *******************************
*******************************************************************************/

/*******************************************************************************
********************************* Exported types *******************************
*******************************************************************************/

// for app(core) use
typedef struct {
    const char* name;
    int id;                    // defined in button_id.h
    int value;                 // 0=not active, 1=active
    int value_last_cycle;      // storee the value of last cycle, processed by button_thread();
    int event_flag_this_cycle; // 1=event that value turns from 0 to 1 event, processed by tutton_thread();
    int event_cnt;             // cnt for 'event_flag_this_cycle', processed by button_thread();

} DIO_STRUCT;


// typedef struct {
//     uint32_t app_thread_cnt;
//
//     uint64_t run_ms; //since start
//
// } MACHINE_TIME_STRU;

/*******************************************************************************
******************************* Exported constants *****************************
*******************************************************************************/


typedef enum {
    //[GROUP1] button id for passengers, in UP-DOWN-CAR order used by CORE.
    ID_01_UP = 0,
    ID_01_DOWN, // first floor DOWN is a dummy IO.
    ID_01_CAR,
    ID_02_UP,
    ID_02_DOWN,
    ID_02_CAR,
    ID_03_UP,
    ID_03_DOWN,
    ID_03_CAR,
    ID_04_UP,
    ID_04_DOWN,
    ID_04_CAR,
    ID_05_UP,
    ID_05_DOWN,
    ID_05_CAR,
    ID_06_UP,
    ID_06_DOWN,
    ID_06_CAR,
    ID_07_UP,
    ID_07_DOWN,
    ID_07_CAR,
    ID_08_UP,
    ID_08_DOWN,
    ID_08_CAR,
    ID_09_UP,
    ID_09_DOWN,
    ID_09_CAR,
    ID_10_UP,
    ID_10_DOWN,
    ID_10_CAR,
    ID_11_UP,
    ID_11_DOWN,
    ID_11_CAR,
    ID_TOP_UP, // top floor UP is a dummy IO.
    ID_TOP_DOWN,
    ID_TOP_CAR,

    ID_DOOR1_OPEN,
    ID_DOOR1_CLOSE,

    // hop2
    ID_HOP2_FIRE_RECALL, // radio group
    ID_HOP2_FIRE_RECALL_RESET,
    ID_HOP2_FIRE_RECALL_OFF,
    ID_HOP2_FIRE_RECALL_ON,
    ID_HOP2_INSP_TOP, // radio group
    ID_HOP2_INSP_TOP_UP,
    ID_HOP2_INSP_TOP_OFF,
    ID_HOP2_INSP_TOP_DOWN,
    ID_HOP2_INSP_BOTTOM, // radio group
    ID_HOP2_INSP_BOTTOM_UP,
    ID_HOP2_INSP_BOTTOM_OFF,
    ID_HOP2_INSP_BOTTOM_DOWN,

    // machineroom block
    ID_MACHINEROOM_INSP, // group radio
    ID_MACHINEROOM_INSP_ON,
    ID_MACHINEROOM_INSP_OFF,
    ID_MACHINEROOM_CAR_DOOR_BYPASS, // group
    ID_MACHINEROOM_CAR_DOOR_BYPASS_ON,
    ID_MACHINEROOM_CAR_DOOR_BYPASS_OFF,
    ID_MACHINEROOM_HALL_DOOR_BYPASS, // group
    ID_MACHINEROOM_HALL_DOOR_BYPASS_ON,
    ID_MACHINEROOM_HALL_DOOR_BYPASS_OFF,
    ID_MACHINEROOM_UP,
    ID_MACHINEROOM_ENABLE,
    ID_MACHINEROOM_DOWN,

    // cop2
    ID_COP2_HOIST_ACCESS, // group
    ID_COP2_HOIST_ACCESS_ON,
    ID_COP2_HOIST_ACCESS_OFF,
    ID_COP2_IND, // group//independent service
    ID_COP2_IND_ON,
    ID_COP2_IND_OFF,
    ID_COP2_RUN, // group//run/stop
    ID_COP2_RUN_RUN,
    ID_COP2_RUN_STOP,
    ID_COP2_UP,
    ID_COP2_ENABLE,
    ID_COP2_DOWN,

    // cop3
    ID_COP3_FIRE_PH2, // group
    ID_COP3_FIRE_PH2_OFF,
    ID_COP3_FIRE_PH2_HOLD,
    ID_COP3_FIRE_PH2_ON,
    ID_COP3_RUN, // group
    ID_COP3_RUN_RUN,
    ID_COP3_RUN_STOP,

    ID_COP3_OPEN_DOOR,
    ID_COP3_CLOSE_DOOR,
    ID_COP3_CALL_CACEL,
    // toc
    ID_TOC_INSP, // group
    ID_TOC_INSP_ON,
    ID_TOC_INSP_OFF,
    ID_TOC_RUN, // group
    ID_TOC_RUN_RUN,
    ID_TOC_RUN_STOP,
    ID_TOC_UP,
    ID_TOC_ENABLE,
    ID_TOC_DOWN,

    // non-manual input device
    ID_MOTOR_ENABLED,
    ID_MB_PICKED,
    ID_EB_PICKED,
    ID_DOOR_GATE_SWITCH,
    ID_DOOR_PHOTOEYE,
    ID_DOOR_ZONE1,
    ID_DOOR_ZONE2,
    ID_DOOR_OPEN_LIMIT,
    ID_CARTOP_ESCAPE,
    ID_CARTOP_SAFETIES,
    ID_PIT_SWITCH,
    ID_BUFFER_HIT,
    ID_FINAL_LIMIT_TOP,
    ID_FINAL_LIMIT_BOTTOM,
    ID_SPEED_GOVERNOR,
    ID_INTERLOCK_TOP,
    ID_INTERLOCK_MIDDLE,
    ID_INTERLOCK_BOTTOM,
    ID_SMOKE1_UPPER_FLOOR,
    ID_SMOKE2_LOBBY,
    ID_SMOKE3_MACHINEROOM,
    ID_SMOKE4_HOISTWAY,
    ID_SMOKE5_PIT,

    CORE_IO_INPUT_NUM,
} ENUM_CORE_INPUT_IDS;

typedef enum {
    // block: output devices
    ID_MOTOR_EN = CORE_IO_INPUT_NUM,
    ID_BUZZER,
    ID_EB_PICK,
    ID_LATERN_UP,
    ID_LATERN_DN,
    ID_CHIME,
} ENUM_CORE_OUTPUT_IDS;

/*******************************************************************************
*************************** Exported global variables **************************
*******************************************************************************/

/*******************************************************************************
******************************* Exported functions *****************************
*******************************************************************************/



#endif /* __TOP_DATATYPE_H */
/********************************* end of file ********************************/
