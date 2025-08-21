/**
********************************************************************************
* @file    top_sim_constants.h
* @author  Arthur
* @version V1.0
* @date    2025-08-18 15:54:00
* @brief   top layer constants from simulation module
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
#ifndef __TOP_SIM_CONSTANTS_H
#define __TOP_SIM_CONSTANTS_H
/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/
//#include 

/*******************************************************************************
********************************* Exported macro *******************************
*******************************************************************************/

/*******************************************************************************
********************************* Exported types *******************************
*******************************************************************************/

/*******************************************************************************
********************************* Exported types *******************************
*******************************************************************************/


typedef enum {
    CMD_DOWN = -1,
    CAR_DIR_DOWN = -1,
    CMD_STOP = 0,
    CAR_DIR_STOPPED = 0,
    CMD_UP = 1,
    CAR_DIR_UP = 1,
} ENUM_CAR_DIR_AND_CMD;

/*******************************************************************************
*********************************  Simulator  **********************************
*******************************************************************************/

typedef enum {
    CAR_ERR_HIT_FINAL_TOP = 1,
    CAR_ERR_HIT_FINAL_BOTTOM,
} ENUM_CAR_ERR_CODE;

//CAR Simulator State Machine
typedef enum {
    CAR_STATE_STOPPED     = 1,
    CAR_STATE_STOPPING    = 2,
    CAR_STATE_MOVING_UP   = 3,
    CAR_STATE_MOVING_DOWN = 4,
} ENUM_CAR_STATE;


//Single door cmd enum
#ifndef DEFINE_DOOR_CMD_ENUM
#define DEFINE_DOOR_CMD_ENUM
typedef enum {
    ENUM_DOOR_CMD_NONE = 0,
    ENUM_DOOR_CMD_OPEN,
    ENUM_DOOR_CMD_CLOSE,
    ENUM_DOOR_CMD_FIREMAN,
    ENUM_DOOR_CMD_EMERGENCY_OPEN
} DOOR_CMD_ENUM;
#endif

//SINGLE DOOR STATE
typedef enum {
    DOOR_STATE_NONE = 0,
    DOOR_STATE_CLOSING,
    DOOR_STATE_CLOSING_END, //buffer period in closing end
    DOOR_STATE_CLOSED,
    DOOR_STATE_OPENING,
    DOOR_STATE_OPENING_END, //buffer period in opening end
    DOOR_STATE_OPEN,
    DOOR_STATE_ERROR,
} SIM_DOOR_STATE_ENUM;



/*******************************************************************************
*************************** Exported global variables **************************
*******************************************************************************/

/*******************************************************************************
******************************* Exported functions *****************************
*******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

    const char* sim30_get_state_string(int state);
    const char* sim31_get_cmd_string(int cmd);
    const char* sim32_get_dir_string(int dir);
    const char* sim33_get_err_string(int errCode);

    const char* sim_door_get_state_string(int state);
    const char* sim_door_get_cmd_string(int command);


#ifdef __cplusplus
}
#endif


#endif /* __TOP_SIM_CONSTANTS_H */
/********************************* end of file ********************************/

