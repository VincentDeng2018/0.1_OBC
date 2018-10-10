/****************************************************************
 Copyright (C),2014-2016,SZZKJ Technology
 File name:		lcd.c
 Author:		WangFeng
 Version:		1.00
 Date:			2014-11-8
 Description:	
 
 Others:		
 
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 History
 ----------------------------------------------------------------
 Index:			1
 Date:			2014-11-8
 Author:		WangFeng
 Modification:	Create this file
****************************************************************/
/*Include files -----------------------------------------------*/
#include "lcd.h"
#include "spi.h"
#include "stm32f10x_gpio.h"


/*Defines -----------------------------------------------------*/


#define CD_LOW()		GPIOA->BRR = GPIO_Pin_7			
#define CD_HIGH()		GPIOA->BSRR = GPIO_Pin_7
#define CD_INIT()		{PORTAbits.RA7 = 1;TRISAbits.TRISA7=0;}
#define RES_LOW()		GPIOB->BRR = GPIO_Pin_5
#define RES_HIGH()		GPIOB->BSRR = GPIO_Pin_5
#define RES_INIT()		{PORTBbits.RB5 = 1;TRISBbits.TRISB5=0;}
#define _nop_()			SPI_Dly()
#define CLR_WDT()		IWDG_ReloadCounter()

/*Extern data & funtion----------------------------------------*/

/*Global data -------------------------------------------------*/
BYTE LcdPartIndex = LCD_PART_LEFT;
BYTE LcdData[LCD_DATA_Y_MAX][LCD_DATA_X_MAX>>1];
BYTE Curve;

BYTE ContrastLevel= 0x02;
/*datas & funcitons define area -------------------------------*/


//-----------------------------------
// delayms routine
//-----------------------------------
void delayms(U16 m)        // 12MHz Xtal, close to ms value
{
    WORD j;
    WORD i;
    
    for(i=0; i<m; i++)
    {
		if (!(i%2000))
		{
			CLR_WDT();
		}
        for(j=0; j<10000; j++)
            _nop_();
    }
}

