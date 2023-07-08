
#include "lcd.h"
#include "font.h"
#include "delay.h"
u16 BACK_COLOR;   //����ɫ


/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
	u8 i;
	LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		   LCD_SDIN_Set();
		else
		   LCD_SDIN_Clr();
		LCD_SCLK_Set();
		dat<<=1;
	}	
  LCD_CS_Set();	
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_DC_Set();//д����
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_DC_Set();//д����
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
}
/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
}


/******************************************************************************
      ����˵����LCD��ʼ������
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void Lcd_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��B�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_10;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_10);	
	

	LCD_RST_Clr();
	delay_ms(20);
	LCD_RST_Set();
	delay_ms(20);
	LCD_BLK_Set();
	
LCD_WR_REG(0x11);              
delay_ms(120); 

LCD_WR_REG(0x36); //MX, MY, RGB mode 
if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
else LCD_WR_DATA8(0xA0);

LCD_WR_REG(0x3A);     
LCD_WR_DATA8(0x55);   

LCD_WR_REG(0xB2);     
LCD_WR_DATA8(0x1F);   
LCD_WR_DATA8(0x1F);   
LCD_WR_DATA8(0x00);   
LCD_WR_DATA8(0x33);   
LCD_WR_DATA8(0x33);  

LCD_WR_REG(0xB7);    
LCD_WR_DATA8(0x12);  //VGH=12.54V,VGL=-8.23V 

LCD_WR_REG(0xBB);     
LCD_WR_DATA8(0x35);  

LCD_WR_REG(0xC0);     
LCD_WR_DATA8(0x2C); 

LCD_WR_REG(0xC2);     
LCD_WR_DATA8(0x01);   

LCD_WR_REG(0xC3);     
LCD_WR_DATA8(0x15); //4.6V 

LCD_WR_REG(0xC4);     
LCD_WR_DATA8(0x20);   //VDV, 0x20:0v

LCD_WR_REG(0xC6);     
LCD_WR_DATA8(0x13);     

LCD_WR_REG(0xD0);     
LCD_WR_DATA8(0xA4);   
LCD_WR_DATA8(0xA1);   

LCD_WR_REG(0xD6);     
LCD_WR_DATA8(0xA1);   //sleep in��gate���ΪGND

LCD_WR_REG(0xE0);
LCD_WR_DATA8(0xF0);
LCD_WR_DATA8(0x06);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x0B);
LCD_WR_DATA8(0x0A);
LCD_WR_DATA8(0x07);
LCD_WR_DATA8(0x2E);
LCD_WR_DATA8(0x43);
LCD_WR_DATA8(0x45);
LCD_WR_DATA8(0x38);
LCD_WR_DATA8(0x14);
LCD_WR_DATA8(0x13);
LCD_WR_DATA8(0x25);
LCD_WR_DATA8(0x29);

LCD_WR_REG(0xE1);
LCD_WR_DATA8(0xF0);
LCD_WR_DATA8(0x07);
LCD_WR_DATA8(0x0A);
LCD_WR_DATA8(0x08);
LCD_WR_DATA8(0x07);
LCD_WR_DATA8(0x23);
LCD_WR_DATA8(0x2E);
LCD_WR_DATA8(0x33);
LCD_WR_DATA8(0x44);
LCD_WR_DATA8(0x3A);
LCD_WR_DATA8(0x16);
LCD_WR_DATA8(0x17);
LCD_WR_DATA8(0x26);
LCD_WR_DATA8(0x2C);  

LCD_WR_REG(0xE4);    
LCD_WR_DATA8(0x1D); //ʹ��240��gate  (N+1)*8
LCD_WR_DATA8(0x00); //�趨gate���λ��
LCD_WR_DATA8(0x00); //��gateû������ʱ��bit4(TMG)��Ϊ0

LCD_WR_REG(0x21);        

LCD_WR_REG(0x2A);     //Column Address Set
LCD_WR_DATA8(0x00);   
LCD_WR_DATA8(0x00);   //0
LCD_WR_DATA8(0x00);   
LCD_WR_DATA8(0xEF);   //239

LCD_WR_REG(0x2B);     //Row Address Set
LCD_WR_DATA8(0x00);   
LCD_WR_DATA8(0x00);   //0
LCD_WR_DATA8(0x00);   
LCD_WR_DATA8(0xEF);   //239

LCD_WR_REG(0x2C);     

LCD_WR_REG(0x29); //Display on

} 


/******************************************************************************
      ����˵����LCD��������
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	  {
			for (j=0;j<LCD_H;j++)
				{
					LCD_WR_DATA(Color);
				}
	  }
}
/******************************************************************************
      ����˵����LCD��ʾ����
      ������ݣ�x,y   ��ʼ����
                index ���ֵ����
                size  �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 index,u8 size,u16 color)	
{  
	u8 i,j;
	u8 *temp,size1;
	if(size==16){temp=Hzk16;}//ѡ���ֺ�
	if(size==32){temp=Hzk32;}//ѡ���ֺ�
  LCD_Address_Set(x,y,x+size-1,y+size-1); //����һ�����ֵ�����
  size1=size*size/8;//һ��������ռ���ֽ�
	temp+=index*size1;//д�����ʼλ��
	for(j=0;j<size1;j++)
	{
		for(i=0;i<8;i++)
		{
		 	if((*temp&(1<<i))!=0)//�����ݵĵ�λ��ʼ��
			{
				LCD_WR_DATA(color);//����
			}
			else
			{
				LCD_WR_DATA(BACK_COLOR);//������
			}
		}
		temp++;
	 }
}



/******************************************************************************
      ����˵����LCD��ʾ����
      ������ݣ�x,y   ��ʼ����
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(color);
} 
/******************************************************************************
      ����˵����LCD��һ����ĵ�
      ������ݣ�x,y   ��ʼ����
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint_big(u16 x,u16 y,u16 color)
{
	LCD_Fill(x-1,y-1,x+1,y+1,color);
} 
/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
      ����ֵ��  ��
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//���ù��λ�� 	    
	} 					  	    
}
/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
/******************************************************************************
      ����˵����������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}
/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
      ����ֵ��  ��
******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}

/******************************************************************************
      ����˵������ʾ�ַ�
      ������ݣ�x,y    �������
                num    Ҫ��ʾ���ַ�
                mode   1���ӷ�ʽ  0�ǵ��ӷ�ʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode,u16 color)
{
    u8 temp;
    u8 pos,t;
	  u16 x0=x;    
    if(x>LCD_W-16||y>LCD_H-16)return;	    //���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_WR_DATA(color);
				else LCD_WR_DATA(BACK_COLOR);
				temp>>=1;
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos,color);//��һ����     
		        temp>>=1; 
		    }
		}
	}   	   	 	  
}



/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y    �������
                *p     �ַ�����ʼ��ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 color)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0,color);
        x+=8;
        p++;
    }  
}


/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}


/******************************************************************************
      ����˵������ʾ����
      ������ݣ�x,y    �������
                num    Ҫ��ʾ������
                len    Ҫ��ʾ�����ָ���
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowNum(u16 x,u16 y,u16 num,u8 len,u16 color)
{         	
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0,color);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0,color); 
	}
} 



/******************************************************************************
      ����˵������ʾС��
      ������ݣ�x,y    �������
                num    Ҫ��ʾ��С��
                len    Ҫ��ʾ�����ָ���
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowNum1(u16 x,u16 y,float num,u8 len,u16 color)
{         	
	u8 t,temp;
	u16 num1;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+8*(len-2),y,'.',0,color);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0,color);
	}
}


/******************************************************************************
      ����˵������ʾ40x40ͼƬ
      ������ݣ�x,y    �������
      ����ֵ��  ��
******************************************************************************/
void showimage(const unsigned char *p) //��ʾ40*40 QQͼƬ
{
  	int i,j=1,k=1;
		int m=120;
	unsigned char picH,picL;
	LCD_Clear(WHITE); //����  
	
	
			LCD_Address_Set(m*j,m*k,m*j+m-1,m*k+m-1);		//��������
	//LCD_Address_Set(120,0,239,120);
		    for(i=0;i<m*m;i++)
			 {	
			 	picL=*(p+i*2);	//���ݵ�λ��ǰ
				picH=*(p+i*2+1);				
				LCD_WR_DATA(picH<<8|picL);  						
			 }			
}

