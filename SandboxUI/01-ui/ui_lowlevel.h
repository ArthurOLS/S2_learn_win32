/**
********************************************************************************
* @file    ui_lowlevel.h
* @author  Arthur
* @version V1.0
* @date    2025-08-07 11:02:20
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
#ifndef __UI_LOWLEVEL_H
#define __UI_LOWLEVEL_H
/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/

/*******************************************************************************
********************************* Exported macro *******************************
*******************************************************************************/

/*******************************************************************************
********************************* Exported types *******************************
*******************************************************************************/

typedef struct {
    const wchar_t* label;
    const wchar_t* r0_label;
    const wchar_t* r1_label;
    const wchar_t* r2_label;
    int id0, id1, id2;
                        //0=r0 is checked, 1=r1 is checked, 2=r2 is checked,
    int state_default;  // which is checked when created

} RADIO_3POS_STRU;

typedef struct {
    const wchar_t* label;
    const wchar_t* r0_label;
    const wchar_t* r1_label;
    int id0, id1;
    int state_default; // which is checked when created

} RADIO_2POS_STRU;

/*******************************************************************************
******************************* Exported constants *****************************
*******************************************************************************/
#define UI_FONT_9PT                     9   // for text, menu, logbox
#define UI_FONT_12PT                    12 // for title bars, taskbars,

/*******************************************************************************
*************************** Exported global variables **************************
*******************************************************************************/

/*******************************************************************************
******************************* Exported functions *****************************
*******************************************************************************/

void ui10_apply_font_to_control(HWND hwndTarget, int pt);
void ui_create_font9();

void ui11_create_label(HWND hwnd, const wchar_t* text, int x, int y, int w, int h);
void ui11_create_label(HWND hwnd, const char* text, int x, int y, int w, int h);
void ui11_create_label_no_border(HWND hwnd, const wchar_t* text, int x, int y, int w, int h);
void ui_create_radio_type_3pos(HWND hwnd, int x, int y, RADIO_3POS_STRU* pr);
void ui_create_radio_type_2pos(HWND hwnd, int x, int y, RADIO_2POS_STRU* pr);
void ui_create_button_type_continuous(HWND hwnd, int x, int y, const wchar_t* name, int id);
void ui_create_button_type_lock(HWND hwnd, int x, int y, const wchar_t* text, int id);
void ui_create_button_type_click(HWND hwnd, int x, int y, const wchar_t* text, int id);

void ui30_draw_button_led_black(LPDRAWITEMSTRUCT lpDrawItem, const wchar_t* text, bool state);
void ui30_draw_button_led_color(LPDRAWITEMSTRUCT lpDrawItem, const wchar_t* text, bool state, int color);
void ui30_draw_custom_button_trigger_redraw(HWND hwnd, int id);




#endif /* __UI_LOWLEVEL_H */
/********************************* end of file ********************************/

