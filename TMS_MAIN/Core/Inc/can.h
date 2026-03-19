/* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file    can.h
//  * @brief   This file contains all the function prototypes for
//  *          the can.c file
//  ******************************************************************************
//  * @attention
//  *
//  * Copyright (c) 2023 STMicroelectronics.
//  * All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  * If no LICENSE file comes with this software, it is provided AS-IS.
//  *
//  ******************************************************************************
//  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "constants.h"
#include "message_buffer.h"
//#include "uvfr_utils.h"
/* USER CODE END Includes */

#define _LONGEST_SC_TIME 300
#define _SC_DAEMON_PERIOD 10


extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

#define DEBUG_LED_Pin GPIO_PIN_12
/* USER CODE BEGIN Private defines */

#define CAN_TX_DAEMON_NAME "CanTxDaemon"
#define CAN_RX_DAEMON_NAME "CanRxDaemon"

//typedef struct uv_CAN_msg uv_CAN_msg;
typedef enum uv_status_t uv_status;

/* USER CODE END Private defines */

void MX_CAN1_Init(void);
void MX_CAN2_Init(void);

#define UV_CAN_CRIT_MSG_BIT 0b10000000


typedef struct uv_CAN_msg{
	uint8_t flags; /**< Bitfield that contains some basic information about the message:
	-Bit 0: Is the message an extended ID message, or a standard ID message? 1 For extended.
	-Bits 1:2 Which CANbus is being used to send the message? 00-> whatever the default is 01 -> CAN1 10 -> CAN2 11-> CAN3 (doesnt exist yet). Will default to CAN1 if all zeros*/

	uint8_t dlc; /**<Data Length Code, representing how many bytes of data are present*/
	uint32_t msg_id; /**<The ID of a message*/
	uint8_t data[8]; /**<The actual data packet contained within the CAN message */
}uv_CAN_msg;

/* USER CODE BEGIN Prototypes */

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan2);
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan2);

uv_status uvSendCanMSG(uv_CAN_msg * msg);

void CANbusTxSvcDaemon(void* args);
void CANbusRxSvcDaemon(void* args);

void start_CANTxSvcDaemon(void);
void start_CANRxSvcDaemon(void);

#define CAN_BUS_1 0b00000010
#define CAN_BUS_2 0b00000100

#define UV_CAN_EXTENDED_ID 0b00000001
#define UV_IDE_BIT UV_CAN_EXTENDED_ID

void _shutDownTask(void* args);

typedef enum uv_status_t{
	UV_OK,
	UV_WARNING,
	UV_ERROR,
	UV_ABORTED //nothing wrong per say,
}uv_status;

typedef uint32_t uv_timespan_ms;


//uvfr state engine
//void killSelf(struct uv_task_info * t);
//
//void suspendSelf(struct uv_task_info * t);


#ifndef uvMalloc
#if USE_OS_MEM_MGMT
	void* __uvMallocOS(size_t memrequest);
	#define uvMalloc(x) __uvMallocOS(x)
#else //default to STDlib
	void * __uvMallocCritSection(size_t memrequest);
	#define uvMalloc(x) __uvMallocCritSection(x)

#endif //OS mem mgmt?
#endif //allow macro override



typedef uint8_t uv_task_id; //WHY DO I NEED TO DO THIS STUPID REDEFINITION HERE

/** @brief Struct to contain data about a parent task
 *
 * This contains the information required for the child task to communicate with it's parent.
 *
 * This will be a queue, since one parent task can in theory have several child tasks
 *
 */
typedef struct task_management_info{
	TaskHandle_t task_handle; /**<Actual handle of parent */
	QueueHandle_t parent_msg_queue; /**< */
}task_management_info;


/** @brief Special commands used to start and shutdown tasks.
 *
 */
typedef enum uv_task_cmd_e{
	UV_NO_CMD,/**<The SCD has issued no command, and therefore no action is required */
	UV_KILL_CMD, /**< The SCD has decreed that this task must be deleted */
	UV_SUSPEND_CMD, /**< The SCD has decreed that this task must be suspended*/
	UV_TASK_START_CMD /**< OK for task to begin execution*/
}uv_task_cmd;


/** @brief Enum representing the state of a managed task.
 *
 * This is used as a flag to indicate whether or not the state_engine is aware of a task is running or not.
 *
 */
typedef enum uv_task_state_t{
	UV_TASK_NOT_STARTED,
	UV_TASK_DELETED,
	UV_TASK_RUNNING,
	UV_TASK_SUSPENDED
} uv_task_status;

void _stateChangeDaemon(void * args);


/** @brief This struct is designed to hold neccessary information about an RTOS task that
 * will be managed by uvfr_state_engine.
 *
 * Pay close attention, because this is one of the most cursed structs in the project, as well as one of the most important
 */
