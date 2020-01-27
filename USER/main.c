#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "string.h"
//ALIENTEK Mini STM32�����巶������11
//TFTLCD��ʾʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
typedef unsigned char uchar;
typedef unsigned int uint;

uint Pick_TAB[20];
uint Pick_count;  //ѡ�Ĳ˵�����
uint SUM_Price;
u8 price[12];			//���price���ַ���	
uchar * Menu_TAB1[]={"ľ������һһһ18Ԫ","С��ţ��һһһ28Ԫ","�����ʳ�һһһ22Ԫ","ũ�ҳ���һһһ12Ԫ",
            "�峴ʱ��һһһ8Ԫ","��ͷ���һһһ38Ԫ","������һһһ48Ԫ","�ϲ˵���һһһ10Ԫ","����������һһ10Ԫ","ե�˷�˿��һһ15Ԫ",
            "���ϼ�һһһһ4Ԫ","ơ��һһһһһ6Ԫ","һһһһһһһһһ"};
char * Menu_TAB2[]={"ľ������","С��ţ��","�����ʳ�","ũ�ҳ���",
            "�峴ʱ��","��ͷ���","������","�ϲ˵���","����������","ե�˷�˿��",
            "���ϼ�","ơ��"," "};
int Price_TAB[]={18,28,22,12,8,38,48,10,10,15,4,6,0,0};

/*�˵��ṹ����*/
struct MenuItem
{
	short MenuCount;//�ṹ�������Ԫ�ظ���
	u8 *DisplayString;//��ǰLCD��ʾ����Ϣ
	int MenuClass;//�ṹ��ڼ���ĵڼ�����Ŀ
	void(*Subs)();//ִ�еĺ�����ָ��. 
	struct MenuItem *ChildrenMenus;//ָ���ӽڵ��ָ��
	struct MenuItem *ParentMenus;//ָ�򸸽ڵ��ָ�� 
};

//�������� 
void Pick_up(struct MenuItem *MenuPoint);
void Delete_up(struct MenuItem *MenuPoint);
void Call_waiter(void);
void Change_menu(struct MenuItem *MenuPoint);
void display(struct MenuItem * MenuPoint,short selectItem);
void Send_menu(void);

//�ṹ������ 
struct MenuItem MainMenu[6] = {
    { 6,"��ζ����һһһһ",1,NULL,NULL,NULL },
		{ 6,"��ζ���һһһһ",2,NULL,NULL,NULL },
    { 6,"�嵭����һһһһ",3,NULL,NULL,NULL },
    { 6,"����һһһһһһ",4,NULL,NULL,NULL },
    { 6,"�ҵ�Ĳ�һһһһ",5,NULL,NULL,NULL },
		{ 6,"���з���Աһһһ",6,(*Call_waiter),NULL,NULL }
};

struct MenuItem Setmenu1[5]={
		{ 5,"ľ������һһһ18Ԫ",11,(*Pick_up),NULL,MainMenu },
		{ 5,"С��ţ��һһһ28Ԫ",12,(*Pick_up),NULL,MainMenu },
		{ 5,"�����ʳ�һһһ22Ԫ",13,(*Pick_up),NULL,MainMenu },
		{ 5,"ũ�ҳ���һһһ12Ԫ",14,(*Pick_up),NULL,MainMenu },
		{ 5,"�峴ʱ��һһһ8Ԫ",15,(*Pick_up),NULL,MainMenu }
};

struct MenuItem Setmenu2[2] = {
    { 2,"��ͷ���һһһ38Ԫ",21,(*Pick_up),NULL,MainMenu },
    { 2,"������һһһ48Ԫ",22,(*Pick_up),NULL,MainMenu } 
};
struct MenuItem Setmenu3[3]={
		{ 3,"�ϲ˵���һһһ10Ԫ",31,(*Pick_up),NULL,MainMenu },
		{ 3,"����������һһ10Ԫ",32,(*Pick_up),MainMenu },
		{ 3,"ե�˷�˿��һһ15Ԫ",33,(*Pick_up),NULL,MainMenu }
};
struct MenuItem Setmenu4[2]={
		{ 2,"���ϼ�һһһһ4Ԫ",41,(*Pick_up),NULL,MainMenu },
		{ 2,"ơ��һһһһһ6Ԫ",42,(*Pick_up),NULL,MainMenu }
};