void base_chinese(void)
{
		LCD_ShowChinese(0,32,0,16,WHITE);  //��
		LCD_ShowChinese(16,32,1,16,WHITE);	//��
		LCD_ShowChinese(0,48,2,16,WHITE);		//ʪ
		LCD_ShowChinese(16,48,3,16,WHITE);	//��
		LCD_ShowChinese(32,32,4,16,WHITE);	//��
		LCD_ShowChinese(56,0,0,32,WHITE);		//��
		LCD_ShowChinese(88,0,1,32,WHITE);		//��
		LCD_ShowChinese(120,0,2,32,WHITE);	//ϵ
		LCD_ShowChinese(152,0,3,32,WHITE);	//ͳ
		LCD_ShowChinese(32,48,4,16,WHITE);	//��
		Draw_Circle(152,232,8,YELLOW);
		LCD_ShowChinese(160,224,5,16,WHITE);  //��
		LCD_ShowChinese(176,224,6,16,WHITE);	//��
		LCD_ShowChinese(192,224,7,16,WHITE);		//ǿ
		LCD_ShowChinese(208,224,8,16,WHITE);	//��
		LCD_ShowChinese(224,224,9,16,WHITE);	//Ʒ
		LCD_ShowChinese(96,32,10,16,WHITE);		//��
		LCD_ShowChar(104,48,'%',0,WHITE);
		LCD_ShowChar(64,32,'.',0,WHITE);
		LCD_ShowChar(64,48,'.',0,WHITE);
		

//	
		LCD_ShowChinese(0,64,12,16,WHITE);		//��
		LCD_ShowChinese(16,64,13,16,WHITE);		//��
		LCD_ShowChinese(32,64,14,16,WHITE);		//��
		LCD_ShowChinese(48,64,15,16,WHITE);		//״
		LCD_ShowChinese(64,64,16,16,WHITE);	//̬
		LCD_ShowChinese(80,64,4,16,WHITE);		//:
		
		LCD_ShowChar(0,80,'L',16,WHITE);		//��
		LCD_ShowChar(8,80,'E',16,WHITE);		//��
		LCD_ShowChar(16,80,'D',16,WHITE);		//��
		LCD_ShowChinese(32,80,17,16,WHITE);		//״
		LCD_ShowChinese(48,80,15,16,WHITE);		//״
		LCD_ShowChinese(64,80,16,16,WHITE);	//̬
		LCD_ShowChinese(80,80,4,16,WHITE);		//:
		
		LCD_ShowString(0,96,"X",WHITE);
		LCD_ShowChinese(16,96,11,16,WHITE);//��
		LCD_ShowChinese(32,96,4,16,WHITE);		//:
		LCD_ShowString(0,112,"Y",WHITE);
		LCD_ShowChinese(16,112,11,16,WHITE);//��
		LCD_ShowChinese(32,112,4,16,WHITE);		//:
		LCD_ShowString(0,128,"Z",WHITE);
		LCD_ShowChinese(16,128,11,16,WHITE);//��
		LCD_ShowChinese(32,128,4,16,WHITE);		//:
		
		
}


