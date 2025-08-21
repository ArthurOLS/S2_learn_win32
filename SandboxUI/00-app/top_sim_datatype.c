/**
******************************************************************************
* @file    top_sim_constants.c
* @author  ARTHUR
* @version V1
* @date    2025-08-18 15:55:30
* @brief   functions to get the string of the constant
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
#include <stdint.h>
#include "top_sim_datatype.h"

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

const char* sim30_get_state_string(int state) {
    switch (state) {
    case CAR_STATE_STOPPED:
        return "STOPPED";
    case CAR_STATE_STOPPING:
        return "STOPPING";
    case CAR_STATE_MOVING_UP:
        return "MOVING_UP";
    case CAR_STATE_MOVING_DOWN:
        return "MOVING_DOWN";
    default:
        return "UNKNOWN"; // Handle unexpected values
    }
}

const char* sim31_get_cmd_string(int cmd) {
    switch (cmd) {
    case CMD_DOWN: return "CMD_DOWN";
    case CMD_STOP: return "CMD_STOP";
    case CMD_UP: return "CMD_UP";
    default: return "UNKNOWN_CMD";
    }
}

const char* sim32_get_dir_string(int dir) {
    switch (dir) {
    case CAR_DIR_DOWN: return "DOWN";
    case CAR_DIR_STOPPED: return "CAR_STOPPED";
    case CAR_DIR_UP: return "UP";
    default: return "UNKNOWN_DIR";
    }
}

const char* sim33_get_err_string(int errCode) {
    switch (errCode) {
    case CAR_ERR_HIT_FINAL_TOP:
        return "CAR_ERR_HIT_FINAL_TOP";
    case CAR_ERR_HIT_FINAL_BOTTOM:
        return "CAR_ERR_HIT_FINAL_BOTTOM";
    case 0:
        return "NONE";
    default:
        return "UNKNOWN_CAR_ERR";
    }
}

const char* sim_door_get_state_string(int state) {
    // Cast the integer 'state' to the enum type for type-safe comparison in the switch statement.
    SIM_DOOR_STATE_ENUM enumState = (SIM_DOOR_STATE_ENUM)state;

    switch (enumState) {
    case DOOR_STATE_NONE:
        return "DOOR_STATE_NONE";
    case DOOR_STATE_CLOSING:
        return "DOOR_STATE_CLOSING";
    case DOOR_STATE_CLOSING_END:
        return "DOOR_STATE_CLOSING_END";
    case DOOR_STATE_CLOSED:
        return "DOOR_STATE_CLOSED";
    case DOOR_STATE_OPENING:
        return "DOOR_STATE_OPENING";
    case DOOR_STATE_OPENING_END:
        return "DOOR_STATE_OPENING_END";
    case DOOR_STATE_OPEN:
        return "DOOR_STATE_OPEN";
    case DOOR_STATE_ERROR:
        return "DOOR_STATE_ERROR";
    default:
        return "UNKNOWN_DOOR_STATE"; // Handle any integer values that don't match an enum.
    }
}

const char* sim_door_get_cmd_string(int command) {
    // Cast the integer 'command' to the enum type for type-safe comparison.
    DOOR_CMD_ENUM enumCommand = (DOOR_CMD_ENUM)command;

    switch (enumCommand) {
    case ENUM_DOOR_CMD_NONE:
        return "ENUM_DOOR_CMD_NONE";
    case ENUM_DOOR_CMD_OPEN:
        return "ENUM_DOOR_CMD_OPEN";
    case ENUM_DOOR_CMD_CLOSE:
        return "ENUM_DOOR_CMD_CLOSE";
    case ENUM_DOOR_CMD_FIREMAN:
        return "ENUM_DOOR_CMD_FIREMAN";
    case ENUM_DOOR_CMD_EMERGENCY_OPEN:
        return "ENUM_DOOR_CMD_EMERGENCY_OPEN";
    default:
        return "UNKNOWN_DOOR_CMD"; // Handle any integer values that don't match an enum.
    }
}


/********************************* end of file ********************************/

