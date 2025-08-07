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
*
*
******************************************************************************
*/

/*******************************************************************************
************************************ Includes **********************************
*******************************************************************************/

#include <windows.h>
#include <stdio.h> //file operation
#include "ui_logbox.h" //it self
#include "ui.h" //to use ui_internal_printf()

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
static HWND hLogBox; // Global handle to the log display area

/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/
extern void ui10_apply_font_to_control(HWND hwndTarget, int pt);

/*******************************************************************************
******************************* Private functions ******************************
*******************************************************************************/

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui40_append_text_to_logbox_tail(const char* text) {
    int len = GetWindowTextLengthW(hLogBox); // Get current length of the text

    SendMessageA(hLogBox, EM_SETSEL, (WPARAM)len, (LPARAM)len); // Move the caret to the end

    SendMessageA(hLogBox, EM_REPLACESEL, FALSE, (LPARAM)text); // Append the new text
}

/*******************************************************************************
******************************* Public Variables *******************************
*******************************************************************************/

/*******************************************************************************
******************************* Public functions *******************************
*******************************************************************************/

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  HWND hwnd, parent window
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui_logbox_create(HWND hwnd, int x, int y, int w, int h) {
    //[4] Log box ( read-only, scrollable, multiline text box.)
    hLogBox = CreateWindow(
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL,
        x, y, w, h, // x, y, width, height (adjust as needed)
        hwnd, NULL, NULL, NULL);

    ui10_apply_font_to_control(hLogBox, 9);
}
   

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui41_logbox_printf(const char* fmt, ...) {
    char buffer[512];

    // Format the string using va_list
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args); // Safe version of sprintf
    va_end(args);

    ui40_append_text_to_logbox_tail(buffer);
}

/*******************************************************************************
 * @brief  Brief_description_of_the_function
 * @param  xxxx
 * @param  xxxx
 * @return xxxx
 *******************************************************************************/
void ui42_logbox_save_content_to_file(void) {
    int len = GetWindowTextLengthA(hLogBox);
    if (len < 100) {
        ui_internal_printf("---- E: %d, <100 words, no need to save.", len);
        return;
    }
    if (len > UI_LOGBOX_MAX_SAVE_BYTES) {
        ui_internal_printf("---- W: logbox has (%dB) more than MAX(%dB) to save.", len, UI_LOGBOX_MAX_SAVE_BYTES);
    }

    char buffer[UI_LOGBOX_MAX_SAVE_BYTES+1];
    GetWindowTextA(hLogBox, buffer, UI_LOGBOX_MAX_SAVE_BYTES);
    const char* filename = "latest_logbox.txt";

    // Save to file, here use 'wb', 'b' means binary mode, if not, the C runtime translates \n into \r\n automatically, which make \r\n to \r\n\n.
    FILE* file = NULL;
    if (fopen_s(&file, filename, "wb") == 0 && file != NULL) {
        fwrite(buffer, sizeof(char), len, file);
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
void ui43_logbox_clear_content() {
    SetWindowTextW(hLogBox, L"");
    InvalidateRect(hLogBox, NULL, TRUE); // fore a redraw
    UpdateWindow(hLogBox);
}

/********************************* end of file ********************************/

