/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ��̬��������
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
/* ������Ӳ��bspͷ�ļ� */
//#include "bsp_led.h"
#include "bsp_usart.h"

/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */

/* LED������ */
static TaskHandle_t Task_Handle = NULL;

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */


/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
StackType_t xTask3Stack[100];
StaticTask_t xTask3TCB;

StackType_t xIdleTaskStack[100];
StaticTask_t xIdleTaskTCB;

/*
*************************************************************************
*                             ��������
*************************************************************************
*/

static void TaskWithParam(void* pvParameters);//����ͬһ����������ͬ����������ͬ����
static void printf2(void* pvParameters);
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                               StackType_t ** ppxIdleTaskStackBuffer,
                                               uint32_t * pulIdleTaskStackSize );


/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
  	USART_Config();

	xTaskCreate((TaskFunction_t )TaskWithParam, /* ������ں��� */
                        (const char*    )"TASK1",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )1,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&Task_Handle);/* ������ƿ�ָ�� */

  
    xTaskCreate((TaskFunction_t )TaskWithParam, /* ������ں��� */
                        (const char*    )"Task2",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )4,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        NULL);/* ������ƿ�ָ�� */

	xTaskCreateStatic((TaskFunction_t )printf2, /* ������ں��� */
                        (const char*    )"Task3",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )4,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        xTask3Stack,
                        &xTask3TCB);/* ������ƿ�ָ�� */




						
    vTaskStartScheduler();   /* �������񣬿������� */
 
  
  while(1);   /* ��������ִ�е����� */    
}


void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                               StackType_t ** ppxIdleTaskStackBuffer,
                                               uint32_t * pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
	*ppxIdleTaskStackBuffer = xIdleTaskStack;
	*pulIdleTaskStackSize = 100;
}



static void TaskWithParam(void* param)
{
	int val = (int)param;
	while(1)
	{
		printf("%d", val);
	}
}

static void printf2(void* param)
{
	while(1)
	{
		printf("2");
	}
}
/********************************END OF FILE****************************/
