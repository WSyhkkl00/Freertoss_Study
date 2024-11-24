#include "main.h"

#include "FreeRTOS.h"

#include "task.h"

#define START_TASK_PRIO 1

#define START_STK_SZIE 128

TaskHandle_t START_Handle_t;

#define LED_TASK_PRIO 3

#define LED_STK_SZIE 128

TaskHandle_t LED_Handle_t;

void Start_Task(void *p);

void LED_Task(void *p);

void FreeRTOS_Dome(void)

{

        xTaskCreate(Start_Task,"Start_Task",START_STK_SZIE,NULL,START_TASK_PRIO,&START_Handle_t);

        vTaskStartScheduler();

}

void Start_Task(void *p)

{

        taskENTER_CRITICAL();

        xTaskCreate(LED_Task,"LED_Task",LED_STK_SZIE,NULL,LED_TASK_PRIO,&LED_Handle_t);

        vTaskDelete(START_Handle_t);

        taskEXIT_CRITICAL();

}

void LED_Task(void *p)

{

        while(1)

        {

                HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);

                vTaskDelay(500);

                HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);

                vTaskDelay(500);

        }

}
