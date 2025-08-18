/**
******************************************************************************
* @file    ui_cop1.cpp
* @author  ARTHUR
* @version V1
* @date    2025-08-08 10:15:58
* @brief   car backgroud (shaft and limits) and animation

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
#include <windows.h>
#include <stdio.h>

#include "../00-app/top_config.h"
#include "../00-app/top_datatype.h"
#include "ui.h" 
#include "ui_logbox.h"

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


/*******************************************************************************
 * @brief  draw car box with a floor line
 * @param  x: block left,
           y: _ui_control_stru.car_box_y;
 * @param  is_idle:  disp->lv1_state == LV1_STATE_IDLE
 * @return xxxx
 *******************************************************************************/
void ui32_draw_cab_box(HDC hdc, int x, int y, bool is_idle) {
    RECT r = { x+UI_CAR_X, (y)-UI_CAR_H, x+UI_CAR_X + UI_CAR_W, y+1 }; //l,t,r,b
#if UI_CAR_FILL_COLOR
    HBRUSH hFillBrush = CreateSolidBrush(is_idle ? UI_COLOR_GREEN : UI_CAR_FILL_COLOR);

    FillRect(hdc, &r, hFillBrush);
    DeleteObject(hFillBrush); // Clean up
#endif
    
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); //// Create a pen for the border (e.g., black) Solid black border
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH)); // No fill for border
    //[1] Draw the rectangle border
    Rectangle(hdc, r.left, r.top, r.right, r.bottom);
    //[2] draw a horizon line on bottom of the box...
    MoveToEx(hdc, r.left-1, y, NULL);    // Start at bottom-right
    LineTo(hdc, x+UI_LIMIT_X, y);   //connect to the 'final limits line'
    
    SelectObject(hdc, hOldPen);// Restore old objects and clean up
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}

/*******************************************************************************
 * @brief  draw car door frame (hollow box) and opening (color filling)
 * @param  x: block x
 * @param  car_y: _ui_control_stru.car_box_y;
 * @param  opening: _ui_control_stru.door_opening_width
 * @return xxxx
 *******************************************************************************/
void ui36_draw_door(HDC hdc, int x, int car_y, int opening) {
    
    int car_left = x + UI_CAR_X;
    //door frame rectangle
    LONG left_gap = (UI_CAR_W - UI_DOOR_W) / 2; //the left_gap width between car left and door left
    RECT r = { 
        car_left + left_gap,
        car_y - UI_DOOR_H,
        car_left + left_gap + UI_DOOR_W,
        car_y + 1 
    }; //l,t,r,b

    HPEN hPen        = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // Create a pen for the border (e.g., black) Solid black border
    HPEN hOldPen     = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH)); // No fill for border
    //[1]draw door frame: rectangle border
    Rectangle(hdc, r.left, r.top, r.right, r.bottom);

    //[2]draw door slides
    if (opening > 0) {//draw an rectangle of the opening
        LONG door_open = opening;
        RECT ro = { 
            r.left + 1, 
            car_y-UI_DOOR_H + 1, 
            r.left + 1 + door_open, 
            car_y 
        }; // l,t,r,b
        HBRUSH hFillBrush = CreateSolidBrush(UI_DOOR_FILL_COLOR);
        FillRect(hdc, &ro, hFillBrush);
        DeleteObject(hFillBrush); // Clean up
    }
    // Restore old objects and clean up
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}


/*******************************************************************************
 * @brief  draw final limits: two red circles and a line conneting them
 * @param  x: parent block left x.
 * @return xxxx
 *******************************************************************************/
