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
char PUB_BUF[255];  //上传的数据存储在PUB_BUF中统一发送
const char *decSubtopic[] ={"yqq/sub"};//订阅主题   设备订阅命令
const char decPubtopic[] = "yqq/pub";//发布消息   设备上行数据
unsigned short timeCount = 0;	//发送间隔变量
unsigned char *dataPtr = NULL;
extern float Pitch,Roll,Yaw;
int main(void)
{	
	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化B5连接的硬件接口
	LED1_Init();			//初始化C13连接的硬件接口
	BEEP_Init();			//蜂鸣器接口初始化
	DHT11_Start();				//温湿度接口初始化
	Lcd_Init();					//7针Lcd接口初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	EXTIX_Init();				//外部中断初始化
	LCD_Clear(BLACK);		//LCD清屏
	base_chinese();			//显示基本界面
	KEY_Init();					//按键初始化
	Usart1_Init(115200);//debug串口
	Usart2_Init(115200);//esp8266通信串口
	TIM2_Int_Init(4999,7199);		//定时器2初始化
	TIM3_Int_Init(9999,719);		//定时器3初始化
	IWDG_Init(7,4000);
	IIC_Init();
	MPU6050_initialize();     //=====MPU6050初始化	
	DMP_Init();	
	UsartPrintf(USART1,"hardware init is ok\n");
	ESP8266_Init();			//esp8266初始化
	OneNet_DevLink();
	BEEP=1;
	delay_ms(500);
	BEEP=0;
	OneNet_Subscribe(decSubtopic, 1);

	while(1)
	{ 
		IWDG_Feed();
		//UsartPrintf(USART_DEBUG,"dht11_flag_free:%d",dht11_flag_free);
			if(++timeCount == 40)		// 1000ms / 25ms  =40 意味着1秒执行一次
			{
				//读取温湿度传感器数据
				DHT11_REC_Data(); 		
				UsartPrintf(USART1,"humi:%d.%d  temp:%d.%d\n" ,rec_data[0],rec_data[1],rec_data[2],rec_data[3]);
				UsartPrintf(USART1,"X=%.1f \r Y=%.1f \r Z=%.1f\n",Pitch,Roll,Yaw);
				LED_SW=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);
				
				//报警条件判断
				if(dht11_flag_free==10)
				{
					if(rec_data[0]<85 && rec_data[2]<35 && rec_data[2]>0 ) dht11_flag=0;		//湿度小于40且温度大于0小于30时不报警否则报警  
					else dht11_flag=1;
				}
				
				//手动控制时间
				if(dht11_flag_free<10) dht11_flag_free++;
				timeCount = 0;
				num++;
			}

			if(num == 5)									//发送间隔5s
			{
				UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
				sprintf(PUB_BUF,"{\"humi\":%d.%d,\"temp\":%d.%d,\"X\":%.2f,\"Y\":%.2f,\"Z\":%.2f,\"LED_SW\":%d,\"BEEP_SW\":%d}",rec_data[0],rec_data[1],rec_data[2],rec_data[3],Pitch,Roll,Yaw,LED_SW,dht11_flag);

				//sprintf(PUB_BUF,"{\"humi\":%d.%d,\"temp\":%d.%d,\"LED_SW\":%d,\"BEEP_SW\":%d}",rec_data[0],rec_data[1],rec_data[2],rec_data[3],LED_SW,dht11_flag);
				OneNet_Publish(decPubtopic, PUB_BUF);
				
				num=0;
				ESP8266_Clear();
			}
		
			dataPtr = ESP8266_GetIPD(3);				//耗费15ms
			if(dataPtr != NULL)
				OneNet_RevPro(dataPtr);
			
			delay_ms(10);					//耗费10ms

		}
	
}



