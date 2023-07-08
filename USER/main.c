#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "dht11.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "exti.h"
#include "esp8266.h"
#include "onenet.h"
#include "MPU6050.h"
#include "wdg.h"
#include <stdio.h>
extern int rec_data[4];
int dht11_flag;
int dht11_flag_free=10;
int key;
int num =0;
int LED_SW;
char PUB_BUF[255];  //�ϴ������ݴ洢��PUB_BUF��ͳһ����
const char *decSubtopic[] ={"yqq/sub"};//��������   �豸��������
const char decPubtopic[] = "yqq/pub";//������Ϣ   �豸��������
unsigned short timeCount = 0;	//���ͼ������
unsigned char *dataPtr = NULL;
extern float Pitch,Roll,Yaw;
int main(void)
{	
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ��B5���ӵ�Ӳ���ӿ�
	LED1_Init();			//��ʼ��C13���ӵ�Ӳ���ӿ�
	BEEP_Init();			//�������ӿڳ�ʼ��
	DHT11_Start();				//��ʪ�Ƚӿڳ�ʼ��
	Lcd_Init();					//7��Lcd�ӿڳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	EXTIX_Init();				//�ⲿ�жϳ�ʼ��
	LCD_Clear(BLACK);		//LCD����
	base_chinese();			//��ʾ��������
	KEY_Init();					//������ʼ��
	Usart1_Init(115200);//debug����
	Usart2_Init(115200);//esp8266ͨ�Ŵ���
	TIM2_Int_Init(4999,7199);		//��ʱ��2��ʼ��
	TIM3_Int_Init(9999,719);		//��ʱ��3��ʼ��
	IWDG_Init(7,4000);
	IIC_Init();
	MPU6050_initialize();     //=====MPU6050��ʼ��	
	DMP_Init();	
	UsartPrintf(USART1,"hardware init is ok\n");
	ESP8266_Init();			//esp8266��ʼ��
	OneNet_DevLink();
	BEEP=1;
	delay_ms(500);
	BEEP=0;
	OneNet_Subscribe(decSubtopic, 1);

	while(1)
	{ 
		IWDG_Feed();
		//UsartPrintf(USART_DEBUG,"dht11_flag_free:%d",dht11_flag_free);
			if(++timeCount == 40)		// 1000ms / 25ms  =40 ��ζ��1��ִ��һ��
			{
				//��ȡ��ʪ�ȴ���������
				DHT11_REC_Data(); 		
				UsartPrintf(USART1,"humi:%d.%d  temp:%d.%d\n" ,rec_data[0],rec_data[1],rec_data[2],rec_data[3]);
				UsartPrintf(USART1,"X=%.1f \r Y=%.1f \r Z=%.1f\n",Pitch,Roll,Yaw);
				LED_SW=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);
				
				//���������ж�
				if(dht11_flag_free==10)
				{
					if(rec_data[0]<85 && rec_data[2]<35 && rec_data[2]>0 ) dht11_flag=0;		//ʪ��С��40���¶ȴ���0С��30ʱ���������򱨾�  
					else dht11_flag=1;
				}
				
				//�ֶ�����ʱ��
				if(dht11_flag_free<10) dht11_flag_free++;
				timeCount = 0;
				num++;
			}

			if(num == 5)									//���ͼ��5s
			{
				UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
				sprintf(PUB_BUF,"{\"humi\":%d.%d,\"temp\":%d.%d,\"X\":%.2f,\"Y\":%.2f,\"Z\":%.2f,\"LED_SW\":%d,\"BEEP_SW\":%d}",rec_data[0],rec_data[1],rec_data[2],rec_data[3],Pitch,Roll,Yaw,LED_SW,dht11_flag);

				//sprintf(PUB_BUF,"{\"humi\":%d.%d,\"temp\":%d.%d,\"LED_SW\":%d,\"BEEP_SW\":%d}",rec_data[0],rec_data[1],rec_data[2],rec_data[3],LED_SW,dht11_flag);
				OneNet_Publish(decPubtopic, PUB_BUF);
				
				num=0;
				ESP8266_Clear();
			}
		
			dataPtr = ESP8266_GetIPD(3);				//�ķ�15ms
			if(dataPtr != NULL)
				OneNet_RevPro(dataPtr);
			
			delay_ms(10);					//�ķ�10ms

		}
	
}



