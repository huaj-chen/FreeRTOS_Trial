/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 动态创建任务
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 STM32全系列开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
/* 开发板硬件bsp头文件 */
//#include "bsp_led.h"
#include "bsp_usart.h"

/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
 /* 创建任务句柄 */

/* LED任务句柄 */
static TaskHandle_t Task_Handle = NULL;
static TaskHandle_t Task3_Handle = NULL;

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */


/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
StackType_t xTask3Stack[100];
StaticTask_t xTask3TCB;

StackType_t xIdleTaskStack[100];
StaticTask_t xIdleTaskTCB;

static int Task1RunFlag = 0;
static int Task2RunFlag = 0;
static int Task3RunFlag = 0;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void Task1(void* param);
static void Task2(void* param);
static void printf3(void* pvParameters);//静态分配需要的函数
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                               StackType_t ** ppxIdleTaskStackBuffer,
                                               uint32_t * pulIdleTaskStackSize );


/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void)
{	
  	USART_Config();

	xTaskCreate((TaskFunction_t )Task1, /* 任务入口函数 */
                        (const char*    )"TASK1",/* 任务名字 */
                        (uint16_t       )100,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&Task_Handle);/* 任务控制块指针 */

  
    xTaskCreate((TaskFunction_t )Task2, /* 任务入口函数 */
                        (const char*    )"Task2",/* 任务名字 */
                        (uint16_t       )100,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        NULL);/* 任务控制块指针 */

	Task3_Handle = xTaskCreateStatic((TaskFunction_t )printf3, /* 任务入口函数 */
                        (const char*    )"Task3",/* 任务名字 */
                        (uint16_t       )100,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        xTask3Stack,
                        &xTask3TCB);/* 任务控制块指针 */




						
    vTaskStartScheduler();   /* 启动任务，开启调度 */
 
  
  while(1);   /* 正常不会执行到这里 */    
}


void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                               StackType_t ** ppxIdleTaskStackBuffer,
                                               uint32_t * pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
	*ppxIdleTaskStackBuffer = xIdleTaskStack;
	*pulIdleTaskStackSize = 100;
}



static void Task1(void* param)
{
	TickType_t tstart = xTaskGetTickCount();
	TickType_t t;
	int flag = 0;
	while(1)
	{
		printf("1");
		Task1RunFlag = 1;
		Task2RunFlag = 0;
		Task3RunFlag = 0;
		t= xTaskGetTickCount();
		
		if(!flag && (t >= tstart + 10))
		{
			vTaskSuspend(Task3_Handle);
			flag = 1;
		}
		if(t >= tstart + 20)
		{
			vTaskResume(Task3_Handle);
			

		}
	
	}
}

static void Task2(void* param)
{
	while(1)
	{
		Task1RunFlag = 0;
		Task2RunFlag = 1;
		Task3RunFlag = 0;
		printf("2");
		vTaskDelay(10);
	}
}

static void printf3(void* param)
{
	while(1)
	{
		Task1RunFlag = 0;
		Task2RunFlag = 0;
		Task3RunFlag = 1;
		printf("3");
	}
}
/********************************END OF FILE****************************/
