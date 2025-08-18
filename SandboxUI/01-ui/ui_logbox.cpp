/**
******************************************************************************
* @file    ui_logbox.cpp
* @author  ARTHUR
* @version V1
* @date    2025-08-07 11:55:16
* @brief   log box module
*
*
******************************************************************************
* @attention
*          ONLY Ascii is supported in logbox
*
******************************************************************************
*/

/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/

#include <windows.h>
#include <stdint.h>
#include <stdio.h> //file operation
#include "ui_logbox.h" //it self
#include "ui.h" //to use ui_internal_printf()
#include "ui_lowlevel.h"

/*******************************************************************************
******************************** Private typedef *******************************
*******************************************************************************/

/*******************************************************************************
******************************** Private define ********************************
*******************************************************************************/
#define UI_LOGBOX_MAX_SAVE_BYTES 65536

/*******************************************************************************
********************************* Private macro ********************************
*******************************************************************************/

/*******************************************************************************
******************************* Private variables ******************************
*******************************************************************************/
static HWND         __hlogbox; // Global handle to the log display area
static uint64_t     __start_time;
static char         __save_buffer[UI_LOGBOX_MAX_SAVE_BYTES + 1]; //for saving content to a file.

/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/

/*******************************************************************************
******************************* Private functions ******************************
*******************************************************************************/

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void _append_text_to_logbox_tail(const char* text) {
    int len = GetWindowTextLengthW(__hlogbox); // Get current length of the text

    SendMessageA(__hlogbox, EM_SETSEL, (WPARAM)len, (LPARAM)len); // Move the caret to the end
    SendMessageA(__hlogbox, EM_REPLACESEL, FALSE, (LPARAM)text); // Append the new text
}

/*******************************************************************************
******************************* Public Variables *******************************
*******************************************************************************/

/*******************************************************************************
******************************* Public functions *******************************
*******************************************************************************/

/*******************************************************************************
 * @brief  create logbox window
 * @param  HWND hwnd, parent window
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_create_logbox(HWND hwnd, int x, int y, int w, int h, uint64_t set_start_ts) {
    //[4] Log box ( read-only, scrollable, multiline text box.)
    __hlogbox = CreateWindow(
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL,
        x, y, w, h, // x, y, width, height (adjust as needed)
        hwnd, NULL, NULL, NULL);

    ui10_apply_font_to_control(__hlogbox, 9);

    if (set_start_ts == 0){
        __start_time = GetTickCount64();
    }
    else {
        __start_time = set_start_ts;
    }
}
   

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_logbox_printf(const char* fmt, ...) {
    char buffer[512];

    // Format the string using va_list
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args); // Safe version of sprintf
    va_end(args);

    _append_text_to_logbox_tail(buffer);
}

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_logbox_save_content_to_file(void) {
    int len = GetWindowTextLengthA(__hlogbox);
    if (len < 100) {
        ui_internal_printf("---- E: %d, <100 words, no need to save.", len);
        return;
    }
    if (len > UI_LOGBOX_MAX_SAVE_BYTES) {
        ui_internal_printf("---- W: logbox has (%dB) more than MAX(%dB) to save.", len, UI_LOGBOX_MAX_SAVE_BYTES);
    }

    GetWindowTextA(__hlogbox, __save_buffer, UI_LOGBOX_MAX_SAVE_BYTES);
    const char* filename = "latest_logbox.txt";

    // Save to file, here use 'wb', 'b' means binary mode, if not, the C runtime translates \n into \r\n automatically, which make \r\n to \r\n\n.
    FILE* file = NULL;
    if (fopen_s(&file, filename, "wb") == 0 && file != NULL) {
        fwrite(__save_buffer, sizeof(char), len, file);
        fclose(file);

        ui_internal_printf("---- OK: save %d words to %s.", len, filename);
    }
    else {
        ui_internal_printf("---- NG: failed to save logbox.");
    }
}

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_logbox_clear_content() {
    SetWindowTextW(__hlogbox, L"");
    InvalidateRect(__hlogbox, NULL, TRUE); // fore a redraw
    UpdateWindow(__hlogbox);
}


/*******************************************************************************
 * @brief  get ui run tim in ms, it use the global ts_at_start variable
 * @param  xxxx
 * @return ms since app started
 * @note   overflows after ~49 days
 *******************************************************************************/
uint64_t ui_logbox_get_run_ms64() {
    uint64_t ts = GetTickCount64() - __start_time;
    return ts; // overflow after ~49 days for u32, 580 million yrs for u64
}

/********************************* end of file ********************************/

