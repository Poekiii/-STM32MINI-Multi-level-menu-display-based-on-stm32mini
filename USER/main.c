#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "string.h"
//ALIENTEK Mini STM32开发板范例代码11
//TFTLCD显示实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
typedef unsigned char uchar;
typedef unsigned int uint;

uint Pick_TAB[20];
uint Pick_count;  //选的菜的总量
uint SUM_Price;
u8 price[12];			//存放price的字符串	
uchar * Menu_TAB1[]={"木耳炒鸡一一一18元","小炒牛肉一一一28元","爆炒肥肠一一一22元","农家炒肉一一一12元",
            "清炒时蔬一一一8元","鱼头火锅一一一38元","羊肉火锅一一一48元","紫菜蛋汤一一一10元","西红柿蛋汤一一10元","榨菜粉丝汤一一15元",
            "王老吉一一一一4元","啤酒一一一一一6元","一一一一一一一一一"};
char * Menu_TAB2[]={"木耳炒鸡","小炒牛肉","爆炒肥肠","农家炒肉",
            "清炒时蔬","鱼头火锅","羊肉火锅","紫菜蛋汤","西红柿蛋汤","榨菜粉丝汤",
            "王老吉","啤酒"," "};
int Price_TAB[]={18,28,22,12,8,38,48,10,10,15,4,6,0,0};

/*菜单结构定义*/
struct MenuItem
{
	short MenuCount;//结构体数组的元素个数
	u8 *DisplayString;//当前LCD显示的信息
	int MenuClass;//结构体第几层的第几个项目
	void(*Subs)();//执行的函数的指针. 
	struct MenuItem *ChildrenMenus;//指向子节点的指针
	struct MenuItem *ParentMenus;//指向父节点的指针 
};

//函数声明 
void Pick_up(struct MenuItem *MenuPoint);
void Delete_up(struct MenuItem *MenuPoint);
void Call_waiter(void);
void Change_menu(struct MenuItem *MenuPoint);
void display(struct MenuItem * MenuPoint,short selectItem);
void Send_menu(void);

//结构体声明 
struct MenuItem MainMenu[6] = {
    { 6,"风味炒菜一一一一",1,NULL,NULL,NULL },
		{ 6,"口味火锅一一一一",2,NULL,NULL,NULL },
    { 6,"清淡汤类一一一一",3,NULL,NULL,NULL },
    { 6,"饮料一一一一一一",4,NULL,NULL,NULL },
    { 6,"我点的菜一一一一",5,NULL,NULL,NULL },
		{ 6,"呼叫服务员一一一",6,(*Call_waiter),NULL,NULL }
};

struct MenuItem Setmenu1[5]={
		{ 5,"木耳炒鸡一一一18元",11,(*Pick_up),NULL,MainMenu },
		{ 5,"小炒牛肉一一一28元",12,(*Pick_up),NULL,MainMenu },
		{ 5,"爆炒肥肠一一一22元",13,(*Pick_up),NULL,MainMenu },
		{ 5,"农家炒肉一一一12元",14,(*Pick_up),NULL,MainMenu },
		{ 5,"清炒时蔬一一一8元",15,(*Pick_up),NULL,MainMenu }
};

struct MenuItem Setmenu2[2] = {
    { 2,"鱼头火锅一一一38元",21,(*Pick_up),NULL,MainMenu },
    { 2,"羊肉火锅一一一48元",22,(*Pick_up),NULL,MainMenu } 
};
struct MenuItem Setmenu3[3]={
		{ 3,"紫菜蛋汤一一一10元",31,(*Pick_up),NULL,MainMenu },
		{ 3,"西红柿蛋汤一一10元",32,(*Pick_up),MainMenu },
		{ 3,"榨菜粉丝汤一一15元",33,(*Pick_up),NULL,MainMenu }
};
struct MenuItem Setmenu4[2]={
		{ 2,"王老吉一一一一4元",41,(*Pick_up),NULL,MainMenu },
		{ 2,"啤酒一一一一一6元",42,(*Pick_up),NULL,MainMenu }
};

struct MenuItem Showmenu[8]={
		{ 8," ",51,(*Delete_up),NULL,MainMenu },
		{ 8," ",52,(*Delete_up),NULL,MainMenu },
		{ 8," ",53,(*Delete_up),NULL,MainMenu },
		{ 8," ",54,(*Delete_up),NULL,MainMenu },
		{ 8," ",55,(*Delete_up),NULL,MainMenu },
		{ 8," ",56,(*Delete_up),NULL,MainMenu },
		{ 8," ",57,(*Delete_up),NULL,MainMenu },
		{ 8,"确认订单",58,(*Send_menu),NULL,MainMenu }
};