struct MenuItem Showmenu[8]={
		{ 8," ",51,(*Delete_up),NULL,MainMenu },
		{ 8," ",52,(*Delete_up),NULL,MainMenu },
		{ 8," ",53,(*Delete_up),NULL,MainMenu },
		{ 8," ",54,(*Delete_up),NULL,MainMenu },
		{ 8," ",55,(*Delete_up),NULL,MainMenu },
		{ 8," ",56,(*Delete_up),NULL,MainMenu },
		{ 8," ",57,(*Delete_up),NULL,MainMenu },
		{ 8,"ȷ�϶���",58,(*Send_menu),NULL,MainMenu }
};

//struct MenuItem Showmenu[8]={
//		{ 8," ",51,NULL,NULL,MainMenu },
//		{ 8," ",52,NULL,NULL,MainMenu },
//		{ 8," ",53,NULL,NULL,MainMenu },
//		{ 8," ",54,NULL,NULL,MainMenu },
//		{ 8," ",55,NULL,NULL,MainMenu },
//		{ 8," ",56,NULL,NULL,MainMenu },
//		{ 8," ",57,NULL,NULL,MainMenu },
//		{ 8,"ȷ�϶���",58,(*Send_menu),NULL,MainMenu }
//};

struct MenuItem* MenuPoint = MainMenu;
short selectItem;

void Pick_up(struct MenuItem *MenuPoint)
{
	LED1=0;
	delay_ms(100);
	LED1=1;
	
	if(MenuPoint[selectItem-1].MenuClass==11)	{SUM_Price=SUM_Price+18;		Pick_TAB[Pick_count]=0;}
	if(MenuPoint[selectItem-1].MenuClass==12)	{SUM_Price=SUM_Price+28;		Pick_TAB[Pick_count]=1;}
	if(MenuPoint[selectItem-1].MenuClass==13)	{SUM_Price=SUM_Price+22;		Pick_TAB[Pick_count]=2;}
	if(MenuPoint[selectItem-1].MenuClass==14)	{SUM_Price=SUM_Price+12;		Pick_TAB[Pick_count]=3;}
	if(MenuPoint[selectItem-1].MenuClass==15)	{SUM_Price=SUM_Price+8;			Pick_TAB[Pick_count]=4;}
	if(MenuPoint[selectItem-1].MenuClass==21)	{SUM_Price=SUM_Price+38;		Pick_TAB[Pick_count]=5;}
	if(MenuPoint[selectItem-1].MenuClass==22)	{SUM_Price=SUM_Price+48;		Pick_TAB[Pick_count]=6;}
	if(MenuPoint[selectItem-1].MenuClass==31)	{SUM_Price=SUM_Price+10;		Pick_TAB[Pick_count]=7;}
	if(MenuPoint[selectItem-1].MenuClass==32)	{SUM_Price=SUM_Price+10;		Pick_TAB[Pick_count]=8;}
	if(MenuPoint[selectItem-1].MenuClass==33)	{SUM_Price=SUM_Price+15;		Pick_TAB[Pick_count]=9;}
	if(MenuPoint[selectItem-1].MenuClass==41)	{SUM_Price=SUM_Price+4;			Pick_TAB[Pick_count]=10;}
	if(MenuPoint[selectItem-1].MenuClass==42)	{SUM_Price=SUM_Price+6;			Pick_TAB[Pick_count]=11;}
	
	Pick_count++;
}

