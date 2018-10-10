#ifndef __MAIN_h__
#define __MAIN_h__

typedef  unsigned char   uint8;
typedef  signed char     int8;
typedef  unsigned int    uint16;
typedef  signed int	     int16;   
typedef  unsigned long	 uint32; 
typedef  signed long	 int32; 

extern xdata unsigned char buffer[512];
//------------------------------------------------------
#define ROW  320		    //ÏÔÊ¾µÄĞĞ¡¢ÁĞÊı
#define COL  240

//#define SD_PIC_TEST_MODE   
	//#define QQVGA1          // QQVGA,128*128;
	//#define QQVGA2          // QQVGA,128*160;
	//#define QCIF           // QCIF,176*220;
	//#define QVGA              // QVGA,240*320;
	//#define WQVGA             // WQQVGA,240*400;
	//#define HVGA1            // HVGA1,272*480;
	//#define HVGA2           // HVGA2,320*480;	 
	//#define WVGA           // WVGA,480*800;

#define SD_PIC_DEMO_MODE 	//ÏÔÊ¾SD¿¨ÄÚµÄÍ¼Æ¬ÒÔÓÃÓÚÆÀ¹ÀºÍÕ¹Ê¾
//------------------------------------------------------
//#define TP_TEST		    //²âÊÔ¹¦ÄÜµÄÑ¡Ôñ£¬Ñ¡Ôñ³£Á¿±»¶¨Òå£¬ÏàÓ¦²âÊÔ¹¦ÄÜ¿ªÆô£» TP_TEST:TP»®Ïß²âÊÔ¹¦ÄÜ
//#define READ_REG		    //¶ÁID CODE¹¦ÄÜ
//#define SLEEP_TEST	    //½øÈE¢ÍË³ösleep¹¦ÄÜ
//#define DEBUG			    //µ÷ÊÔ¹¦ÄÜ£¬°ÑÒªµ÷ÊÔµÄÖ¸ÁûÖÍ²ÎÊıĞ´½øÊı×éÄÚ£¬Ã¿°´¼E´ĞĞÒ»×éÖ¸ÁûÖÍ²ÎÊı

//#define EVAL_PIC		    //Ê¹ÓÃMCUÆ¬ÉÏ64k Flash()ÄÚµÄÍ¼Æ¬£¬160*160ÌûÏäÏÔÊ¾ÇøÓE
//#define EXTEND_TEST	    //À©Õ¹²âÊÔ¹¦ÄÜ£¬Èç£¬»Ò¶È£¬É«²Ê½¥±äµÈ»­ÃE
//------------------------------------------------------
#define CHAR_FONT_W8_H16	//Ñ¡ÔñÏÔÊ¾×Ö·ûµÄ×ÖÌå£¬CHAR_FONT_W8_H16£º8*16£»CHAR_FONT_W16_H21£º16*21
//#define CHAR_FONT_W16_H21

#define CONN_USB		    //ÔÚ°´¼EÈ´ıÊ±Á¬½ÓUSB½øĞĞÊı¾İÍ¨ĞÅ¡¢½»»»£¨ÒÔ´ËÊµÏÖUSBµ÷ÊÔ¹¦ÄÜ£©	
#define USE_SD_INIT			//SD¿¨³õÊ¼»¯
//#define EXTEND_TEST

//#define AUTO_TEST			//Èô¶¨ÒåAUTO-TEST£¬µÈ´ı°´¼E±²»±Ø°´°´¼E¬ÑÓÊ±Ò»¶Îºó×Ô¶¯½øĞĞÏÂÒ»²½µÄ²âÊÔ	
//------------------------------------------------------
#define DBH  P2				//½Ó¿Ú¶¨Òå²¿·Ö
#define DBL  P4

//LCM | SPI4W

sbit	CS0 = P1^0;
sbit	RST = P1^3;
sbit	RS  = P3^4;
sbit	SCL = P1^1;
sbit	SDA = P1^2;

//TP 
sbit    TP_CS	  = P1^4;
sbit    TP_PENIRQ = P3^5;
sbit    TP_DOUT   = P3^3;
sbit    TP_DIN	  = P3^2;
sbit    TP_DCLK   = P1^5;
sbit    TP_BUSY	  = P3^4;

//KEY
sbit    KEY_STEP  = P3^6;	//¶¨Òå¡°ÏÂÒ»²½¡±°´¼E
//------------------------------------------------------
#ifdef  READ_REG	
	#define STRING_FUNCTION
    #define READ_FUNCTION
#endif

#ifdef  DEBUG	
	#define STRING_FUNCTION
#endif
#ifdef  CONN_USB	
	#define STRING_FUNCTION
    #define READ_FUNCTION
#endif
#ifdef  TP_TEST	
	#define DRAW_FUNCTION
	#define STRING_FUNCTION
#endif

#ifdef AUTO_TEST
	#define AUTO_NEXT
#else
	#define MANUA_NEXT
#endif	

//#define STRING_FUNCTION	  //¶¨Òå×Ö·û´®´¦ÀúÖÍÏÔÊ¾µÄº¯Êı
//#define DRAW_FUNCTION		  //¶¨ÒåTP²âÊÔµÄº¯Êı
//#define READ_FUNCTION		  //¶¨Òå¶ÁÈ¡IC²Ù×÷µÄº¯Êı
//------------------------------------------------------
#ifdef  CHAR_FONT_W8_H16	
	#define  FONT_W  8
	#define  FONT_H  16
#endif
#ifdef  CHAR_FONT_W16_H21
	#define  FONT_W  16
	#define  FONT_H  21
#endif
//------------------------------------------------------
#define PIC_WIDTH    160	 //Ô¤±¸ÏòLCDÏÔÊ¾ÇøÓòÌûÏäµÄÍ¼Æ¬µÄ´óĞ¡
#define PIC_HEIGHT   160

#define RED    0xF800		 //¶¨ÒåÑÕÉ«³£Á¿ 
#define GREEN  0x07E0
#define BLUE   0x001F
#define WHITE  0xFFFF
#define BLACK  0x0000
#define GRAY   0xEF5D	     //0x2410
#define GRAY75 0x39E7 
#define GRAY50 0x7BEF	
#define GRAY25 0xADB5	

#define X_min 0x0043		//TP²âÊÔ·¶Î§³£Á¿¶¨ÒE
#define X_max 0x07AE
#define Y_min 0x00A1
#define Y_max 0x0759
//------------------------------------------------------
void  Delay(unsigned int dly);
void  WaitKey(void);
void  ConnToUSB(void);
void  Suspend_Device(void);
void  MCU_Init(void); 
void  SD_Start(void);
void  Devices_Init(void);

#endif
