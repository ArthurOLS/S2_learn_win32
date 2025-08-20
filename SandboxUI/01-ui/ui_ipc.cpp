/**
******************************************************************************
* @file    IPC.CPP
* @author  ARTHUR
* @version V1
* @date    MAY 26, 2025
* @brief   use shared memory as the IPC solution
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
#include <windows.h>
#include "ui_ipc.h" 
#include "config.h" //to read mode
#include "../03-core/core_all_includes.h" //to read all states

#include "../03-core/service_queue.h"
#include "ui.h" //to print data

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
static HANDLE          _h_mutex; //the mutex to protect the shmem data from multiple accesses at a time
static IPC_BIG_DATA*   _p_big_data = NULL; //pointer to shared memory
static int             _mode; //host running mode, master or slave1/2/3
static int             _is_slave_initiated = 0; //this is used to tackel the slave initilization problem, slave won't succeed until master has created the shmem


/*******************************************************************************
************************** Private function prototypes *************************
*******************************************************************************/

/*******************************************************************************
******************************* Private functions ******************************
*******************************************************************************/

/*******************************************************************************
* @brief  master create the shared memory
* @param  xxxx
* @retval xxxx
*******************************************************************************/
void _ipc_init_master(void) {
    //[1] create file mapping
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(IPC_BIG_DATA), SHMEM_NAME);
    if (!hMapFile) {
        ui_internal_printf("CreateFileMapping failed (%lu)\n", GetLastError());
        return;
    }
    //[2] create view of the file
    _p_big_data = (IPC_BIG_DATA*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(IPC_BIG_DATA));
    if (!_p_big_data) {
        ui_internal_printf("MapViewOfFile failed (%lu)\n", GetLastError());
        return;
    }
    //[3] create mutex for data access safety
    _h_mutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
    if (!_h_mutex) {
        ui_internal_printf("CreateMutex failed (%lu)\n", GetLastError());
        return;
    }
}


/*******************************************************************************
* @brief  xxxx
* @param  xxxx
* @retval xxxx
*******************************************************************************/
void _ipc_init_slave(void) {
    if (_is_slave_initiated == 1) {
        return;
    }

    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SHMEM_NAME);
    if (!hMapFile) {
        ui_internal_printf("OpenFileMapping failed (%lu)\n", GetLastError());
        Sleep(1000);
        return;
    }

    _p_big_data = (IPC_BIG_DATA*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(IPC_BIG_DATA));
    if (!_p_big_data) {
        ui_internal_printf("MapViewOfFile failed (%lu)\n", GetLastError());
        Sleep(1000);
        return;
    }

    _h_mutex = OpenMutex(SYNCHRONIZE, FALSE, MUTEX_NAME);
    if (!_h_mutex) {
        ui_internal_printf("OpenMutex failed (%lu)\n", GetLastError());
        Sleep(1000);
        return;
    }

    _is_slave_initiated = 1;
}


/*******************************************************************************
******************************* Public Variables *******************************
*******************************************************************************/

/*******************************************************************************
******************************* Public functions *******************************
*******************************************************************************/



/*******************************************************************************
* @brief  master reads data from global memory and also sends data there
* 
*         big data->group control
*         core->big data
* @param  xxxx
* @retval xxxx
*******************************************************************************/
void ipc2_master_read_write(void){
    if (_mode == ELEVATOR_MODE_MASTER) {
        WaitForSingleObject(_h_mutex, INFINITE);
        //[1]read slaves data to local
        g_shareddata.slave1 = _p_big_data->slave1; //hrere g_sharedata hold the slave status data

        //[2]write local core data to shared memory
        _p_big_data->ts_ms = ui64_get_ui_run_ms();
        _p_big_data->master.is_in_group_mode = 1;

        ReleaseMutex(_h_mutex);
    }

}


/*******************************************************************************
* @brief  called in app.cpp to collect data and posts to shared memory 
* @param  xxxx
* @retval xxxx
*******************************************************************************/
void ipc3_slave_collect_data_to_shmem(void){
    if (_mode == ELEVATOR_MODE_SLAVE1
    || _mode == ELEVATOR_MODE_SLAVE2
    || _mode == ELEVATOR_MODE_SLAVE3) {
        _ipc_init_slave();

        if (_is_slave_initiated == 0) {
            return;
        }
        WaitForSingleObject(_h_mutex, INFINITE);
        //[1]read master data from shared memory
        g_shareddata.ts_ms  = _p_big_data->ts_ms;
        g_shareddata.master = _p_big_data->master;

        //[2]core data to shared memory
        int32_t lv1_state;
        lv1_state                                 = fsm_lv1_get_current_state();
        g_shareddata.slave1.is_idle_and_available = (lv1_state == LV1_STATE_IDLE) ? 1 : 0;
        if (g_core_input_stru.current_landing > 0) {
            g_shareddata.slave1.current_landing = g_core_input_stru.current_landing;
        }

        g_shareddata.slave1.lv2_state = fsm_lv2_get_current_state();
        g_shareddata.slave1.lv3_state = fsm_lv3_get_current_state();
        g_shareddata.slave1.lv4_state = fsm_lv4_get_current_state();
        g_shareddata.slave1.door_state            = lv3_get_door_state();
        serviceq89_get_range2(&(g_shareddata.slave1.range2));
        _p_big_data->slave1 = g_shareddata.slave1; //from local to shmem
    
        ReleaseMutex(_h_mutex);
    }
}


/*******************************************************************************
* @brief  called in ElevatorSimulatorV2.cpp at WM_CREATE event
* @param  xxxx
* @retval xxxx
*******************************************************************************/
void ipc1_init(void) {
    ui42_config_get_parameter("Mode", &(_mode)); //mode=value like ELEVATOR_MODE_MASTER
    if (_mode == ELEVATOR_MODE_MASTER) {
        _ipc_init_master();
    }
    else if(_mode == ELEVATOR_MODE_SLAVE1 
         || _mode == ELEVATOR_MODE_SLAVE2 
         || _mode == ELEVATOR_MODE_SLAVE3){
        _ipc_init_slave();
    }
    else {
        //no ipc needed for simplex mode.
    }
}

/********************************* end of file ********************************/

