#ifndef __LED_H
#define __LED_H	 

#include "sys.h"


#define LED0 PBout(9)// PB9
#define LED1 PCout(13)// PC13

void LED_Init(void);//初始化B9
void LED1_Init(void);//初始化C13
		 				    
#endif