void ui34_draw_final_limits(HDC hdc, int x)
{
    int centerX  = x+UI_LIMIT_X;
    int centerY1 = UI_LIMIT_Y_TOP;
    int centerY2 = UI_LIMIT_Y_BOTTOM;
    
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));// Create a solid red brush
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, redBrush);

    // Draw the filled circle (ellipse with equal width and height)
    Ellipse(hdc,
        centerX - UI_LIMIT_CIRCLE_RADIUS,
        centerY1 - UI_LIMIT_CIRCLE_RADIUS,
        centerX + UI_LIMIT_CIRCLE_RADIUS,
        centerY1 + UI_LIMIT_CIRCLE_RADIUS);

    Ellipse(hdc,
        centerX - UI_LIMIT_CIRCLE_RADIUS,
        centerY2 - UI_LIMIT_CIRCLE_RADIUS,
        centerX + UI_LIMIT_CIRCLE_RADIUS,
        centerY2 + UI_LIMIT_CIRCLE_RADIUS);
    
    MoveToEx(hdc, centerX, centerY1 + UI_LIMIT_CIRCLE_RADIUS, NULL);// Draw connecting line
    LineTo(hdc, centerX, centerY2 - UI_LIMIT_CIRCLE_RADIUS);

    SelectObject(hdc, oldBrush);// Restore old brush and clean up
    DeleteObject(redBrush);
}



/*******************************************************************************
* @brief  draw floor boxes and ground line base on block-x and ground-y
* @param  x = UI_ANIMATION_X
* @param  y = UI_GROUND_Y
* @return xxxx
*******************************************************************************/
void ui_draw_floors(HDC hdc, int x, int y) {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));     // 2px black line
    HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH); // No fill
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);// Select them into the device context
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    //[1]Draw ground line
    MoveToEx(hdc, x+UI_GROUND_LINE_X_LEFT, y, NULL);
    LineTo(hdc, x+UI_GROUND_LINE_X_RIGHT, y);
    //[2] Draw each floor box
    for (int i = 0; i < APP_FLOOR_NUM; ++i) {
        RECT floorRect;
        floorRect.left      = x + UI_SHAFT_X;
        floorRect.right     = x + UI_SHAFT_X + UI_SHAFT_W;
        floorRect.bottom    = y - i*UI_FLOOR_H+1;
        floorRect.top       = floorRect.bottom- UI_FLOOR_H-1;
        Rectangle(hdc, floorRect.left, floorRect.top, floorRect.right, floorRect.bottom);
        
        //[2.2] Draw floor labels
        char label[8];
        int labelY = floorRect.bottom - 40;
        sprintf_s(label, "F%d", i + 1); // i=0..n, add '1' to display 1..m
        TextOutA(hdc, x + UI_FLOOR_LABEL_X, labelY, label, (int)strlen(label));
    }

    SelectObject(hdc, hOldPen);// Restore old objects
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);// Clean up GDI objects
}


/*******************************************************************************
 * @brief  This is for drawing a red rectangle where needed
 * @param  x = UI_ANIMATION_X
 * @return xxxx
 *******************************************************************************/
void ui_draw_invalidate_rect_area_debug(HDC hdc, CONST RECT* lpRect) {
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));// nbew brush - save old
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));// new red pen

    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);// select new pen - save old
    Rectangle(hdc, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);// draw rect

    SelectObject(hdc, hOldPen);// restore original pen and brush
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);// delete created pen
}


/*******************************************************************************
 * @brief  convert realtime car positon to realtime pixel position for animation 
 * @param  mm, the car's position from simulator
 * @return pixel, the car's bottom y pixel positon between shaft top and bottom limit
 *******************************************************************************/
int ui_calc_car_y_pix(int car_realtime_position_mm) {
    int y_pix;
    y_pix = UI_GROUND_Y - car_realtime_position_mm / UI_PIXEL_RATIO;
    return y_pix;
}
   
/*******************************************************************************
 * @brief  convert opening percentage to opening width in pixel.
 * @param  per, opening percentage, 0--100
 * @return width of door opening, 0~32pix
 *******************************************************************************/
int ui_calc_door_opening(int per) {
    int opening_width;
    opening_width = per * UI_DOOR_W / 100;
    return opening_width;
}

/********************************* end of file ********************************/

