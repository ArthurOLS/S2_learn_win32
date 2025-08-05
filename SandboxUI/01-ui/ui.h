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

/*******************************************************************************
********************************* Overall Settings *****************************
*******************************************************************************/
// main window
#define WINDOWS_WIDTH               800
#define WINDOWS_HEIGHT              480
#define WINDOWS_START_X             0
#define WINDOWS_START_Y             0
#define UI_PERIOD_MS                30   // refresh ui data and display every 50ms
#define UI_PIXEL_RATIO              50 // pixel = length(in mm) * RATIO. I adopt: 1px=50mm, 60px=3000mm for a floor height
#define UI_GROUND_Y                 550   // set where the ground (physical 0mm horizontal line)




/*******************************************************************************
********************************* UI local setting *****************************
*******************************************************************************/


#define LABEL_STRING_SIZE           512

#define UI_FONT_9PT                 9  //for text, menu, logbox
#define UI_FONT_12PT                12 //for title bars, taskbars,


/*******************************************************************************
******************************* Exported functions *****************************
*******************************************************************************/
void ui1_init_widgets(HWND hwnd);
void ui2_button_action_callback(HWND hwnd, int id);
void ui3_button_drawitem_callback(LPDRAWITEMSTRUCT lpDrawItem);
#endif /* __UI_H */
/********************************* end of file ********************************/

