/**
******************************************************************************
* @file    config.cpp
* @author  ARTHUR
* @version V1
* @date    MAY 26, 2025
* @brief   load config data form config.ini, it the file does not exit, create
*          a new one with default values.
*
* How to use this module?
* 1. call app_init()/ui40_load_config() to load data from cfg file to 
*    _config_data_management[],
*    [note] this function only needs to be called once at the very begining of 
*    the application.
* 2. optionally, call ui41_print_all_config(); to view all the values in the file
* 3. read a parameter each time through
*    int _mode;
*    ui42_config_get_parameter("Mode", &(_mode));
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

#include <direct.h> //_mkdir()

#include "../00-app/top_config.h"
#include "ui_configfile.h"
#include "ui.h"
#include "ui_logbox.h"

/*******************************************************************************
******************************** Private typedef *******************************
*******************************************************************************/

/*******************************************************************************
******************************** Private define ********************************
*******************************************************************************/
#define CONFIG_FILE_PATH "config/config.ini"

/*******************************************************************************
********************************* Private macro ********************************
*******************************************************************************/
//eg. DEFINE_CONFIG_ITEM(WindowX, 0) expands to {"WindowX", 0, 0}
#define DEFINE_CONFIG_ITEM(name, def_val)    {#name, 0, def_val}

//this magic marco is used to convert anything like #define NAME into "NAME" string
//Usage:MAKE_STRING(XXX), you get "XXX"
#define STRINGIFY(x) #x
#define MAKE_STRING(x) STRINGIFY(x)

/*******************************************************************************
******************************** Private typedef *******************************
*******************************************************************************/
typedef struct {
    const char* name;
    int	  value;
    int   default_value; //when config file is not available
} CONFIG_NAME_VALUE_DEFVAL_STRU;

/*******************************************************************************
******************************* Private variables ******************************
*******************************************************************************/
static FILE* configFile;

static CONFIG_NAME_VALUE_DEFVAL_STRU _config_data_management[] = {
    DEFINE_CONFIG_ITEM(WindowX, 0),
    DEFINE_CONFIG_ITEM(WindowY, 0),
    DEFINE_CONFIG_ITEM(Mode, 0),

    /** Add more parameters **/
};

#define CONFIG_LIST_SIZE   (sizeof(_config_data_management)/sizeof(CONFIG_NAME_VALUE_DEFVAL_STRU))

/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/
static void _config1_create_default_file();


/*******************************************************************************
******************************* Private functions ******************************
*******************************************************************************/


void _config1_create_default_file() {
    //Create a empty new file
    int ok = _mkdir("config"); //ensure config directory exists

    if (fopen_s(&configFile, CONFIG_FILE_PATH, "w") ==0) { //if open successfully
         fclose(configFile);
   }
    else {
        DWORD err = GetLastError();
        ui_internal_printf("Failed to create empty config file. Error code: %lu\n", err);
    }

    //[1]Write data to the file
    for (int i = 0; i < CONFIG_LIST_SIZE; i++) {
        char str[16] = { 0 };
        _itoa_s(_config_data_management[i].default_value, str, 16); // _itoa(number, buffer, 10);
        BOOL success =  WritePrivateProfileStringA("Window",
            _config_data_management[i].name, 
            str, CONFIG_FILE_PATH);

        if (!success) {
            DWORD error = GetLastError();
            char fullPath[MAX_PATH];
            GetFullPathNameA(CONFIG_FILE_PATH, MAX_PATH, fullPath, NULL);
            ui_internal_printf("Failed to write config file. Path=%s, Error=%lu\n", fullPath, error);
        }
    }

}


/*******************************************************************************
******************************* Public Variables *******************************
*******************************************************************************/

/*******************************************************************************
******************************* Public functions *******************************
*******************************************************************************/

/*******************************************************************************
* @brief  called once to load data from .ini file 
* @param  xxxx
* @retval xxxx
*******************************************************************************/
void ui40_load_config(void){

    // Step 1: Check if config file exists
    DWORD attributes = GetFileAttributesA(CONFIG_FILE_PATH);
    if (attributes == INVALID_FILE_ATTRIBUTES || (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        ui_internal_printf("Config file not found. Creating default config.");
        _config1_create_default_file();
    }

    //[2] Read config values
    for (int i = 0; i < CONFIG_LIST_SIZE; i++) {
        _config_data_management[i].value = GetPrivateProfileIntA("Window", 
            _config_data_management[i].name,
            _config_data_management[i].default_value, CONFIG_FILE_PATH);
    }
}


/*******************************************************************************
* @brief  print all parameter onto logo box for reviewing
* @param  xxxx
* @retval xxxx
*******************************************************************************/
void ui41_print_all_config(void) {
    const char* str1 =
        "\r\n***********************************************\r\n"
        "-Config.ini parameters: (default value)";
    const char* str2 =
        "\r\n***********************************************\r\n";
    ui_logbox_printf(str1);
    for (int i = 0; i < CONFIG_LIST_SIZE; i++) {
        ui_logbox_printf("\r\n - %s=%d (%d).", 
            _config_data_management[i].name,
            _config_data_management[i].value,
            _config_data_management[i].default_value);
    }
    ui_logbox_printf(str2);
}


/*******************************************************************************
* @brief  Get one parameter value
* @param  name1: the name to look up
*         value: where to write back the value found, if not found, this value is 
*                not changed.
* @retval -1=fail, 1=success
*******************************************************************************/
int ui42_config_get_parameter(const char* name1, int *value) {
    for (int i = 0; i < CONFIG_LIST_SIZE; i++) {
        if (0 == strcmp(name1, _config_data_management[i].name)) {
            *value = _config_data_management[i].value;
        }
    }
    return -1;
}

/********************************* end of file ********************************/