//struct MenuItem Showmenu[8]={
//		{ 8," ",51,NULL,NULL,MainMenu },
//		{ 8," ",52,NULL,NULL,MainMenu },
//		{ 8," ",53,NULL,NULL,MainMenu },
//		{ 8," ",54,NULL,NULL,MainMenu },
//		{ 8," ",55,NULL,NULL,MainMenu },
//		{ 8," ",56,NULL,NULL,MainMenu },
//		{ 8," ",57,NULL,NULL,MainMenu },
//		{ 8,"确认订单",58,(*Send_menu),NULL,MainMenu }
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
			for ( i = 0; i < (selectItem-1); i++)   //整个页面
			{
					showhz16str(x,y,MenuPoint[i].DisplayString,BLACK,WHITE);
					y+=30;
			}
			
			POINT_COLOR=BLACK;
			showhz16str(x,y,MenuPoint[selectItem-1].DisplayString,RED,WHITE); //被选中的那一行			
			y+=30;
			LCD_DrawRectangle(x,y,x+120,y+21);  //横线选  
			
			if(MenuPoint[selectItem-1].MenuClass>0&&MenuPoint[selectItem-1].MenuClass<=10)  //当前菜单名称
			{
			   showhz16str(86,30,"主菜单",WHITE,BLACK);
			}
			if(MenuPoint[selectItem-1].MenuClass>10&&MenuPoint[selectItem-1].MenuClass<=20)
			{
				 showhz16str(70,30,"风味炒菜",WHITE,BLACK);
				
			}
		  if(MenuPoint[selectItem-1].MenuClass>20&&MenuPoint[selectItem-1].MenuClass<=30)
			{
				 showhz16str(70,30,"口味火锅",WHITE,BLACK);
			}
			if(MenuPoint[selectItem-1].MenuClass>30&&MenuPoint[selectItem-1].MenuClass<=40)
			{
				 showhz16str(70,30,"清淡汤类",WHITE,BLACK);
			}
			if(MenuPoint[selectItem-1].MenuClass>40&&MenuPoint[selectItem-1].MenuClass<=50)
			{
				 showhz16str(102,30,"饮料",WHITE,BLACK);
			}
			if(MenuPoint[selectItem-1].MenuClass>50&&MenuPoint[selectItem-1].MenuClass<=60)
			{
				 showhz16str(70,30,"我点的菜",WHITE,BLACK);
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
			for ( i = 0; i < Pick_count; i++)   //整个页面
			{
					Showmenu[i].DisplayString=Menu_TAB1[Pick_TAB[i]];
			}				
}

void Call_waiter(void)
{
  LED1=0;
	delay_ms(100);
	LED1=1;
	printf("\r\n01号桌呼叫服务员！\r\n");
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
	
	printf("\r\n01号桌点的菜:\r\n");
	UART_Send_Str("\n");
	for (i=0;i<Pick_count ;i++)
	{
		if(Pick_TAB[i]<12)
		{
		 UART_Put_Inf(Menu_TAB2[Pick_TAB[i]],Price_TAB[Pick_TAB[i]]);             //  字符串
		 UART_Send_Str("元");
		 UART_Send_Str("\n");
		}
//		else
//		{
//			i++;
//		}
	}
  UART_Put_Inf("合计：",SUM_Price);             //  字符串
	UART_Send_Str("元");
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
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2 
		POINT_COLOR=BLACK;//
		MainMenu[0].ChildrenMenus = Setmenu1;
		MainMenu[1].ChildrenMenus = Setmenu2;
		MainMenu[2].ChildrenMenus = Setmenu3;
		MainMenu[3].ChildrenMenus = Setmenu4;
	  MainMenu[4].ChildrenMenus = Showmenu;
		selectItem =1;
	  Pick_count=0;
	  SUM_Price=0;
	  LCD_Fill(0,0,260,90,BLACK);  //横线选
	  LCD_Fill(0,270,260,320,BLACK);  //横线选
		LCD_ShowString(50,110,200,24,24,"Welcomed TO");
		LCD_ShowString(40,150,200,16,16,"**HOME Restaurant**");
		LCD_ShowString(70,190,200,16,16,"GLUT@Poeki");     					 
		LCD_ShowString(70,230,200,12,12,"2019/12/26");	  
	  delay_ms(9000);
		LCD_Clear(WHITE);
		display(MenuPoint, selectItem);
	  while(1)
    {
			  sprintf((char*)price,"SUM:%d",SUM_Price);//将SUM_Price打印到price数组							
			  LCD_ShowString(10,30,200,16,16,price);		//显示LCD ID
//				showhz16str(30,90,Menu_TAB[0],BLACK,WHITE); //被选中的那一行
        key=KEY_Scan(0);		//得到键值
        delay_ms(10);
		switch(key)
    {
			
//        case KEY0_PRES:{//上
//					    LCD_Clear(WHITE);
//							if (selectItem == 1) selectItem = MenuPoint->MenuCount;
//							else selectItem--;
//					display(MenuPoint, selectItem);
//          key=0;
//        };break; 
			
				 case KEY0_PRES:{//下
				 LCD_Clear(WHITE);
                if (selectItem == MenuPoint->MenuCount) selectItem = 1;
                else selectItem++;
				  display(MenuPoint, selectItem);
          key=0;
        };break; 
				
				 case WKUP_PRES:{//确认
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
				 
				case KEY1_PRES:{//返回
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
//	u8 lcd_id[12];			//存放LCD ID字符串	
//	delay_init();	    	 //延时函数初始化	  
//	uart_init(9600);	 	//串口初始化为9600
//	LED_Init();		  		//初始化与LED连接的硬件接口
// 	LCD_Init();
//	POINT_COLOR=BLACK; 
//	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。				 	
//  	while(1) 
//	{		 
//		LCD_Clear(WHITE);
//		POINT_COLOR=BLACK;	  
//		LCD_ShowString(30,40,200,24,24,"1.菜单");	
//		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
//		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
// 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
//		LCD_ShowString(30,130,200,12,12,"2014/3/7");	      					 
//		delay_ms(1000);	
//	} 
//}