void display(struct MenuItem * MenuPoint,short selectItem)
{
			int i;
			int j;
			u16 x=30;
			u16 y=60;
			for ( i = 0; i < (selectItem-1); i++)   //����ҳ��
			{
					showhz16str(x,y,MenuPoint[i].DisplayString,BLACK,WHITE);
					y+=30;
			}
			
			POINT_COLOR=BLACK;
			showhz16str(x,y,MenuPoint[selectItem-1].DisplayString,RED,WHITE); //��ѡ�е���һ��			
			y+=30;
			LCD_DrawRectangle(x,y,x+120,y+21);  //����ѡ  
			
			if(MenuPoint[selectItem-1].MenuClass>0&&MenuPoint[selectItem-1].MenuClass<=10)  //��ǰ�˵�����
			{
			   showhz16str(86,30,"���˵�",WHITE,BLACK);
			}
			if(MenuPoint[selectItem-1].MenuClass>10&&MenuPoint[selectItem-1].MenuClass<=20)
			{
				 showhz16str(70,30,"��ζ����",WHITE,BLACK);
				
			}
		  if(MenuPoint[selectItem-1].MenuClass>20&&MenuPoint[selectItem-1].MenuClass<=30)
			{
				 showhz16str(70,30,"��ζ���",WHITE,BLACK);
			}
			if(MenuPoint[selectItem-1].MenuClass>30&&MenuPoint[selectItem-1].MenuClass<=40)
			{
				 showhz16str(70,30,"�嵭����",WHITE,BLACK);
			}
			if(MenuPoint[selectItem-1].MenuClass>40&&MenuPoint[selectItem-1].MenuClass<=50)
			{
				 showhz16str(102,30,"����",WHITE,BLACK);
			}
			if(MenuPoint[selectItem-1].MenuClass>50&&MenuPoint[selectItem-1].MenuClass<=60)
			{
				 showhz16str(70,30,"�ҵ�Ĳ�",WHITE,BLACK);
			}						

			
			for ( j= selectItem; j < MenuPoint->MenuCount; j++)
			{
								showhz16str(x,y,MenuPoint[j].DisplayString,BLACK,WHITE);
								y+=30;
			}
}

void Change_menu(struct MenuItem *MenuPoint)
{
			int i;
			for ( i = 0; i < Pick_count; i++)   //����ҳ��
			{
					Showmenu[i].DisplayString=Menu_TAB1[Pick_TAB[i]];
			}				
}

void Call_waiter(void)
{
  LED1=0;
	delay_ms(100);
	LED1=1;
	printf("\r\n01�������з���Ա��\r\n");
	delay_ms(100);
}



void Send_menu(void)
{
	int i;
	LED1=0;
	delay_ms(100);
	LED1=1;
	delay_ms(100);
	LED1=0;
  delay_ms(100);
	LED1=1;
	
	printf("\r\n01������Ĳ�:\r\n");
	UART_Send_Str("\n");
	for (i=0;i<Pick_count ;i++)
	{
		if(Pick_TAB[i]<12)
		{
		 UART_Put_Inf(Menu_TAB2[Pick_TAB[i]],Price_TAB[Pick_TAB[i]]);             //  �ַ���
		 UART_Send_Str("Ԫ");
		 UART_Send_Str("\n");
		}
//		else
//		{
//			i++;
//		}
	}
  UART_Put_Inf("�ϼƣ�",SUM_Price);             //  �ַ���
	UART_Send_Str("Ԫ");
  UART_Send_Enter();
}

void Delete_up(struct MenuItem *MenuPoint)
{
	LED0=0;
	delay_ms(100);
	LED0=1;
	if(Pick_TAB[selectItem-1]==0)	{SUM_Price=SUM_Price-18;		}
	if(Pick_TAB[selectItem-1]==1)	{SUM_Price=SUM_Price-28;		}
	if(Pick_TAB[selectItem-1]==2)	{SUM_Price=SUM_Price-22;		}
	if(Pick_TAB[selectItem-1]==3)	{SUM_Price=SUM_Price-12;		}
	if(Pick_TAB[selectItem-1]==4)	{SUM_Price=SUM_Price-8;			}
	if(Pick_TAB[selectItem-1]==5)	{SUM_Price=SUM_Price-38;		}
	if(Pick_TAB[selectItem-1]==6)	{SUM_Price=SUM_Price-48;		}
	if(Pick_TAB[selectItem-1]==7)	{SUM_Price=SUM_Price-10;		}
	if(Pick_TAB[selectItem-1]==8)	{SUM_Price=SUM_Price-10;		}
	if(Pick_TAB[selectItem-1]==9)	{SUM_Price=SUM_Price-15;		}
	if(Pick_TAB[selectItem-1]==10)	{SUM_Price=SUM_Price-4;			}
	if(Pick_TAB[selectItem-1]==11)	{SUM_Price=SUM_Price-6;			}
	
  Pick_TAB[selectItem-1]=12;
	
	Change_menu(MenuPoint);
//	Pick_count--;
}



