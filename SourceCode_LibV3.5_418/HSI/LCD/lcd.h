#ifndef __LCD_h__
#define __LCD_h__
#include <stdint.h>

#ifdef DEBUG
	#define DEBUG_X  50
	#define DEBUG_Y  15
	//extern unsigned int code debug_code[X][Y];
	extern unsigned int code debug_code[50][15];
	
	#define END 0xffff
#endif


#define ROW  320		    //显示的行、列数
#define COL  240

#define RED    0xF800		 //定义颜色常量 
#define GREEN  0x07E0
#define BLUE   0x001F
#define WHITE  0xFFFF
#define BLACK  0x0000
#define GRAY   0xEF5D	     //0x2410
#define GRAY75 0x39E7 
#define GRAY50 0x7BEF	
#define GRAY25 0xADB5	

#define X_min 0x0043		//TP测试范围常量定襾E
#define X_max 0x07AE
#define Y_min 0x00A1
#define Y_max 0x0759

#define CHAR_FONT_W8_H16
#define READ_REG_LCD
#define STRING_FUNCTION

#ifdef  CHAR_FONT_W8_H16	
    #define  FONT_W  8
    #define  FONT_H  16
#endif
#ifdef  CHAR_FONT_W16_H21
    #define  FONT_W  16
    #define  FONT_H  21
#endif

#define PIC_WIDTH    160	 //预备向LCD显示区域帖箱的图片的大小
#define PIC_HEIGHT   160

extern const unsigned int pic_eval[];
extern const unsigned char ascii[]; 

//---------------------------------------------------------------------
void WriteComm(unsigned int i);
void WriteData(unsigned int i);
void WriteDispData(unsigned char DataH,unsigned char DataL);
void LCD_Init(void);
void LCD_EnterSleep(void);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void DispBand(void);
void DispColor(unsigned int color);
void DispFrame(void);
void DispPic(const unsigned int *picture);
void DispPicFromSD(unsigned char PicNum);

void DispScaleHor1(void);
void DispScaleVer(void);
void DispScaleVer_Red(void);
void DispScaleVer_Green(void);
void DispScaleVer_Blue(void);
void DispScaleVer_Gray(void);
void DispGrayHor16(void);
void DispGrayHor32(void);
void DispScaleHor2(void);
void DispSnow(void);
void DispBlock(void);

void WriteOneDot(unsigned int color);
unsigned char ToOrd(unsigned char ch); 
void  DispOneChar(unsigned char ord,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);	 // ord:0~95
void DispStr(unsigned char *str,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);
void DispInt(unsigned int i,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);

unsigned int ReadData(void);
void DispRegValue(unsigned int RegIndex,unsigned char ParNum);

void Debug(void);

void PutPixel(unsigned int x,unsigned int y,unsigned int color);
void DrawLine(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend,unsigned int color);
void DrawGird(unsigned int color);

void Disp_GRAY_565(void);
void Disp_GRAY_666(void);
void Disp_GRAY_888(void);

void  DispColor_SPI_2_lane(unsigned int color);

/************************* Task API *************************/
extern void LCD_TaskHandler(void *pvParameters);
extern void LCD_TaskTrigger(void);
extern void Delay(uint16_t x_ms);
#endif
