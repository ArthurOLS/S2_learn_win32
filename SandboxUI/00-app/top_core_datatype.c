/**
******************************************************************************
* @file    top_core_constants.c
* @author  ARTHUR
* @version V1
* @date    2025-08-20 17:10:14
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
#include "top_core_datatype.h"

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


const char* lv3_get_door_state_name(int32_t step) {
    switch (step) {
    case LV3_DOOR_STEP_WAIT1:
        return "LV3_DOOR_WAIT1";
    case LV3_DOOR_STEP_OPENING:
        return "LV3_DOOR_OPENING";
    case LV3_DOOR_STEP_HOLDING:
        return "LV3_DOOR_HOLDING";
    case LV3_DOOR_STEP_CLOSING:
        return "LV3_DOOR_CLOSING";
    case LV3_DOOR_STEP_CLOSE_CONFIRMED:
        return "LV3_DOOR_CLOSE_CONFIRMED";
    default:
        return "UNKNOWN_LV3_DOOR_STEP"; // Catches any unexpected integer values
    }
}

const char* fsm_lv1_get_state_string(int32_t state) {
    const char* name[] = {
        "IDLE",
        "SERVICE_DOWN",
        "SERVICE_UP",
    };
    return (const char*)(name[state]);
}

const char* core_interface_get_lv2_state_string(int32_t state) {
    switch (state) {
    case LV2_INIT:
        return "LV2_INIT";
    case LV2_SWITCH_DELAY:
        return "LV2_SWITCH_DELAY";
    case LV2_MANUAL_MODE:
        return "LV2_MANUAL_MODE";
    case LV2_AUTO_MODE:
        return "LV2_AUTO_MODE";
    default:
        return "UNKNOWN_LV2_STATE";
    }
}

const char* fsm_lv3_get_state_string(int32_t state) {
    switch (state) {
    case LV3_IDLE_STOPPED:
        return "LV3_IDLE_STOPPED";
    case LV3_IDLE_HOMING:
        return "LV3_IDLE_HOMING";
    case LV3_MOVING_X:
        return "LV3_MOVING_X";
    case LV3_PICK_DROP:
        return "LV3_PICK_DROP";
    default:
        return "UNKNOWN_LV3_STATE";
    }
}

const char* fsm_lv4_get_state_string(int32_t state) {
    // return (const char*)(_fsm_lv4_states_list[state].name);
    switch (state) {
    case LV4_STATE_INIT:
        return "LV4_STATE_INIT";
    case LV4_STATE_GOTO_SERVICE_START:
        return "LV4_STATE_GOTO_START";
    case LV4_STATE_AT_SERVICE_START:
        return "LV4_STATE_AT_START";
    case LV4_STATE_GO_ON_SERVICE:
        return "LV4_STATE_GO_ON_SERVICE";
    case LV4_STATE_EXIT:
        return "LV4_STATE_EXIT";
    default:
        return "UNKNOWN_LV4_STATE";
    }
}

/********************************* end of file ********************************/