typedef struct uv_task_info{
	uv_task_id task_id; /**< Detailed description after the member */
	char* task_name; /**< Detailed description after the member */

	uv_timespan_ms task_period; /**< Maximum period between task execution*/
	uv_timespan_ms deletion_delay; /**< If deferred deletion is enabled, how long to wait before we delete task? */

	TaskFunction_t task_function; /**< Pointer to function that implements the task */
	osPriority task_priority; /**< Priority of the task. Int between 0 and 7 */


	uint32_t stack_size; /**< Number of words allocated to the stack of the task */




	uv_task_status task_state; //tracks the internal state of the task


	TaskHandle_t task_handle; /**< Handle of freeRTOS task control block */

	uv_task_cmd cmd_data; /**< how we communicate with the task rn - THIS SUCKS SO BAD */

	void* task_args; /**< arguments for the specific task, this is where we will likely pass in task settings */

	struct uv_task_info_t* parent;/**< info about the parent of the task */

	task_management_info* tmi; /**< how we will be communicating in the future */
	MessageBufferHandle_t task_rx_mailbox; /**< Incoming messages for this task*/
	TickType_t last_execution_time;

	uint16_t active_states; //corresponds to the vehicle states where the task should be active
	uint16_t deletion_states; //corresponds to the vehicle states where the task should be suspended
	uint16_t suspension_states; //when should the task be suspended? When it should exist, but shouldnt be active.

	uint16_t task_flags; /**<
		- Bits 0:1 - | Task MGMT | Vehicle Application task - 01 | Periodic SVC Task - 10 | Dormant SVC Task - 11
		- Bit 2  - Log task start + stop time
		- Bit 3  - Log mem usage
		- Bit 4  - SCD ignore flag (only use if task is application layer
		- Bit 5  - is parent
		- Bit 6	 - is child
		- Bit 7  - is orphaned
		- Bit 8	 - error in child task
		- Bit 9	 - awaiting deferred deletion
		- Bit 10 - deferred deletion enabled
		- Bits 11:12 - Deadline firmness | No enforcement - 00 | Gradual Priority Incrimentation - 01 | Firm deadline 10 | Critical Deadline - 11
		- Bit 13 - mission critical, if this specific task crashes, the car will not continue to run
		- Bit 14 - Task currently delaying, either by vTaskDelay or vTaskDelayUntil
		 */

	uint8_t throttle_factor; /**< How much to throttle the task */
}uv_task_info;

uv_status uvIsPTRValid(void* ptr);


#ifndef uvFree
#if USE_OS_MEM_MGMT
	uv_status __uvFreeOS(void* ptr);
	#define uvFree(x) __uvFreeOS(x)

#else
	uv_status __uvFreeCritSection(void* ptr);
	#define uvFree(x) __uvFreeCritSection(x)

#endif //OS mem mgmt
#endif //Allows macro overriding


/** @brief Response from a task confirming it has been either deleted or suspended
 *
 */
enum uv_scd_response_e{
	UV_SUCCESSFUL_DELETION, /**< Returned when a task was successfully deleted*/
	UV_SUCCESSFUL_SUSPENSION,/**< Returned when a task is successfully suspended*/
	UV_COULDNT_DELETE, /**< Task was not successfully deleted*/
	UV_COULDNT_SUSPEND, /**< Task was not successfully suspended*/
	UV_UNSAFE_STATE /**< Task has ended up in a fucked middle ground state*/
};

typedef struct uv_scd_response{
	enum uv_scd_response_e response_val; /**< */
	uv_task_id meta_id; /**< */
}uv_scd_response;

//void uvPanic(char* msg, uint8_t msg_len); //ruh roh scoobs, something has gone a little bit fucky wucky
//void __uvPanic(char* msg, uint8_t msg_len, const char* file, const int line, const char* func);
void __uvPanic(void);

#ifndef uvPanic
/** @brief Called when things have gone heinously wrong, and we would like to get off the ride
 *
 * This function is called in the event of a non-recoverable error. It puts the vehicle into a safe state, logs the error, and changes over
 * vehicle state.
 *
 */
//#define uvPanic(msg, errnum) __uvPanic(msg, errnum, __UV_FILENAME__,__LINE__,__FUNCTION__)

#define uvPanic(void) __uvPanic(void)
#endif

/**@addtogroup state_engine_api
 * @{
 */


//maybe delete??
/** @brief Type representing the overall state and operating mode of the vehicle.
 *
 * Type made to represent the state of the vehicle, and the location in the state machine
 *	The states are powers of two to make it easier to discern tasks that need to happen in multiple states
 */
typedef enum uv_vehicle_state_t{
	UV_INIT = 0x0001, /**< Vehicle is in the process of initializing */
	UV_READY = 0x0002, /**< Vehicle has initialized and is ready to drive*/
	PROGRAMMING = 0x0004, /**< The settings of the vehicle are being edited now*/
	UV_DRIVING = 0x0008, /**< The vehicle is actively driving*/
	UV_SUSPENDED = 0x0010, /**< The vehicle is not allowed to produce any torque, but not full shutdown*/
	UV_LAUNCH_CONTROL = 0x0020, /**< The vehicle is presently in launch control mode*/
	UV_ERROR_STATE = 0x0040, /**< Some error has occurred here*/
	UV_BOOT = 0x0080, /**< Pre-init, when the boot loader is going*/
	UV_HALT = 0x0100 /**< Stop literally everything, except for what is needed to reset vehicle*/
}uv_vehicle_state;



//FROM UVFR vehicle logger

typedef enum {
    FAULT_PANIC = 0,
    FAULT_TASK_FAIL,
    FAULT_TASK_LATE,
    FAULT_STATE_ILLEGAL,
    FAULT_SCD_TIMEOUT,
    FAULT_SENSOR_ERROR,
    FAULT_STATE_TRANSITION,
    FAULT_MANUAL,
    FAULT_FLASH_ERROR,
    FAULT_SETTINGS_CORRUPT,
    FAULT_CAN_MSG_FAIL,
    // Add more if needed?
} fault_event_type_e;

uv_status changeVehicleState();


extern

void insertCANMessageHandler(uint32_t id, void* handlerfunc, int can_num);
void nuke_hash_table(int can_num);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