int main(void)
{
		u8 key=0;
		delay_init();
		uart_init(9600); 
	  LED_Init();
		LCD_Init();
		KEY_Init();
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2 
		POINT_COLOR=BLACK;//
		MainMenu[0].ChildrenMenus = Setmenu1;
		MainMenu[1].ChildrenMenus = Setmenu2;
		MainMenu[2].ChildrenMenus = Setmenu3;
		MainMenu[3].ChildrenMenus = Setmenu4;
	  MainMenu[4].ChildrenMenus = Showmenu;
		selectItem =1;
	  Pick_count=0;
	  SUM_Price=0;
	  LCD_Fill(0,0,260,90,BLACK);  //����ѡ
	  LCD_Fill(0,270,260,320,BLACK);  //����ѡ
		LCD_ShowString(50,110,200,24,24,"Welcomed TO");
		LCD_ShowString(40,150,200,16,16,"**HOME Restaurant**");
		LCD_ShowString(70,190,200,16,16,"GLUT@Poeki");     					 
		LCD_ShowString(70,230,200,12,12,"2019/12/26");	  
	  delay_ms(9000);
		LCD_Clear(WHITE);
		display(MenuPoint, selectItem);
	  while(1)
    {
			  sprintf((char*)price,"SUM:%d",SUM_Price);//��SUM_Price��ӡ��price����							
			  LCD_ShowString(10,30,200,16,16,price);		//��ʾLCD ID
//				showhz16str(30,90,Menu_TAB[0],BLACK,WHITE); //��ѡ�е���һ��
        key=KEY_Scan(0);		//�õ���ֵ
        delay_ms(10);
		switch(key)
    {
			
//        case KEY0_PRES:{//��
//					    LCD_Clear(WHITE);
//							if (selectItem == 1) selectItem = MenuPoint->MenuCount;
//							else selectItem--;
//					display(MenuPoint, selectItem);
//          key=0;
//        };break; 
			
				 case KEY0_PRES:{//��
				 LCD_Clear(WHITE);
                if (selectItem == MenuPoint->MenuCount) selectItem = 1;
                else selectItem++;
				  display(MenuPoint, selectItem);
          key=0;
        };break; 
				
				 case WKUP_PRES:{//ȷ��
            if (MenuPoint[selectItem - 1].ChildrenMenus != NULL)
                {
                     LCD_Clear(WHITE);
									   MenuPoint = MenuPoint[selectItem - 1].ChildrenMenus;
                     selectItem = 1;									
										 display(MenuPoint, selectItem);
									   
								}
								else if (MenuPoint[selectItem - 1].Subs !=NULL )
							{
									(*MenuPoint[selectItem - 1].Subs)(MenuPoint);
									 Change_menu(MenuPoint);
							}	
          key=0;
        };break; 
				 
				case KEY1_PRES:{//����
						if (MenuPoint[selectItem - 1].ParentMenus != NULL)
                {
                     LCD_Clear(WHITE);
									   MenuPoint = MenuPoint[selectItem - 1].ParentMenus;
                     selectItem = 1;
										display(MenuPoint, selectItem);
								}
          key=0;
        };break; 
				
			}
		delay_ms(10);
		}
}		

// int main(void)
// { 
//	u8 lcd_id[12];			//���LCD ID�ַ���	
//	delay_init();	    	 //��ʱ������ʼ��	  
//	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
//	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
// 	LCD_Init();
//	POINT_COLOR=BLACK; 
//	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣				 	
//  	while(1) 
//	{		 
//		LCD_Clear(WHITE);
//		POINT_COLOR=BLACK;	  
//		LCD_ShowString(30,40,200,24,24,"1.�˵�");	
//		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
//		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
// 		LCD_ShowString(30,110,200,16,16,lcd_id);		//��ʾLCD ID	      					 
//		LCD_ShowString(30,130,200,12,12,"2014/3/7");	      					 
//		delay_ms(1000);	
//	} 
//}