//-----------------------------------
// 串行通讯方式驱动函数
//-----------------------------------
  void SdCmd(U8 Command)
        {
          SPI_SCK_HIGH();                   // 初始化SCLK
          _nop_();
           CD_LOW();                    // 选择指令通道
           SPI_CHN_LCD_CE_LOW();                   // 选通模块
           _nop_();
          if((Command&0x80)==0x80){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
          _nop_();
		  if((Command&0x40)==0x40){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
          _nop_();
		  if((Command&0x20)==0x20){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH(); 
          _nop_();
		  if((Command&0x10)==0x10){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();
          _nop_();
		  if((Command&0x08)==0x08){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();
          _nop_();
		  if((Command&0x04)==0x04){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();   
          _nop_();
		  if((Command&0x02)==0x02){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
          _nop_();
		  if((Command&0x01)==0x01){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
			_nop_();
		  SPI_CHN_LCD_CE_HIGH();                      // 封锁模块
       }
//-----显示数据写入函数----------------------------
void SdData(U8 DData)
      {
          SPI_SCK_HIGH();                   // 初始化SCLK
          _nop_();
          CD_HIGH();                    // 选择数据通道
          SPI_CHN_LCD_CE_LOW();                    // 选通模块
          _nop_();
          if((DData&0x80)==0x80){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
          _nop_();if((DData&0x40)==0x40){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
          _nop_();if((DData&0x20)==0x20){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
          _nop_();if((DData&0x10)==0x10){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
          _nop_();if((DData&0x08)==0x08){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
          _nop_();if((DData&0x04)==0x04){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();   
          _nop_();if((DData&0x02)==0x02){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
          _nop_();if((DData&0x01)==0x01){SPI_SDO_HIGH();}else{SPI_SDO_LOW();}SPI_SCK_LOW();_nop_();SPI_SCK_HIGH();  
		  _nop_();
		  SPI_CHN_LCD_CE_HIGH();                      // 封锁模块
      }

/*Functions ---------------------------------------------------*/
/****************************************************************
* Function:		
* Description:	
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void LcdIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//CD_INIT();
	//RES_INIT();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	 /*!< Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);

	return;
}

/****************************************************************
* Function:		
* Description:	
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void LcdInit(void)
{
	LcdIOInit();
	RES_LOW(); delayms(200);   //硬件复位
    RES_HIGH(); delayms(200);  //复位后延迟800ms以上时间
   // ContrastLevel=0x05;    
   //ContrastLevel ++;
    SdCmd(0x25);                                // 设置温度补偿系数-0.05%/C
    SdCmd(0x2b);                                // 内部DC-DC
	SdCmd(0xc4);                                // LCD映像MY=1，MX=0，LC0=0  c4
	SdCmd(0xa0);                                // 设置行扫描频率  fr=76.34hz a1
	SdCmd(0xd1);                                // 彩色数据格式R-G-B  d1
	SdCmd(0xd5);								// 设置数据位为12位RRRR-GGGG-BBBB
	SdCmd(0xc8); SdCmd(0x00);                   // 设置M信号为19行翻转 11
	SdCmd(0xeb);                                // 设置偏压比1/12
	SdCmd(0xa6);                                // 正性显示
	SdCmd(0xa4);                                // 正常显示
	SdCmd(0x81); SdCmd(0x25/*ContrastLevel*/);          //设置对比度bf
	SdCmd(0xd8);                                // 设置扫描模式d8

    SdCmd(0xf1);SdCmd(147);     //结束行CEN,159
    SdCmd(0xf2);SdCmd(20);     //起始DST
    SdCmd(0xf3);SdCmd(147);     //结束DEN
	SdCmd(0x85); 
	LcdClr();
	SdCmd(0xad);   delayms(100);                // 开显示ad
	return;
}

/****************************************************************
* Function:		
* Description:	
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void LcdClr(void)
{
	UINT p; 
   U8 i,j,k,m,Ddata,x,y;
   x=24;                                  // 起始列地址偏移量修正
   y=0;                                  // 起始行地址偏移量修正
   SdCmd(0xf4); SdCmd(x);                   // 设置操作窗口左边界
   SdCmd(0xf5); SdCmd(y+20);                        // 设置操作窗口上边界 
   SdCmd(0xf6); SdCmd(x+240/3-1);             // 设置操作窗口右边界
   SdCmd(0xf7); SdCmd(y+20+128-1);                   // 设置操作窗口下边界
   SdCmd(0xf8);		                    		 // 设置窗口操作使能
   SdCmd(x&0x0f);SdCmd(0x10|(x>>4));             // 设置起始列地址

   SdCmd(0x60+(y&0x0f)); SdCmd(0x70+(y>>4));     // 设置起始行地址
   //p=0;                                          // 数组指针初始化
   for(i=0;i<128;i++)                           // 循环行数据量
     {
	 for ( j=0;j<240/8;j++)                    // 循环字节数/行
	     {
          //k=pData[p++];                            // 取图形数据
          for (m=0;m<4;m++)                      // 1字节转换成4个字节数据写入
              {
    	       /*switch (k&0xc0)                   // 根据数据位值设置显示色数据
                   {
		            case 0x00 : Ddata=0x00;break;
		            case 0x40 : Ddata=0x0f;break;
		            case 0x80 : Ddata=0xf0;break;
		            case 0xc0 : Ddata=0xff;break;
		           }*/
              SdData(0x00);                      // 写入数据
		      //k=k<<2;
		      }
       }

  }
	return;
}

void LcdSet(void)
{
	UINT p; 
   U8 i,j,k,m,Ddata,x,y;
   x=24;                                  // 起始列地址偏移量修正
   y=0;                                  // 起始行地址偏移量修正
   SdCmd(0xf4); SdCmd(x);                   // 设置操作窗口左边界
   SdCmd(0xf5); SdCmd(y+20);                        // 设置操作窗口上边界 
   SdCmd(0xf6); SdCmd(x+240/3-1);             // 设置操作窗口右边界
   SdCmd(0xf7); SdCmd(y+20+128-1);                   // 设置操作窗口下边界
   SdCmd(0xf8);		                    		 // 设置窗口操作使能
   SdCmd(x&0x0f);SdCmd(0x10|(x>>4));             // 设置起始列地址

   SdCmd(0x60+(y&0x0f)); SdCmd(0x70+(y>>4));     // 设置起始行地址
   //p=0;                                          // 数组指针初始化
   for(i=0;i<128;i++)                           // 循环行数据量
     {
	 for ( j=0;j<240/8;j++)                    // 循环字节数/行
	     {
          //k=pData[p++];                            // 取图形数据
          for (m=0;m<4;m++)                      // 1字节转换成4个字节数据写入
              {
    	       /*switch (k&0xc0)                   // 根据数据位值设置显示色数据
                   {
		            case 0x00 : Ddata=0x00;break;
		            case 0x40 : Ddata=0x0f;break;
		            case 0x80 : Ddata=0xf0;break;
		            case 0xc0 : Ddata=0xff;break;
		           }*/
              SdData(0xff);                      // 写入数据
		      //k=k<<2;
		      }
       }

  }
	return;
}



/****************************************************************
* Function:		
* Description:	
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void LcdRefresh(WORD x, WORD y, WORD cx, WORD cy, BYTE *pData)
{
	// 坐标X为像素列块0-79列块，3像素点数据/列块
// 坐标Y为像素点行数0-159
// 图形宽度width为水平方向点列数，要求该值为6的倍数
// 图形高度high为垂直方向点行数，取值为0-159
// 图形数组bmp[]为所要写入的图形数据，以1bpp（8dots/byte)、水平排列格式表示


   UINT p; 
   U8 i,j,k,m,Ddata;



   x = x/3;
   x=x+24;                                  // 起始列地址偏移量修正
   y=y;                                  // 起始行地址偏移量修正
   SdCmd(0xf4); SdCmd(x);                   // 设置操作窗口左边界
   SdCmd(0xf5); SdCmd(y + 20);                        // 设置操作窗口上边界 
   SdCmd(0xf6); SdCmd(x+cx/3-1);             // 设置操作窗口右边界
   SdCmd(0xf7); SdCmd(y+ 20 + cy-1);                   // 设置操作窗口下边界
   SdCmd(0xf8);		                    		 // 设置窗口操作使能
   SdCmd(x&0x0f);SdCmd(0x10|(x>>4));             // 设置起始列地址

   SdCmd(0x60+(y&0x0f)); SdCmd(0x70+(y>>4));     // 设置起始行地址


   
   p=cx/8;  p*=cy;  p --;                                      // 数组指针初始化
   for(i=0;i<cy;i++)                           // 循环行数据量
     {
	 	//p=cx*(cy - i) - 1;
		//p = cx/8;
		//p*=(cy-i);
		//p--;
	 for ( j=0;j<cx/8;j++)                    // 循环字节数/行
	     {
          k=pData[p--];                            // 取图形数据
          for (m=0;m<4;m++)                      // 1字节转换成4个字节数据写入
              {
    	       switch (k&0x03)                   // 根据数据位值设置显示色数据
                   {
		            case 0x00 : Ddata=0x00;break;
		            case 0x01 : Ddata=0xf0;break;
		            case 0x02 : Ddata=0x0f;break;
		            case 0x03 : Ddata=0xff;break;
		           }
              SdData(Ddata);                      // 写入数据
		      k=k>>2;
		      }
       }

  }
   
}

/* end of file */


