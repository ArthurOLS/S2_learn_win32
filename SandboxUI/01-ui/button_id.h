/**
********************************************************************************
* @file    button_id.h
* @author  Arthur
* @version V1.0
* @date    2025-08-01 09:51:48
* @brief   button id definition in this project.
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
#ifndef __BUTTON_ID_H
#define __BUTTON_ID_H
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
******************************* Exported constants *****************************
*******************************************************************************/
typedef enum {

    //click button id
    BUTTON_ID_DOWN = 2000,
    BUTTON_ID_UP,
    BUTTON_ID_ENABLE,


    //[GROUP1]
    BUTTON_BASE_ID = 1000,
    H1UP = 1000, // its id = 1000, name="H1UP"
    H1DN,        // 1001
    CF1,         // 1002
    H2UP,        // 1003
    H2DN,        // 1004
    CF2,         // 1005
    H3UP,
    H3DN,
    CF3,
    H4UP,
    H4DN,
    CF4,
    H5UP,
    H5DN,
    CF5,
    H6UP,
    H6DN,
    CF6,
    H7UP,
    H7DN,
    CF7,
    H8UP,
    H8DN,
    CF8,
    H9UP,
    H9DN,
    CF9,
    H10UP,
    H10DN,
    CF10,
    CALL_BUTTON_MAX,
    D1_OPEN,
    D1_CLOSE,


    //[DEBUG BUTTONS]
    COP_UP = 2000,
    COP_DN,
    COP_STOP,

    MOTOR_UP,
    MOTOR_DN,
    MOTOR_ST,

    SPEED1, // 0.2 M/S
    SPEED2, // 1.0 M/S
    SPEED3, // 2.0 M/S

    NORMAL,
    INSP,
    INDP,

    X_APP,  // Simulator Stop
    X_CORE, // Simulator Start
    X_SIM,

    Go_n1m, // test traction to goto -1m abs
    Go_1m,  // test traction to goto 1m abs
    Go_5m,  // test traction to goto 2m abs

    RESET, // this will reset and automatically start logging.
    STOP_LOG,
    SAVE_PRINT, // save the printf logbox to a file
    OPEN_DOOR,
    CLOSE_DOOR,
    HOLD_DOOR,
    /* Also remember to modify BUTTON_ID_DEFINE in ui.cpp */
    /* Also remember to adjust UI_BUTTON_GROUP2_NUM in UI.h */

    //Radio switchs
    ID_RADIO_STARTING = 4000,
    ID_COP3_FIRE_PH2_OFF,
    ID_COP3_FIRE_PH2_HOLD,
    ID_COP3_FIRE_PH2_ON,
    
    ID_COP3_RUN_RUN,
    ID_COP3_RUN_STOP,

    ID_COP3_ENABLE,
} ENUM_BUTTON_ID;


/*******************************************************************************
*************************** Exported global variables **************************
*******************************************************************************/

/*******************************************************************************
******************************* Exported functions *****************************
*******************************************************************************/



#endif /* __BUTTON_ID_H */
/********************************* end of file ********************************/

