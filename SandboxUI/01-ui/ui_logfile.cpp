/**
******************************************************************************
* @file    logger.cpp
* @author  Sean, Arthur
* @version V1
* @date    MAY 9, 2025
* @brief   Logger for car position and car floor position
*          Handling .csv files. 
*         -May 22, 2025: change log functions to adapt to 'reset' functions
*         -June20,2025: fix log file bug to let test automation read it at the same time.
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
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <direct.h> //_mkdir()

#include "ui.h"
#include <tchar.h>

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
static FILE*    _logfile; //current logfile handler, when it's NULL, log_data() will skip operating

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
* @brief  creat a new log file "logs/%s" or "logs/log_%04d-%02d-%02d_%02d-%02d-%02d.csv"
* @param  filename: file name for the log file, if NULL, use timestamp instead
* @retval xxxx
*******************************************************************************/
void ui61_start_logging(const char* filename) {
    char finalFilename[256];
    
    int rt = _mkdir("logs");  //// Ensure the logs folder exists. No effect if it already exists
    if (filename && filename[0] != '\0') {
        snprintf(finalFilename, sizeof(finalFilename), "logs/%s", filename);
    }
    else {
        time_t now = time(NULL);
        struct tm t;
        localtime_s(&t, &now);
        snprintf(finalFilename, sizeof(finalFilename),
            "logs/log_%04d-%02d-%02d_%02d-%02d-%02d.csv",
            t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
            t.tm_hour, t.tm_min, t.tm_sec);
    }

    
    _logfile = _fsopen(finalFilename, "w", _SH_DENYNO);// _SH_DENYNO = shared read access so other processes can read it
    if (_logfile) {
        ui_internal_printf("Start logging into '%s'.", finalFilename);
        fprintf(_logfile, "t_ms,\thpv_mm,\tvpv_mm,\tdoor1_percent\n");
        fflush(_logfile);
    }
    else {
        _logfile = NULL;
    }
}

/*******************************************************************************
* @brief  call this periodically to add data line to the log file.
* @param  xxxx
* @retval xxxx
*******************************************************************************/
void ui62_log_data(int t_ms, int hpv_mm, int vpv_mm, int door1_percent) {
    if (_logfile) {
        fprintf(_logfile, "%d,\t%d,\t%d,\t%d\n", t_ms, hpv_mm, vpv_mm, door1_percent);
        fflush(_logfile); // optional: for real-time logs
    }
}

/*******************************************************************************
* @brief  close the file, _logfile will be NULL and log_data() will skip afterwards
* @param  xxxx
* @retval xxxx
*******************************************************************************/
void ui63_stop_logging() {
    if (_logfile) {
        fclose(_logfile);
        _logfile = NULL;
        ui_internal_printf("Logging is stopped.");
    }
}

/********************************* end of file ********************************/