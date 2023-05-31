#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
/*

红色按键KEY0按下去时输出信号为1
默认为0
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
红色按键上拉输入,上升沿触发
高电平触发

蓝色按键KEY1按下去时输出信号为0
默认为1
蓝色按键下拉输入,下降沿触发
低电平触发
*/


//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟
	
	
	//初始化 KEY0-->GPIOB.0	  上拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//KEY0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0设置成输入，默认上拉  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.0

	
	//初始化 KEY1-->GPIOB.1   上拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.1

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//注意此函数有响应优先级,KEY0>KEY1!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//按键按松开标志
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(KEY0==1||KEY1==0))
//	{
//		delay_ms(10);//去抖动 
//		key_up=0;
//		if(KEY0==1)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//	}else if(KEY0==0&&KEY1==1)key_up=1; 	    
// 	return 0;// 无按键按下
//}
