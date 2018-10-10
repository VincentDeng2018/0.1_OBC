/****************************************************************
 Copyright (C),2014-2016,SZZKJ Technology
 File name:		display.c
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
#include "db.h"
#include "ctrl.h"
#include "display.h"


/*Defines -----------------------------------------------------*/
#define S16 short
#define DWORD unsigned long
#define UINT unsigned int

#define DISPLAY_MENU_LVL0		0
#define DISPLAY_MENU_LVL1		1
#define DISPLAY_MENU_LVL2		2

#define DISPLAY_ML0_IDX_XXX		0


	
#define DISPLAY_T_CO_MARK_LEN	3
#define DISPLAY_U_CO_MARK_LEN	3
#define DISPLAY_I_CO_MARK_LEN	3
#define DISPLAY_U_I_CO_OFFSET	7
#define DISPLAY_T_CO_OFFSET		7

#define DISPLAY_AREA_WIDTH		(LCD_CX >> 1)
#define DISPLAY_AREA_HEIGHT		(LCD_CY - 16)
#define DISPLAY_U_CO_OFFSET		DISPLAY_U_I_CO_OFFSET
#define DISPLAY_I_CO_OFFSET		(DISPLAY_AREA_WIDTH - 1 - DISPLAY_U_I_CO_OFFSET)

#define DISPLAY_CURVE_WIDTH		(DISPLAY_AREA_WIDTH - 2*DISPLAY_U_I_CO_OFFSET)
#define DISPLAY_CURVE_HEIGHT	(DISPLAY_AREA_HEIGHT - DISPLAY_T_CO_OFFSET)

/*
:.0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
*/
typedef enum {
	SPACE = 0,
	SYM_MINUS,
	SYM_DASH,
	SYM_COLON,
	SYM_PERIOD,
	DIG_0,
	DIG_1,
	DIG_2,
	DIG_3,
	DIG_4,
	DIG_5,
	DIG_6,
	DIG_7,
	DIG_8,
	DIG_9,
	ALPH_A,
	ALPH_B,
	ALPH_C,
	ALPH_D,
	ALPH_E,
	ALPH_F,
	ALPH_G,
	ALPH_H,
	ALPH_I,
	ALPH_J,
	ALPH_K,
	ALPH_L,
	ALPH_M,
	ALPH_N,
	ALPH_O,
	ALPH_P,
	ALPH_Q,
	ALPH_R,
	ALPH_S,
	ALPH_T,
	ALPH_U,
	ALPH_V,
	ALPH_W,
	ALPH_X,
	ALPH_Y,
	ALPH_Z,
	ALPH_a,
	ALPH_b,
	ALPH_c,
	ALPH_d,
	ALPH_e,
	ALPH_f,
	ALPH_g,
	ALPH_h,
	ALPH_i,
	ALPH_j,
	ALPH_k,
	ALPH_l,
	ALPH_m,
	ALPH_n,
	ALPH_o,
	ALPH_p,
	ALPH_q,
	ALPH_r,
	ALPH_s,
	ALPH_t,
	ALPH_u,
	ALPH_v,
	ALPH_w,
	ALPH_x,
	ALPH_y,
	ALPH_z,
	FONT_DIG_ALPH_MAX
		
} FONT_DIG_ALPH_T;

typedef enum {
	S5x7_SPACE = 0,
	S5x7_SYM_PERIOD,
	S5x7_ALPH_A,
	S5x7_ALPH_V,
	S5x7_ALPH_M,
	S5x7_SYM_MINUS,
	S5x7_SYM_MORE,
	S5x7_SYM_LESS,
	S5x7_DIG_0,
	S5x7_DIG_1,
	S5x7_DIG_2,
	S5x7_DIG_3,
	S5x7_DIG_4,
	S5x7_DIG_5,
	S5x7_DIG_6,
	S5x7_DIG_7,
	S5x7_DIG_8,
	S5x7_DIG_9,
	S5x7_FONT_DIG_ALPH_MAX
		
} S5x7_FONT_DIG_ALPH_T;


/*
深圳宇杰达机型电压流时间容量曲线
*/
typedef enum {
	CHR_SHEN = 0,
	CHR_ZHEN,
	CHR_YU,
	CHR_JIE,
	CHR_DA,
	CHR_JI,
	CHR_XING,
	CHR_DIAN,
	CHR_YA,
	CHR_LIU,
	CHR_SHI,
	CHR_JIAN,
	CHR_RONG,
	CHR_LIANG,
	CHR_QU,
	CHR_XIAN,
	FONT_CHINESE_MAX
} FONT_CHINESE_T;

/*Extern data & funtion----------------------------------------*/
extern ConfigData_t stSelConfigData;

/*Global data -------------------------------------------------*/
BYTE byMenuLevel = 0;
BYTE byMenuL0CurIndex = 0;
BYTE byMenuL1CurIndex = 0;
BYTE byMenuL2CurIndex = 0;

U8 byMID = MID_TOP;
U16 wdML1Offset = 0;
U16 wdML1OffsetMax = 0;
U16 wdML1OffsetSel = 0xffff; /* 0xffff 表示未选定 */
U16 wdML2CfgOffset = 0;
U16 wdML2CfgOffsetMax = 0;
U16 wdML2CfgOffsetSel = 0xffff; /* 0xffff 表示未选定 */
U16 wdML2ChgOffset = 0;
U16 wdML2ChgOffsetMax = 0;
U16 wdML2ChgOffsetSel = 0xffff; /* 0xffff 表示未选定 */

/*datas & funcitons define area -------------------------------*/
S16 CurX = 0;
S16 CurY = 0;
BYTE byLineWidth = 1;
WORD wdCoTimeMin = 0;
WORD wdCoTimeScale = 1;
BYTE wdCoTimeMarkSpace = 8;
WORD wdCoUMin = 0;
WORD wdCoUMax = 0;
BYTE wdCoUMarkSpace = 8;
WORD wdCoUScale = 1;
BYTE wdCoIMarkSpace = 8;
WORD wdCoIMin = 0;
WORD wdCoIMax = 0;
WORD wdCoIScale = 1;

//DWORD dwdCurT = 0;
BYTE byDispNum = 0;

U16 CurveTMin = 0;
U16 CurveTMax = 0;
U16 CurveUMin = 0;
U16 CurveUMax = 0;
U16 CurveIMin = 0;
U16 CurveIMax = 0;
U16 UnitT = 1;
U16 UnitU = 1;
U16 UnitI = 1;

S16 TMarkSpace = 20;
S16 UMarkSpace = 20;
S16 IMarkSpace = 20;
U32 dwdDispUITCnt = 0;
U16 wdDispCurUITFileInex = 0;
INT iDispCurErr = 0;

U32 dwdDispChgTmLen = 0;
U32 dwdDispChgCapacity = 0;
//ConfigData_t stDispCfgData;





WORD CurveUBuf[DISPLAY_CURVE_WIDTH];
WORD CurveIBuf[DISPLAY_CURVE_WIDTH];

const BYTE abyCfgCurveUBuf[CCE_MAX][100] =
{
	{
		10,14,18,21,24,27,30,33,36,38,40,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,
		58,59,60,61,62,63,64,64,65,65,65,66,66,66,67,67,67,68,68,69,70,71,72,73,75,75,75,
		75,76,76,76,76,76,76,76,77,77,77,77,77,77,77,78,78,78,78,78,78,79,79,79,79,79,79,
		80,80,80,80,80,80,80,70,70,70,70,70,70,70,70,70,70,70,70
	},
	{
		10,15,20,24,28,31,34,37,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,
		70,70,71,71,71,72,72,72,72,73,73,73,73,74,74,74,74,75,75,75,75,76,76,76,76,77,77,
		77,77,77,77,77,77,78,78,78,78,78,78,78,78,78,79,79,79,79,79,79,79,79,79,79,80,80,
		80,80,80,80,80,80,80,70,70,70,70,70,70,70,70,70,70,70,70
	},
	{
		10,15,20,24,28,31,34,37,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,75,
		75,76,76,76,76,77,77,77,77,77,78,78,78,78,78,79,79,79,79,79,80,80,80,80,81,81,82,
		82,83,83,84,84,84,85,85,85,86,86,86,86,86,87,87,87,87,87,88,88,88,88,88,89,89,89,
		89,89,89,90,90,90,90,70,70,70,70,70,70,70,70,70,70,70,70
	},
	{
		10,15,20,25,30,34,38,42,46,50,53,56,59,62,66,70,72,75,70,71,72,72,73,73,74,74,75,
		75,75,76,76,76,76,77,77,77,77,77,78,78,78,78,78,78,78,79,79,79,79,79,79,79,79,79,
		79,79,79,79,79,80,80,80,80,80,80,80,80,82,83,84,84,85,85,85,86,86,86,87,87,88,88,
		88,89,89,89,90,90,90,70,70,70,70,70,70,70,70,70,70,70,70
	}
};

const BYTE abyCfgCurveIBuf[CCE_MAX][100] =
{
	{
		80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,
		80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,79,74,
		69,64,59,55,51,47,43,40,37,34,31,29,27,25,23,21,19,17,16,15,14,14,13,13,13,12,12,
		12,12,11,11,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10
	},
	{
		80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,
		60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,57,53,
		50,47,44,41,38,36,34,32,30,28,26,24,22,20,18,17,16,15,15,14,14,14,13,13,13,12,12,
		12,12,11,11,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10
	},
	{
		80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,
		78,76,72,68,64,59,55,51,48,45,42,39,36,33,28,26,24,22,20,18,17,17,16,16,15,15,15,
		15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
		15,15,15,15,15,15,15,10,10,10,10,10,10,10,10,10,10,10,10
	},
	{
		80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,60,60,60,60,60,60,60,60,60,
		60,60,60,60,60,60,60,60,60,60,60,57,53,50,47,44,41,38,36,34,32,30,28,26,25,24,23,
		22,21,20,19,19,18,18,17,17,17,16,16,16,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
		15,15,15,15,15,15,15,10,10,10,10,10,10,10,10,10,10,10,10
	}
};


const BYTE aBitMask[8] = 
{
	//0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
	0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01
};

const BYTE abyFontDigAlph[FONT_DIG_ALPH_MAX][16] = 
{
/*	  (0)  -(0) _(1) :(2) .(2) 0(3) 1(4) 2(5) 3(6) 4(7) 5(8) 6(9) 7(10) 8(11) 9(12) a(13) b(14) c(15)
 d(16) e(17) f(18) g(19) h(20) i(21) j(22) k(23) l(24) m(25) n(26) o(27) p(28) q(29) r(30) s(31)
 t(32) u(33) v(34) w(35) x(36) y(37) z(38) A(39) B(40) C(41) D(42) E(43) F(44) G(45) H(46) I(47)
 J(48) K(49) L(50) M(51) N(52) O(53) P(54) Q(55) R(56) S(57) T(58) U(59) V(60) W(61) X(62) Y(63)
 Z(64)
*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
	
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",0*/
	
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF},/*"_",1*/
	
	{0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00},/*":",2*/
	
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00},/*".",2*/
	
	{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"0",3*/
	
	{0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"1",4*/
	
	{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x04,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00},/*"2",5*/
	
	{0x00,0x00,0x00,0x3C,0x42,0x42,0x04,0x18,0x04,0x02,0x02,0x42,0x44,0x38,0x00,0x00},/*"3",6*/
	
	{0x00,0x00,0x00,0x04,0x0C,0x14,0x24,0x24,0x44,0x44,0x7E,0x04,0x04,0x1E,0x00,0x00},/*"4",7*/
	
	{0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00},/*"5",8*/
	
	{0x00,0x00,0x00,0x1C,0x24,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"6",9*/
	
	{0x00,0x00,0x00,0x7E,0x44,0x44,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},/*"7",10*/
	
	{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",11*/
	
	{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x26,0x1A,0x02,0x02,0x24,0x38,0x00,0x00},/*"9",12*/
	
	{0x00,0x00,0x00,0x10,0x10,0x18,0x28,0x28,0x24,0x3C,0x44,0x42,0x42,0xE7,0x00,0x00},/*"A",39*/
	
	{0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x78,0x44,0x42,0x42,0x42,0x44,0xF8,0x00,0x00},/*"B",40*/
	
	{0x00,0x00,0x00,0x3E,0x42,0x42,0x80,0x80,0x80,0x80,0x80,0x42,0x44,0x38,0x00,0x00},/*"C",41*/
	
	{0x00,0x00,0x00,0xF8,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0xF8,0x00,0x00},/*"D",42*/
	
	{0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x42,0x42,0xFC,0x00,0x00},/*"E",43*/
	
	{0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x40,0x40,0xE0,0x00,0x00},/*"F",44*/
	
	{0x00,0x00,0x00,0x3C,0x44,0x44,0x80,0x80,0x80,0x8E,0x84,0x44,0x44,0x38,0x00,0x00},/*"G",45*/
	
	{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"H",46*/
	
	{0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"I",47*/
	
	{0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0xF0},/*"J",48*/
	
	{0x00,0x00,0x00,0xEE,0x44,0x48,0x50,0x70,0x50,0x48,0x48,0x44,0x44,0xEE,0x00,0x00},/*"K",49*/
	
	{0x00,0x00,0x00,0xE0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x42,0xFE,0x00,0x00},/*"L",50*/
	
	{0x00,0x00,0x00,0xEE,0x6C,0x6C,0x6C,0x6C,0x54,0x54,0x54,0x54,0x54,0xD6,0x00,0x00},/*"M",51*/
	
	{0x00,0x00,0x00,0xC7,0x62,0x62,0x52,0x52,0x4A,0x4A,0x4A,0x46,0x46,0xE2,0x00,0x00},/*"N",52*/
	
	{0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00},/*"O",53*/
	
	{0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x42,0x7C,0x40,0x40,0x40,0x40,0xE0,0x00,0x00},/*"P",54*/
	
	{0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0xB2,0xCA,0x4C,0x38,0x06,0x00},/*"Q",55*/
	
	{0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x7C,0x48,0x48,0x44,0x44,0x42,0xE3,0x00,0x00},/*"R",56*/
	
	{0x00,0x00,0x00,0x3E,0x42,0x42,0x40,0x20,0x18,0x04,0x02,0x42,0x42,0x7C,0x00,0x00},/*"S",57*/
	
	{0x00,0x00,0x00,0xFE,0x92,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00},/*"T",58*/
	
	{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"U",59*/
	
	{0x00,0x00,0x00,0xE7,0x42,0x42,0x44,0x24,0x24,0x28,0x28,0x18,0x10,0x10,0x00,0x00},/*"V",60*/
	
	{0x00,0x00,0x00,0xD6,0x92,0x92,0x92,0x92,0xAA,0xAA,0x6C,0x44,0x44,0x44,0x00,0x00},/*"W",61*/
	
	{0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00},/*"X",62*/
	
	{0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00},/*"Y",63*/
	
	{0x00,0x00,0x00,0x7E,0x84,0x04,0x08,0x08,0x10,0x20,0x20,0x42,0x42,0xFC,0x00,0x00},/*"Z",64*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x1E,0x22,0x42,0x42,0x3F,0x00,0x00},/*"a",13*/

	{0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x64,0x58,0x00,0x00},/*"b",14*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x40,0x40,0x40,0x22,0x1C,0x00,0x00},/*"c",15*/

	{0x00,0x00,0x00,0x06,0x02,0x02,0x02,0x1E,0x22,0x42,0x42,0x42,0x26,0x1B,0x00,0x00},/*"d",16*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x7E,0x40,0x40,0x42,0x3C,0x00,0x00},/*"e",17*/

	{0x00,0x00,0x00,0x0F,0x11,0x10,0x10,0x7E,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"f",18*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x44,0x44,0x38,0x40,0x3C,0x42,0x42,0x3C},/*"g",19*/

	{0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"h",20*/

	{0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"i",21*/

	{0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x1C,0x04,0x04,0x04,0x04,0x04,0x04,0x44,0x78},/*"j",22*/

	{0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x4E,0x48,0x50,0x68,0x48,0x44,0xEE,0x00,0x00},/*"k",23*/

	{0x00,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"l",24*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x49,0x49,0x49,0x49,0x49,0xED,0x00,0x00},/*"m",25*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"n",26*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"o",27*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0x64,0x42,0x42,0x42,0x44,0x78,0x40,0xE0},/*"p",28*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x22,0x42,0x42,0x42,0x22,0x1E,0x02,0x07},/*"q",29*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x32,0x20,0x20,0x20,0x20,0xF8,0x00,0x00},/*"r",30*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x42,0x40,0x3C,0x02,0x42,0x7C,0x00,0x00},/*"s",31*/

	{0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x7C,0x10,0x10,0x10,0x10,0x10,0x0C,0x00,0x00},/*"t",32*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0x42,0x42,0x42,0x42,0x46,0x3B,0x00,0x00},/*"u",33*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x10,0x10,0x00,0x00},/*"v",34*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD7,0x92,0x92,0xAA,0xAA,0x44,0x44,0x00,0x00},/*"w",35*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x24,0x18,0x18,0x18,0x24,0x76,0x00,0x00},/*"x",36*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x18,0x10,0x10,0xE0},/*"y",37*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x44,0x08,0x10,0x10,0x22,0x7E,0x00,0x00},/*"z",38*/


};

const BYTE FontChinese[FONT_CHINESE_MAX][32] = 
{
	/*
	 深(0) 圳(1) 宇(2) 杰(3) 达(4) 机(5) 型(6) 电(7) 压(8) 流(9) 时(10) 间(11) 容(12) 量(13) 曲(14) 线(15)
	
	*/
#if 0

{0x00,0x00,0xE4,0x3F,0x28,0x20,0x28,0x25,0x81,0x08,0x42,0x10,0x02,0x02,0x08,0x02,0xE8,0x3F,0x04,0x02,0x07,0x07,0x84,0x0A,0x44,0x12,0x34,0x62,0x04,0x02,0x00,0x02},/*"深",0*/

{0x88,0x20,0x88,0x24,0x88,0x24,0x88,0x24,0x88,0x24,0xBF,0x24,0x88,0x24,0x88,0x24,0x88,0x24,0x88,0x24,0x88,0x24,0xB8,0x24,0x87,0x24,0x42,0x24,0x40,0x20,0x20,0x20},/*"圳",1*/

{0x40,0x00,0x80,0x00,0xFE,0x7F,0x02,0x40,0x01,0x20,0xFC,0x1F,0x80,0x00,0x80,0x00,0x80,0x00,0xFF,0x7F,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xA0,0x00,0x40,0x00},/*"宇",2*/

{0x80,0x00,0x80,0x00,0x80,0x00,0xFE,0x3F,0xC0,0x01,0xA0,0x02,0x90,0x04,0x88,0x08,0x84,0x10,0x83,0x60,0x80,0x00,0x00,0x00,0x24,0x11,0x44,0x22,0x42,0x22,0x01,0x20},/*"杰",3*/

{0x00,0x02,0x04,0x02,0x08,0x02,0x08,0x02,0xE0,0x7F,0x00,0x02,0x0F,0x02,0x08,0x05,0x08,0x09,0x88,0x10,0x48,0x20,0x28,0x20,0x08,0x00,0x14,0x00,0xE2,0x7F,0x00,0x00},/*"达",4*/

{0x08,0x00,0x88,0x0F,0x88,0x08,0x88,0x08,0xBF,0x08,0x88,0x08,0x8C,0x08,0x9C,0x08,0xAA,0x08,0xAA,0x08,0x89,0x08,0x88,0x48,0x88,0x48,0x48,0x48,0x48,0x70,0x28,0x00},/*"机",5*/

{0x00,0x20,0xFE,0x21,0x48,0x24,0x48,0x24,0x48,0x24,0xFF,0x25,0x48,0x24,0x44,0x20,0x44,0x28,0x82,0x10,0x81,0x00,0xFC,0x1F,0x80,0x00,0x80,0x00,0xFF,0x7F,0x00,0x00},/*"型",6*/

{0x80,0x00,0x80,0x00,0x80,0x00,0xFC,0x1F,0x84,0x10,0x84,0x10,0x84,0x10,0xFC,0x1F,0x84,0x10,0x84,0x10,0x84,0x10,0xFC,0x1F,0x84,0x50,0x80,0x40,0x80,0x40,0x00,0x7F},/*"电",7*/

{0x00,0x00,0xFC,0x7F,0x04,0x00,0x04,0x01,0x04,0x01,0x04,0x01,0x04,0x01,0xF4,0x3F,0x04,0x01,0x04,0x01,0x04,0x09,0x04,0x11,0x04,0x11,0x02,0x01,0xFA,0x7F,0x01,0x00},/*"压",8*/

{0x00,0x01,0x04,0x02,0xE8,0x7F,0x08,0x01,0x81,0x08,0x42,0x10,0xE2,0x3F,0x08,0x20,0x08,0x00,0x44,0x12,0x47,0x12,0x44,0x12,0x44,0x12,0x44,0x52,0x24,0x52,0x10,0x62},/*"流",9*/

{0x00,0x10,0x00,0x10,0x3E,0x10,0x22,0x10,0xA2,0x7F,0x22,0x10,0x22,0x10,0x3E,0x10,0x22,0x11,0x22,0x12,0x22,0x12,0x22,0x10,0x3E,0x10,0x22,0x10,0x00,0x14,0x00,0x08},/*"时",10*/

{0x04,0x00,0xC8,0x3F,0x08,0x20,0x02,0x20,0xE2,0x23,0x22,0x22,0x22,0x22,0x22,0x22,0xE2,0x23,0x22,0x22,0x22,0x22,0x22,0x22,0xE2,0x23,0x02,0x20,0x02,0x28,0x02,0x10},/*"间",11*/

{0x40,0x00,0x80,0x00,0xFE,0x7F,0x02,0x40,0x11,0x24,0x88,0x08,0x44,0x11,0x20,0x02,0x10,0x04,0x0C,0x18,0xFB,0x6F,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0x0F,0x08,0x08},/*"容",12*/

{0x00,0x00,0xF8,0x0F,0x08,0x08,0xF8,0x0F,0x08,0x08,0xFF,0x7F,0x00,0x00,0xF8,0x0F,0x88,0x08,0xF8,0x0F,0x88,0x08,0xF8,0x0F,0x80,0x00,0xF8,0x0F,0x80,0x00,0xFE,0x3F},/*"量",13*/

{0x20,0x02,0x20,0x02,0x20,0x02,0x20,0x02,0xFE,0x3F,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0xFE,0x3F,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0xFE,0x3F,0x02,0x20},/*"曲",14*/

{0x08,0x0A,0x08,0x12,0x04,0x02,0x24,0x3A,0xA2,0x07,0x1F,0x02,0x08,0x7A,0xC4,0x07,0x02,0x22,0x3F,0x12,0x02,0x0C,0x00,0x44,0x38,0x4A,0x07,0x51,0xC2,0x60,0x00,0x40},/*"线",15*/

#else
		{0x00,0x00,0x27,0xFC,0x14,0x04,0x14,0xA4,0x81,0x10,0x42,0x08,0x40,0x40,0x10,0x40,0x17,0xFC,0x20,0x40,0xE0,0xE0,0x21,0x50,0x22,0x48,0x2C,0x46,0x20,0x40,0x00,0x40},/*"深",0*/
		
		{0x11,0x04,0x11,0x24,0x11,0x24,0x11,0x24,0x11,0x24,0xFD,0x24,0x11,0x24,0x11,0x24,0x11,0x24,0x11,0x24,0x11,0x24,0x1D,0x24,0xE1,0x24,0x42,0x24,0x02,0x04,0x04,0x04},/*"圳",1*/
		
		{0x02,0x00,0x01,0x00,0x7F,0xFE,0x40,0x02,0x80,0x04,0x3F,0xF8,0x01,0x00,0x01,0x00,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00},/*"宇",2*/
		
		{0x01,0x00,0x01,0x00,0x01,0x00,0x7F,0xFC,0x03,0x80,0x05,0x40,0x09,0x20,0x11,0x10,0x21,0x08,0xC1,0x06,0x01,0x00,0x00,0x00,0x24,0x88,0x22,0x44,0x42,0x44,0x80,0x04},/*"杰",3*/
		
		{0x00,0x40,0x20,0x40,0x10,0x40,0x10,0x40,0x07,0xFE,0x00,0x40,0xF0,0x40,0x10,0xA0,0x10,0x90,0x11,0x08,0x12,0x04,0x14,0x04,0x10,0x00,0x28,0x00,0x47,0xFE,0x00,0x00},/*"达",4*/
		
		{0x10,0x00,0x11,0xF0,0x11,0x10,0x11,0x10,0xFD,0x10,0x11,0x10,0x31,0x10,0x39,0x10,0x55,0x10,0x55,0x10,0x91,0x10,0x11,0x12,0x11,0x12,0x12,0x12,0x12,0x0E,0x14,0x00},/*"机",5*/
		
		{0x00,0x04,0x7F,0x84,0x12,0x24,0x12,0x24,0x12,0x24,0xFF,0xA4,0x12,0x24,0x22,0x04,0x22,0x14,0x41,0x08,0x81,0x00,0x3F,0xF8,0x01,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00},/*"型",6*/
		
		{0x01,0x00,0x01,0x00,0x01,0x00,0x3F,0xF8,0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x0A,0x01,0x02,0x01,0x02,0x00,0xFE},/*"电",7*/
		
		{0x00,0x00,0x3F,0xFE,0x20,0x00,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x80,0x2F,0xFC,0x20,0x80,0x20,0x80,0x20,0x90,0x20,0x88,0x20,0x88,0x40,0x80,0x5F,0xFE,0x80,0x00},/*"压",8*/
		
		{0x00,0x80,0x20,0x40,0x17,0xFE,0x10,0x80,0x81,0x10,0x42,0x08,0x47,0xFC,0x10,0x04,0x10,0x00,0x22,0x48,0xE2,0x48,0x22,0x48,0x22,0x48,0x22,0x4A,0x24,0x4A,0x08,0x46},/*"流",9*/
		
		{0x00,0x08,0x00,0x08,0x7C,0x08,0x44,0x08,0x45,0xFE,0x44,0x08,0x44,0x08,0x7C,0x08,0x44,0x88,0x44,0x48,0x44,0x48,0x44,0x08,0x7C,0x08,0x44,0x08,0x00,0x28,0x00,0x10},/*"时",10*/
		
		{0x20,0x00,0x13,0xFC,0x10,0x04,0x40,0x04,0x47,0xC4,0x44,0x44,0x44,0x44,0x44,0x44,0x47,0xC4,0x44,0x44,0x44,0x44,0x44,0x44,0x47,0xC4,0x40,0x04,0x40,0x14,0x40,0x08},/*"间",11*/
		
		{0x02,0x00,0x01,0x00,0x7F,0xFE,0x40,0x02,0x88,0x24,0x11,0x10,0x22,0x88,0x04,0x40,0x08,0x20,0x30,0x18,0xDF,0xF6,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10},/*"容",12*/
		
		{0x00,0x00,0x1F,0xF0,0x10,0x10,0x1F,0xF0,0x10,0x10,0xFF,0xFE,0x00,0x00,0x1F,0xF0,0x11,0x10,0x1F,0xF0,0x11,0x10,0x1F,0xF0,0x01,0x00,0x1F,0xF0,0x01,0x00,0x7F,0xFC},/*"量",13*/
		
		{0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x7F,0xFC,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x7F,0xFC,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x7F,0xFC,0x40,0x04},/*"曲",14*/
		
		{0x10,0x50,0x10,0x48,0x20,0x40,0x24,0x5C,0x45,0xE0,0xF8,0x40,0x10,0x5E,0x23,0xE0,0x40,0x44,0xFC,0x48,0x40,0x30,0x00,0x22,0x1C,0x52,0xE0,0x8A,0x43,0x06,0x00,0x02},/*"线",15*/
#endif
};


const BYTE abyFontL2R5x7[18][7] =
{
 // (0) A(1) V(2) M(3) -(4) >(5) <(6) 0(7) 1(8) 2(9) 3(10) 4(11) 5(12) 6(13) 7(14) 8(15) 9(16)

{0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
{0x00,0x00,0x00,0x00,0x00,0x40,0x00},/*".",0*/

{0x00,0x20,0x60,0x50,0x70,0x88,0x00},/*"A",1*/

{0x00,0x88,0x50,0x50,0x60,0x20,0x00},/*"V",2*/

{0x00,0xD8,0xD8,0xE8,0xA8,0xA8,0x00},/*"M",3*/

{0x00,0x00,0x00,0xF8,0x00,0x00,0x00},/*"-",4*/

{0x00,0x80,0x60,0x18,0x20,0x40,0x00},/*">",5*/

{0x00,0x10,0x20,0xC0,0x20,0x10,0x00},/*"<",6*/

{0x00,0x70,0x90,0x90,0x90,0x70,0x00},/*"0",7*/

{0x00,0x20,0x60,0x20,0x20,0x20,0x00},/*"1",8*/

{0x00,0x70,0x10,0x10,0x20,0x70,0x00},/*"2",9*/

{0x00,0x70,0x10,0x30,0x10,0x70,0x00},/*"3",10*/

{0x00,0x10,0x30,0x50,0xF8,0x10,0x00},/*"4",11*/

{0x00,0x70,0x80,0xF0,0x10,0x70,0x00},/*"5",12*/

{0x00,0x20,0x40,0x70,0x88,0x70,0x00},/*"6",13*/

{0x00,0xF8,0x10,0x20,0x20,0x40,0x00},/*"7",14*/

{0x00,0x70,0x90,0x70,0x90,0x70,0x00},/*"8",15*/

{0x00,0x70,0x90,0xD0,0x20,0x40,0x00},/*"9",16*/
};

const BYTE abyFontT2B5x7[18][5] =
{
//(0) A(1) V(2) M(3) -(4) >(5) <(6) 0(7) 1(8) 2(9) 3(10) 4(11) 5(12) 6(13) 7(14) 8(15) 9(16)

{0x00<<1, 0x00<<1, 0x00<<1, 0x00<<1, 0x00<<1 },/*" ",0*/
{0x00<<1, 0x20<<1, 0x00<<1, 0x00<<1, 0x00<<1 },/*".",0*/

{0x20<<1, 0x1C<<1, 0x16<<1, 0x18<<1, 0x20<<1 },/*"A",1*/

{0x02<<1, 0x1C<<1, 0x30<<1, 0x0C<<1, 0x02<<1 },/*"V",2*/

{0x3E<<1, 0x0E<<1, 0x38<<1, 0x06<<1, 0x3E<<1 },/*"M",3*/

{0x08<<1, 0x08<<1, 0x08<<1, 0x08<<1, 0x08<<1 },/*"-",4*/

{0x02<<1, 0x24<<1, 0x14<<1, 0x08<<1, 0x08<<1 },/*">",5*/

{0x08<<1, 0x08<<1, 0x14<<1, 0x22<<1, 0x00<<1 },/*"<",6*/

{0x1C<<1, 0x22<<1, 0x22<<1, 0x3E<<1, 0x00<<1 },/*"0",7*/

{0x00<<1, 0x04<<1, 0x3E<<1, 0x00<<1, 0x00<<1 },/*"1",8*/

{0x00<<1, 0x22<<1, 0x32<<1, 0x2E<<1, 0x00<<1 },/*"2",9*/

{0x00<<1, 0x22<<1, 0x2A<<1, 0x3E<<1, 0x00<<1 },/*"3",10*/

{0x10<<1, 0x18<<1, 0x14<<1, 0x3E<<1, 0x10<<1 },/*"4",11*/

{0x0C<<1, 0x2A<<1, 0x2A<<1, 0x3A<<1, 0x00<<1 },/*"5",12*/

{0x10<<1, 0x2C<<1, 0x2A<<1, 0x28<<1, 0x10<<1 },/*"6",13*/

{0x02<<1, 0x22<<1, 0x1A<<1, 0x06<<1, 0x02<<1 },/*"7",14*/

{0x14<<1, 0x2A<<1, 0x2A<<1, 0x3E<<1, 0x00<<1 },/*"8",15*/

{0x0C<<1, 0x2A<<1, 0x12<<1, 0x0E<<1, 0x00<<1 },/*"9",16*/
	

};

const BYTE abyFontB2T5x7[18][5] =
{
	 // (0) A(1) V(2) M(3) -(4) >(5) <(6) 0(7) 1(8) 2(9) 3(10) 4(11) 5(12) 6(13) 7(14) 8(15) 9(16)

{0x00,0x00,0x00,0x00,0x00},/*" ",0*/
{0x00,0x04,0x00,0x00,0x00},/*".",0*/

{0x04,0x38,0x68,0x18,0x04},/*"A",1*/

{0x40,0x38,0x0C,0x30,0x40},/*"V",2*/

{0x7C,0x70,0x1C,0x60,0x7C},/*"M",3*/

{0x10,0x10,0x10,0x10,0x10},/*"-",4*/

{0x40,0x24,0x28,0x10,0x10},/*">",5*/

{0x10,0x10,0x28,0x44,0x00},/*"<",6*/

{0x38,0x44,0x44,0x7C,0x00},/*"0",7*/

{0x00,0x20,0x7C,0x00,0x00},/*"1",8*/

{0x00,0x44,0x4C,0x74,0x00},/*"2",9*/

{0x00,0x44,0x54,0x7C,0x00},/*"3",10*/

{0x08,0x18,0x28,0x7C,0x08},/*"4",11*/

{0x30,0x54,0x54,0x5C,0x00},/*"5",12*/

{0x08,0x34,0x54,0x14,0x08},/*"6",13*/

{0x40,0x44,0x58,0x60,0x40},/*"7",14*/

{0x28,0x54,0x54,0x7C,0x00},/*"8",15*/

{0x30,0x54,0x48,0x70,0x00},/*"9",16*/
};

const BYTE abyLogoBmp[16][30] = 
{
#if 1
	/*--  调入了一幅图像：F:\cloud\data\work\charger\codes\powerfirst.bmp  --*/
	/*--  宽度x高度=240x16  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x80,
	0x00,0x00,0x00,0x0F,0xC3,0x00,0x00,0x00,0x00,0x0F,0xC0,0x00,0x0C,0x00,0x00,0x03,
	0x00,0x00,0x00,0x00,0x07,0x80,0x00,0x00,0x0C,0x0F,0xCF,0x00,0x0C,0xC0,0x00,0x00,
	0x00,0x0C,0x00,0x00,0x00,0x06,0x00,0x03,0x00,0x00,0x0C,0x00,0x00,0x03,0x00,0x00,
	0x00,0x00,0x0C,0xC0,0x00,0x00,0x0C,0x03,0x0D,0x80,0x0C,0xC0,0x00,0x00,0x00,0x0C,
	0x00,0x00,0x00,0x06,0x00,0x03,0x00,0x00,0x0C,0x00,0x00,0x03,0x00,0x00,0x00,0x00,
	0x0C,0xC0,0x00,0x00,0x0C,0x03,0x0D,0xC0,0x0C,0xC7,0x9B,0xC7,0x8F,0x8C,0x03,0x0F,
	0x8F,0x9F,0x80,0x03,0x07,0x87,0x8F,0x8F,0x87,0x83,0x07,0x87,0xCC,0xC0,0x0C,0x07,
	0x80,0x00,0x0C,0x03,0x0C,0xC0,0x0F,0x8D,0xDF,0xCD,0xCE,0x0F,0x83,0x0E,0x0D,0x86,
	0x00,0x03,0x0D,0xCD,0xCC,0xCC,0xCD,0xC3,0x0D,0xCD,0x8D,0x80,0x1C,0x0D,0xC0,0x00,
	0x0C,0x03,0x0C,0xC0,0x0C,0x1C,0xCF,0xCF,0xCE,0x0C,0x03,0x0E,0x0F,0x06,0x00,0x03,
	0x0F,0xDC,0x0C,0xCC,0xDC,0xC3,0x1C,0xCD,0x8D,0x80,0x1C,0xDC,0xC0,0x00,0x0C,0x03,
	0x0C,0xC0,0x0C,0x1C,0xCF,0x8C,0x0C,0x0C,0x03,0x0C,0x01,0x86,0x00,0x03,0x0C,0x1C,
	0xCC,0xCC,0xDC,0xC3,0x1C,0xCF,0x07,0x80,0x0C,0xDC,0xC0,0x00,0x0C,0x03,0x0D,0xC0,
	0x0C,0x0D,0xCF,0x8D,0xCC,0x0C,0x03,0x0C,0x0D,0xC6,0x00,0x03,0x0D,0xCD,0xCC,0xCC,
	0xCD,0xC3,0x0D,0xCC,0x07,0x00,0x0D,0xCD,0xC0,0x00,0x0C,0x03,0x0D,0x80,0x0C,0x07,
	0x8D,0x87,0x8C,0x0C,0x03,0x0C,0x07,0x83,0xC0,0x03,0x07,0x87,0x8C,0xCC,0xC7,0x83,
	0x07,0x8F,0xC7,0x00,0x07,0x87,0x8C,0x00,0x0F,0xC3,0x0F,0x0C,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,
	0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x8E,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#else
	/*--  宽度x高度=240x16  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x80,
	0x00,0x00,0x00,0x00,0x3F,0x0C,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0xC0,0x00,0x00,
	0x30,0x00,0x00,0x00,0x00,0xF0,0x00,0x00,0x30,0x3F,0x3C,0x00,0x0C,0xC0,0x00,0x00,
	0x00,0x00,0x30,0x00,0x00,0x00,0x18,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x30,0x00,
	0x00,0x00,0x01,0x98,0x00,0x00,0x30,0x0C,0x36,0x00,0x0C,0xC0,0x00,0x00,0x00,0x00,
	0x30,0x00,0x00,0x00,0x18,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x30,0x00,0x00,0x00,
	0x01,0x98,0x00,0x00,0x30,0x0C,0x37,0x00,0x0C,0xC7,0x9B,0xC7,0x8F,0x80,0x30,0x0C,
	0x3E,0x3E,0x7E,0x00,0x30,0x78,0x78,0xF8,0xF8,0x78,0x30,0x78,0x7C,0xCC,0x01,0x80,
	0xF0,0x00,0x30,0x0C,0x33,0x00,0x0F,0x8D,0xDF,0xCD,0xCE,0x00,0x3E,0x0C,0x38,0x32,
	0x18,0x00,0x30,0xDC,0xDC,0xCC,0xCC,0xDC,0x30,0xDC,0xD8,0xD8,0x03,0x81,0xB8,0x00,
	0x30,0x0C,0x33,0x00,0x0C,0x1C,0xCF,0xCF,0xCE,0x00,0x30,0x0C,0x38,0x3C,0x18,0x00,
	0x30,0xFD,0xC0,0xCC,0xCD,0xCC,0x31,0xCC,0xD8,0xD8,0x03,0x9B,0x98,0x00,0x30,0x0C,
	0x33,0x00,0x0C,0x1C,0xCF,0x8C,0x0C,0x00,0x30,0x0C,0x30,0x06,0x18,0x00,0x30,0xC1,
	0xCC,0xCC,0xCD,0xCC,0x31,0xCC,0xF0,0x78,0x01,0x9B,0x98,0x00,0x30,0x0C,0x37,0x00,
	0x0C,0x0D,0xCF,0x8D,0xCC,0x00,0x30,0x0C,0x30,0x37,0x18,0x00,0x30,0xDC,0xDC,0xCC,
	0xCC,0xDC,0x30,0xDC,0xC0,0x70,0x01,0xB9,0xB9,0x80,0x30,0x0C,0x36,0x30,0x0C,0x07,
	0x8D,0x87,0x8C,0x00,0x30,0x0C,0x30,0x1E,0x0F,0x00,0x30,0x78,0x78,0xCC,0xCC,0x78,
	0x30,0x78,0xFC,0x70,0x00,0xF0,0xF1,0x80,0x3F,0x0C,0x3C,0x30,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xCC,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xE0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

#endif
};




void MoveTo(S16 x, S16 y);
void LineTo(S16 x1, S16 y1);
void DrawLine(S16 x0, S16 y0, S16 x1, S16 y1);
void DrawDot(S16 x, S16 y);
void DrawDotMode(S16 x, S16 y, BYTE mode);
void TextOut(S16 x, S16 y, BYTE *pData);

BYTE DisplayInit(void);
void DisplayMenu(void);
void DisplayCurve(void);
void Display(void);
void GetDispCurveValue(void);
void AdjustUIValue(void);

/*Functions ---------------------------------------------------*/
/****************************************************************
* Function:		
* Description:	
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void MoveTo(S16 x, S16 y)
{
	y = DISPLAY_AREA_HEIGHT - 1 - y;
	CurX = x;
	CurY = y;
}

/****************************************************************
* Function:		
* Description:	
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawDot(S16 x, S16 y)
{
	//y = DISPLAY_AREA_HEIGHT - 1 - y;

	
	
	if (y >= DISPLAY_AREA_HEIGHT)
	{
		return;
	}
	if ((x>>3) >= DISPLAY_AREA_WIDTH)
	{
		return;
	}

	if (y < 0 || x < 0)
	{
		return;
	}
	
	LcdData[y][x>>3] |= aBitMask[x&0x07];
}

void DrawDotEx(S16 x, S16 y)
{
	y = DISPLAY_AREA_HEIGHT - 1 - y;

	if (y >= DISPLAY_AREA_HEIGHT)
	{
		return;
	}
	if ((x>>3) >= DISPLAY_AREA_WIDTH)
	{
		return;
	}

	if (y < 0 || x < 0)
	{
		return;
	}
	
	LcdData[y][x>>3] |= aBitMask[x&0x07];
}

/****************************************************************
* Function:		
* Description:	
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawDotMode(S16 x, S16 y, BYTE mode)
{
	
	if (y >= DISPLAY_AREA_HEIGHT)
	{
		return;
	}
	if ((x>>3) >= DISPLAY_AREA_WIDTH)
	{
		return;
	}

	if (y < 0 || x < 0)
	{
		return;
	}
	
	if (DRAW_MODE_OR == mode)
	{
		LcdData[y][x>>3] |= aBitMask[x&0x07];
	}
	else if (DRAW_MODE_AND == mode)
	{
		LcdData[y][x>>3] &= ~aBitMask[x&0x07];
	}
	else if (DRAW_MODE_XOR == mode)
	{
		LcdData[y][x>>3] ^= aBitMask[x&0x07];
	}
	
}

void DrawDotModeEx(S16 x, S16 y, BYTE mode)
{
	y = DISPLAY_AREA_HEIGHT - 1 - y;
	if (y >= DISPLAY_AREA_HEIGHT)
	{
		return;
	}
	if ((x>>3) >= DISPLAY_AREA_WIDTH)
	{
		return;
	}

	if (y < 0 || x < 0)
	{
		return;
	}
	
	if (DRAW_MODE_OR == mode)
	{
		LcdData[y][x>>3] |= aBitMask[x&0x07];
	}
	else if (DRAW_MODE_AND == mode)
	{
		LcdData[y][x>>3] &= ~aBitMask[x&0x07];
	}
	else if (DRAW_MODE_XOR == mode)
	{
		LcdData[y][x>>3] ^= aBitMask[x&0x07];
	}
	
}


//横线显示当y相等时
void DrawLineRow(S16 x0,S16 x1,S16 y,S16 color)
{
 S16 t;

 //y = DISPLAY_AREA_HEIGHT - 1 - y;
 
 if(x0 > x1)
 {
  t=x1;
  x1=x0;
  x0=t;
 }
 for(;x0 <= x1;x0++)
 {
  DrawDotMode(x0,y,color);
 }
}

//竖线显示当x相等时
void DrawLineCol(S16 x,S16 y0,S16 y1,S16 color)
{
	
 S16 z;

 //y0 = DISPLAY_AREA_HEIGHT - 1 - y0;
//	y1 = DISPLAY_AREA_HEIGHT - 1 - y1;
	
 if(y0>y1)
 {
  z=y1;
  y1=y0;
  y0=z;
 }
 for(;y0<=y1;y0++)
 {
  DrawDotMode(x,y0,color);
 }
}



/****************************************************************
* Function:		LineToMode
* Description:	从当前点至(x1,y1)画线，mode可选择与目标位置与，或，异或
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void LineToMode(S16 x1, S16 y1, U8 mode)
{
#if 1
 S16 t,distance;
 S16 x=0,y=0,dx,dy;
 S16 x0,y0;
 S16 cx,cy;

 x0 = CurX;
 y0 = CurY;
 //y0 = DISPLAY_AREA_HEIGHT - 1 - y0;
 y1 = DISPLAY_AREA_HEIGHT - 1 - y1;

 CurX = x1;
 CurY = y1;
 dx=x1-x0;
 dy=y1-y0;
 //判断两个横坐标的大小
 if(dx>0)
 {
  cx=1;
 }
 else if(dx==0)//两个横坐标相等则输出横的直线
 {
  DrawLineCol(x0,y0,y1,mode);
  return;
 }
 else 
 {
  cx=-1;
 }
//判断两个做坐标的大小
if(dy>0)
 {
  cy=1;
 }
 else if(dy==0)
 {
  DrawLineRow(x0,x1,y0,mode);
  return;
 }
 else 
 {
  cy=-1;
 }
 //对dx和dy取绝对值，使其为正整数
 if(dx<0)
 dx=-dx;
 if(dy<0)
 dy=-dy;
 //判断两个大小，把大的给变量distance
if(dx>dy)   
{
 distance=dx;
}
else
{
 distance=dy;
}
DrawDotMode(x0,y0,mode);//画出第一个点
//画线
for(t=0;t<=distance+1;t++)
{
 DrawDotMode(x0,y0,mode);//显示在那个点
 x=x+dx;
 y=y+dy;
 if(x>distance)
 {
  x=x-distance;
  x0=x0+cx;
 }
 if(y>distance)
 {
  y=y-distance;
  y0=y0+cy;
 }
}

#else
	S16 a;
	S16 b;
	S16 d1;
	S16 d2;
	S16 d;
	S16 x;
	S16 y;
	
	y1 = DISPLAY_AREA_HEIGHT - 1 - y1;
	if ((x1 == CurX) && (y1 == CurY))
	{
		DrawDotMode(x1, y1, mode);
		return;
	}

	a = CurY - y1;
	b = x1 - CurX;
	d = 2*a + b;
	d1 = 2*a;
	d2 = 2*(a+b);
	x = CurX;
	y = CurY;

	DrawDotMode(x, y, mode);

	while (x < x1)
	{
		if (d < 0)
		{
			x ++;
			y ++;
			d += d2;
		}
		else
		{
			x ++;
			d += d1;
		}
		DrawDotMode(x, y, mode);
	}
#endif	
}

/****************************************************************
* Function:		LineTo
* Description:	从当前点至(x1,y1)画线
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void LineTo(S16 x1, S16 y1)
{

#if 1
 S16 t,distance;
 S16 x=0,y=0,dx,dy;
 S16 x0,y0;
 S16 cx,cy;

 x0 = CurX;
 y0 = CurY;
// y0 = DISPLAY_AREA_HEIGHT - 1 - y0;
 y1 = DISPLAY_AREA_HEIGHT - 1 - y1;

 CurX = x1;
  CurY = y1;

 dx=x1-x0;
 dy=y1-y0;
 //判断两个横坐标的大小
 if(dx>0)
 {
  cx=1;
 }
 else if(dx==0)//两个横坐标相等则输出横的直线
 {
  DrawLineCol(x0,y0,y1,DRAW_MODE_OR);
  return;
 }
 else 
 {
  cx=-1;
 }
//判断两个做坐标的大小
if(dy>0)
 {
  cy=1;
 }
 else if(dy==0)
 {
  DrawLineRow(x0,x1,y0,DRAW_MODE_OR);
  return;
 }
 else 
 {
  cy=-1;
 }
 //对dx和dy取绝对值，使其为正整数
 if(dx<0)
 dx=-dx;
 if(dy<0)
 dy=-dy;
 //判断两个大小，把大的给变量distance
if(dx>dy)   
{
 distance=dx;
}
else
{
 distance=dy;
}
DrawDot(x0,y0);//画出第一个点
//画线
for(t=0;t<=distance+1;t++)
{
 DrawDot(x0,y0);//显示在那个点
 x=x+dx;
 y=y+dy;
 if(x>distance)
 {
  x=x-distance;
  x0=x0+cx;
 }
 if(y>distance)
 {
  y=y-distance;
  y0=y0+cy;
 }
}

#else
	S16 a;
	S16 b;
	S16 d1;
	S16 d2;
	S16 d;
	S16 x;
	S16 y;
	
	y1 = DISPLAY_AREA_HEIGHT - 1 - y1;
	if ((x1 == CurX) && (y1 == CurY))
	{
		DrawDot(x1,y1);
		return;
	}

	a = CurY - y1;
	b = x1 - CurX;
	d = 2*a + b;
	d1 = 2*a;
	d2 = 2*(a+b);
	x = CurX;
	y = CurY;

	DrawDot(x,y);

	while (x < x1)
	{
		if (d < 0)
		{
			x ++;
			y ++;
			d += d2;
		}
		else
		{
			x ++;
			d += d1;
		}
		DrawDot(x,y);
	}
#endif
}



/****************************************************************
* Function:		DrawLine
* Description:	画线，从(x0,y0)到(x1,y1),包括起始点
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawLine(S16 x0, S16 y0, S16 x1, S16 y1)
{
#if 1
 S16 t,distance;
 S16 x=0,y=0,dx,dy;
 S16 cx,cy;

 y0 = DISPLAY_AREA_HEIGHT - 1 - y0;
 y1 = DISPLAY_AREA_HEIGHT - 1 - y1;
 
 dx=x1-x0;
 dy=y1-y0;
 //判断两个横坐标的大小
 if(dx>0)
 {
  cx=1;
 }
 else if(dx==0)//两个横坐标相等则输出横的直线
 {
  DrawLineCol(x0,y0,y1,DRAW_MODE_OR);
  return;
 }
 else 
 {
  cx=-1;
 }
//判断两个做坐标的大小
if(dy>0)
 {
  cy=1;
 }
 else if(dy==0)
 {
  DrawLineRow(x0,x1,y0,DRAW_MODE_OR);
  return;
 }
 else 
 {
  cy=-1;
 }
 //对dx和dy取绝对值，使其为正整数
 if(dx<0)
 dx=-dx;
 if(dy<0)
 dy=-dy;
 //判断两个大小，把大的给变量distance
if(dx>dy)   
{
 distance=dx;
}
else
{
 distance=dy;
}
DrawDot(x0,y0);//画出第一个点
//画线
for(t=0;t<=distance+1;t++)
{
 DrawDot(x0,y0);//显示在那个点
 x=x+dx;
 y=y+dy;
 if(x>distance)
 {
  x=x-distance;
  x0=x0+cx;
 }
 if(y>distance)
 {
  y=y-distance;
  y0=y0+cy;
 }
}

#else
	S16 a;
	S16 b;
	S16 d1;
	S16 d2;
	S16 d;
	S16 x;
	S16 y;

	y0 = DISPLAY_AREA_HEIGHT - 1 - y0;
	y1 = DISPLAY_AREA_HEIGHT - 1 - y1;
	if ((x1 == x0) && (y1 == y0))
	{
		DrawDot(x1,y1);
		return;
	}

	a = y0 - y1;
	b = x1 - x0;
	d = 2*a + b;
	d1 = 2*a;
	d2 = 2*(a+b);
	x = x0;
	y = y0;

	DrawDot(x,y);

	while (x < x1)
	{
		if (d < 0)
		{
			x ++;
			y ++;
			d += d2;
		}
		else
		{
			x ++;
			d += d1;
		}
		DrawDot(x,y);
	}
#endif
}


/****************************************************************
* Function:		DrawFont
* Description:	绘制字体到LCD
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawFont(BYTE x, BYTE y, BYTE val)
{
	BYTE i;
	BYTE *pData;
	BYTE y1;

	x = x>>3;
	y1 = y;
	pData = (BYTE *)(&abyFontDigAlph[val][0]);
	for (i = 0; i < 16; i++)
	{
		LcdData[y1][x] = *pData;
		y1++;
		pData++;
	}
}

/****************************************************************
* Function:		DrawChineseFont
* Description:	绘制中文字体到LCD
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawChineseFont(BYTE x, BYTE y, BYTE val)
{
	BYTE i;
	BYTE *pData;
	BYTE x1;
	BYTE y1;

	x = x>>3;
	x1 = x + 1;
	y1 = y;
	pData = (BYTE *)(&FontChinese[val][0]);
	for (i = 0; i < 16; i++)
	{
		LcdData[y1][x] = *pData;
		pData++;
		LcdData[y1][x1] = *pData;
		pData++;
		y1++;
		
	}
}

/****************************************************************
* Function:		DrawFontL2R8x16
* Description:	绘制从左到右的8*16点阵字体
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawFontL2R8x16(BYTE x, BYTE y, BYTE val)
{
	BYTE i;
	BYTE *pData;
	BYTE x1;
	BYTE y1;
	WORD wdTmp;
	BYTE byOffset;

	x1 = x>>3;

	if (x1 >= (LCD_DATA_X_MAX>>1))
	{
		return;
	}
	
	byOffset = x&0x07;
	y1 = y;
	pData = (BYTE *)(&abyFontDigAlph[val][0]);
	for (i = 0; i < 16; i++)
	{
		if (byOffset && ((x1+1) < (LCD_DATA_X_MAX>>1)))
		{
			*((BYTE*)(&wdTmp) + 1) = *pData;
			*((BYTE*)(&wdTmp)) = 0;
			wdTmp = wdTmp >> byOffset;
			LcdData[y1][x1] |= *((BYTE*)(&wdTmp)+1);
			LcdData[y1][x1+1] |= *((BYTE*)(&wdTmp));
		}
		else
		{
			LcdData[y1][x1] |= *pData;
			
			
		}
		y1++;
		pData++;
		
	}
}
/****************************************************************
* Function:		DrawFontL2R8x16Not
* Description:	绘制从左到右的8*16点阵字体,反显
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawFontL2R8x16Not(BYTE x, BYTE y, BYTE val)
{
	BYTE i;
	BYTE *pData;
	BYTE x1;
	BYTE y1;
	WORD wdTmp;
	BYTE byOffset;

	x1 = x>>3;

	if (x1 >= (LCD_DATA_X_MAX>>1))
	{
		return;
	}
	
	byOffset = x&0x07;
	y1 = y;
	pData = (BYTE *)(&abyFontDigAlph[val][0]);
	for (i = 0; i < 16; i++)
	{
		if (byOffset && ((x1+1) < (LCD_DATA_X_MAX>>1)))
		{
			*((BYTE*)(&wdTmp) + 1) = ~(*pData);
			*((BYTE*)(&wdTmp)) = 0;
			wdTmp = wdTmp >> byOffset;
			LcdData[y1][x1] |= *((BYTE*)(&wdTmp)+1);
			LcdData[y1][x1+1] |= *((BYTE*)(&wdTmp));
		}
		else
		{
			LcdData[y1][x1] |= ~(*pData);
			
			
		}
		y1++;
		pData++;
		
	}
}

/****************************************************************
* Function:		DrawFontL2R5x7
* Description:	绘制从左到右的5*7点阵字体
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawFontL2R5x7(BYTE x, BYTE y, BYTE val)
{
	BYTE i;
	BYTE *pData;
	BYTE x1;
	BYTE y1;
	WORD wdTmp;
	BYTE byOffset;

	x1 = x>>3;

	if (x1 >= (LCD_DATA_X_MAX>>1))
	{
		return;
	}
	
	byOffset = x&0x07;
	y1 = y;
	pData = (BYTE *)(&abyFontL2R5x7[val][0]);
	for (i = 0; i < 7; i++)
	{

		if (byOffset)
		{
			if ((byOffset <= 2) || ((x1+1) >= (LCD_DATA_X_MAX>>1)))
			{
				LcdData[y1][x1] |= (*pData)>>byOffset;
			}
			else
			{
				*((BYTE*)(&wdTmp) + 1) = *pData;
				*((BYTE*)(&wdTmp)) = 0;
				wdTmp = wdTmp >> byOffset;
				LcdData[y1][x1] |= *((BYTE*)(&wdTmp)+1);
				LcdData[y1][x1+1] |= *((BYTE*)(&wdTmp));
			}
		}
		else
		{
			LcdData[y1][x1] |= *pData;
		}
		
		y1++;
		pData++;
	}
}
/****************************************************************
* Function:		DrawFontT2B5x7
* Description:	绘制从上到下的5*7点阵字体
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawFontT2B5x7(BYTE x, BYTE y, BYTE val)
{
	BYTE i;
	BYTE *pData;
	BYTE x1;
	BYTE y1;
	WORD wdTmp;
	BYTE byOffset;

	x1 = x>>3;

	if (x1 >= (LCD_DATA_X_MAX>>1))
	{
		return;
	}
	
	byOffset = x&0x07;
	y1 = y;
	pData = (BYTE *)(&abyFontT2B5x7[val][0]);
	for (i = 0; i < 5; i++)
	{

		if (byOffset)
		{
			if ((byOffset <= 1) || ((x1+1) >= (LCD_DATA_X_MAX>>1)))
			{
				LcdData[y1][x1] |= (*pData)>>byOffset;
			}
			else
			{
				*((BYTE*)(&wdTmp) + 1) = *pData;
				*((BYTE*)(&wdTmp)) = 0;
				wdTmp = wdTmp >> byOffset;
				LcdData[y1][x1] |= *((BYTE*)(&wdTmp)+1);
				LcdData[y1][x1+1] |= *((BYTE*)(&wdTmp));
				
			}
			
			
		}
		else
		{
			LcdData[y1][x1] |= *pData;
			
		}
		y1++;
		pData++;
	}
}
/****************************************************************
* Function:		DrawFontB2T5x7
* Description:	绘制从下到上的5*7点阵字体
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawFontB2T5x7(BYTE x, BYTE y, BYTE val)
{
	BYTE i;
	BYTE *pData;
	BYTE x1;
	BYTE y1;
	WORD wdTmp;
	BYTE byOffset;

	x1 = x>>3;

	if (x1 >= (LCD_DATA_X_MAX>>1))
	{
		return;
	}
	
	byOffset = x&0x07;
	y1 = y;
	pData = (BYTE *)(&abyFontB2T5x7[val][4]);
	for (i = 0; i < 5; i++)
	{

		if (byOffset)
		{
			if ((byOffset <= 1) || ((x1+1) >= (LCD_DATA_X_MAX>>1)))
			{
				LcdData[y1][x1] |= (*pData)>>byOffset;
			}
			else
			{
				*((BYTE*)(&wdTmp) + 1) = *pData;
				*((BYTE*)(&wdTmp)) = 0;
				wdTmp = wdTmp >> byOffset;
				LcdData[y1][x1] |= *((BYTE*)(&wdTmp)+1);
				LcdData[y1][x1+1] |= *((BYTE*)(&wdTmp));
				
			}
		}
		else
		{
			LcdData[y1][x1] |= *pData;
			
		}
		y1++;
		pData--;
		
	}
}

/****************************************************************
* Function:		GetAlphDigIndex
* Description:	将字符串中的字符ANSII码转为当前字模点阵数组中的索引值
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
BYTE GetAlphDigIndex(CHAR Val)
{
	if (Val >= '0'  && Val <= '9')
	{
		return Val - '0' + DIG_0;
	}

	if (Val >= 'a'  && Val <= 'z')
	{
		return Val - 'a' + ALPH_a;
	}

	if (Val >= 'A'  && Val <= 'Z')
	{
		return Val - 'A' + ALPH_A;
	}

	if (Val == '_')
	{
		return SYM_DASH;
	}
	
	if (Val == '-')
	{
		return SYM_MINUS;
	}
	

	return SPACE;
}



/****************************************************************
* Function:		DrawCoordinate
* Description:	绘制座标轴
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawCoordinate(void)
{
	U8 i;
	U8 tmp;
	U16 wdTmp;
	U32 dwdTmp;
	
	// draw t axis
	MoveTo(DISPLAY_U_CO_OFFSET, DISPLAY_T_CO_OFFSET);
	LineTo(DISPLAY_AREA_WIDTH-1, DISPLAY_T_CO_OFFSET);
	LineTo(DISPLAY_AREA_WIDTH-1 - 3, DISPLAY_T_CO_OFFSET - 3);
	MoveTo(DISPLAY_AREA_WIDTH-1, DISPLAY_T_CO_OFFSET);
	LineTo(DISPLAY_AREA_WIDTH-1 - 3, DISPLAY_T_CO_OFFSET + 3);
	for (i = DISPLAY_U_CO_OFFSET + 20; i < DISPLAY_I_CO_OFFSET; i += 20)
	{
		DrawDotEx(i,DISPLAY_T_CO_OFFSET + 1);
		if (!(i % 40))
		{
			DrawDotEx(i,DISPLAY_T_CO_OFFSET + 2);
		}
		
	}
	DrawFontL2R5x7(DISPLAY_U_CO_OFFSET, DISPLAY_AREA_HEIGHT - 7, S5x7_DIG_0);
	DrawFontL2R5x7(DISPLAY_I_CO_OFFSET - 2, DISPLAY_AREA_HEIGHT - 7, S5x7_ALPH_M);


	if (MID_CFG_SEL != byMID)
	{
		dwdTmp = UnitT;
		dwdTmp *= (DISPLAY_I_CO_OFFSET - DISPLAY_U_CO_OFFSET) 
			- (DISPLAY_I_CO_OFFSET - DISPLAY_U_CO_OFFSET)%20;
		dwdTmp *= SAVE_UIT_TM_SPACE/2;
		dwdTmp /= 60;

		i = DISPLAY_I_CO_OFFSET - 2 - 5;
		do
		{
			DrawFontL2R5x7(i, DISPLAY_AREA_HEIGHT - 7, S5x7_DIG_0 + dwdTmp%10);
			dwdTmp = dwdTmp/10;
			i -= 5;
		} 
		while (dwdTmp != 0);
	}
	
	
	// draw U axis
	MoveTo(DISPLAY_U_CO_OFFSET, DISPLAY_T_CO_OFFSET);
	LineTo(DISPLAY_U_CO_OFFSET, DISPLAY_AREA_HEIGHT-1);
	LineTo(DISPLAY_U_CO_OFFSET+3, DISPLAY_AREA_HEIGHT-3-1);
	MoveTo(DISPLAY_U_CO_OFFSET, DISPLAY_AREA_HEIGHT-1);
	LineTo(DISPLAY_U_CO_OFFSET-3, DISPLAY_AREA_HEIGHT-3-1);
	for (i = 0, tmp = DISPLAY_T_CO_OFFSET/* + wdCoUMin*/; tmp < DISPLAY_AREA_HEIGHT; tmp += wdCoUMarkSpace, i++)
	{
		//if (tmp < DISPLAY_T_CO_OFFSET)
		//{
		//	continue;
		//}
		
		DrawDotEx(DISPLAY_U_CO_OFFSET + 1, tmp);
		if (!(i % 2))
		{
			DrawDotEx(DISPLAY_U_CO_OFFSET + 2, tmp);
		}
		
	}
	DrawFontT2B5x7(0, 4, S5x7_ALPH_V);
	//dwdTmp = wdCoUMin;
	//dwdTmp *= UnitU;
	if (MID_CFG_SEL != byMID)
	{

		dwdTmp = CurveUMin;

		for (i = DISPLAY_AREA_HEIGHT - DISPLAY_T_CO_OFFSET - 5, tmp = 0;
			(tmp <= 2) || (dwdTmp != 0); tmp++,dwdTmp = dwdTmp/10,i -= 5)
		{
			DrawFontT2B5x7(0, i, S5x7_DIG_0 + dwdTmp%10);
			if (1 == tmp)
			{
				i -= 5;
				DrawFontT2B5x7(0, i, S5x7_SYM_PERIOD);
			}
		} 

		//dwdTmp = wdCoUMax;
		//dwdTmp *= UnitU;

		//dwdTmp = CurveUMax;
		dwdTmp = DISPLAY_AREA_HEIGHT - DISPLAY_T_CO_OFFSET - 1;
		dwdTmp *= UnitU;
		dwdTmp += CurveUMin;
		
		for (i = 10, wdTmp = 10000; wdTmp != 0; wdTmp /= 10)
		{
			if ((dwdTmp >= wdTmp) || (wdTmp <= 100))
			{
				DrawFontT2B5x7(0, i, S5x7_DIG_0 + dwdTmp/wdTmp%10);

				if (100 == wdTmp)
				{
					i += 5;
					DrawFontT2B5x7(0, i, S5x7_SYM_PERIOD);
				}
				i += 5;
			}
		} 
	}
		
	// draw I axis
	MoveTo(DISPLAY_I_CO_OFFSET, DISPLAY_T_CO_OFFSET);
	LineTo(DISPLAY_I_CO_OFFSET, DISPLAY_AREA_HEIGHT-1);
	LineTo(DISPLAY_I_CO_OFFSET+3, DISPLAY_AREA_HEIGHT-3-1);
	MoveTo(DISPLAY_I_CO_OFFSET, DISPLAY_AREA_HEIGHT-1);
	LineTo(DISPLAY_I_CO_OFFSET-3, DISPLAY_AREA_HEIGHT-3-1);
	for (i = 0, tmp = DISPLAY_T_CO_OFFSET/* + wdCoIMin */; tmp < DISPLAY_AREA_HEIGHT; tmp += wdCoIMarkSpace, i++)
	{
		//if (tmp < DISPLAY_T_CO_OFFSET)
		//{
		//	continue;
		//}
		
		DrawDotEx(DISPLAY_I_CO_OFFSET - 1, tmp);
		if (!(i % 2))
		{
			DrawDotEx(DISPLAY_I_CO_OFFSET - 2, tmp);
		}
		
	}
	DrawFontB2T5x7(DISPLAY_AREA_WIDTH - 7, 4, S5x7_ALPH_A);

	//dwdTmp = wdCoIMax;
	//dwdTmp *= UnitI;
	
	//dwdTmp = CurveIMax;
	if (MID_CFG_SEL != byMID)
	{
		dwdTmp = DISPLAY_AREA_HEIGHT - DISPLAY_T_CO_OFFSET - 1;
		dwdTmp *= UnitI;
		dwdTmp += CurveIMin;
		for (i = 10, tmp = 0;
			(tmp <= 2) || (dwdTmp != 0); tmp++,dwdTmp = dwdTmp/10,i += 5)
		{
			DrawFontB2T5x7(DISPLAY_AREA_WIDTH - 7, i, S5x7_DIG_0 + dwdTmp%10);
			if (1 == tmp)
			{
				i += 5;
				DrawFontB2T5x7(DISPLAY_AREA_WIDTH - 7, i, S5x7_SYM_PERIOD);
			}
		} 

		//dwdTmp = wdCoIMin;
		//dwdTmp *= UnitI;
		dwdTmp = CurveIMin;
		for (i = DISPLAY_AREA_HEIGHT - DISPLAY_T_CO_OFFSET - 8, wdTmp = 10000; wdTmp != 0; wdTmp /= 10)
		{
			if ((dwdTmp >= wdTmp) || (wdTmp <= 100))
			{
				DrawFontB2T5x7(DISPLAY_AREA_WIDTH - 7, i, S5x7_DIG_0 + dwdTmp/wdTmp%10);

				if (100 == wdTmp)
				{
					i -= 5;
					DrawFontB2T5x7(DISPLAY_AREA_WIDTH - 7, i, S5x7_SYM_PERIOD);
				}
				i -= 5;
			}
		} 
	}
	
}

/****************************************************************
* Function:		DrawCurve
* Description:	刷新曲线显示区的内容
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawCurve(BYTE mode)
{
	BYTE i;
	DWORD tmp;
	BYTE CurveSel;
	BYTE *pUData = NULL;
	BYTE *pIData = NULL;

	if (((MID_TOP == byMID) && (CTRL_STATE_CHARGE == byState)) || (MID_SHOW_CHG_DATA== byMID))
	{
		if (MID_SHOW_CHG_DATA== byMID)
		{
			wdDispCurUITFileInex = wdML2ChgOffset + 1;
			iDispCurErr = DBGetUIFileSize(wdDispCurUITFileInex,
				NULL,
				&tmp);
			if (!iDispCurErr)
			{
				dwdDispUITCnt = tmp;
			}
		}
		else
		{
			dwdDispUITCnt = dwdUITCnt;
			wdDispCurUITFileInex = wdCurUITFileInex;
			iDispCurErr = 0;
		}
		

		if (iDispCurErr)
		{
			
		}
		else
		{
			UnitT = 1;
			tmp = DISPLAY_CURVE_WIDTH;
			while (dwdDispUITCnt >= tmp)
			{
				tmp += DISPLAY_CURVE_WIDTH;
				UnitT++;
			}

			GetDispCurveValue();
			MoveTo(DISPLAY_U_I_CO_OFFSET, CurveUBuf[0] + DISPLAY_T_CO_OFFSET);
			for ( i = 1; i < byDispNum ; i++)
			{
				LineToMode(i + DISPLAY_U_I_CO_OFFSET, CurveUBuf[i] + DISPLAY_T_CO_OFFSET, mode);
			}
			i = byDispNum - 1;
			
			DrawDotModeEx(i + DISPLAY_U_I_CO_OFFSET, CurveUBuf[i] + DISPLAY_T_CO_OFFSET, DRAW_MODE_XOR);
			DrawDotModeEx(i + DISPLAY_U_I_CO_OFFSET+1, CurveUBuf[i] + DISPLAY_T_CO_OFFSET, DRAW_MODE_OR);
			DrawDotModeEx(i + DISPLAY_U_I_CO_OFFSET-1, CurveUBuf[i] + DISPLAY_T_CO_OFFSET, DRAW_MODE_OR);
			DrawDotModeEx(i + DISPLAY_U_I_CO_OFFSET, CurveUBuf[i] + DISPLAY_T_CO_OFFSET+1, DRAW_MODE_OR);
			DrawDotModeEx(i + DISPLAY_U_I_CO_OFFSET, CurveUBuf[i] + DISPLAY_T_CO_OFFSET-1, DRAW_MODE_OR);

			MoveTo(DISPLAY_U_I_CO_OFFSET, CurveIBuf[0] + DISPLAY_T_CO_OFFSET);
			for ( i = 1; i < byDispNum; i++)
			{
				LineToMode(i + DISPLAY_U_I_CO_OFFSET, CurveIBuf[i] + DISPLAY_T_CO_OFFSET, mode);
			}
			i = byDispNum - 1;
		}
		
	}
	else if (MID_CFG_SEL == byMID)
	{
		if (iDispCurErr)
		{
			
		}
		else
		{
			CurveSel = 0;
			if (stChargeCfgData.T3)
			{
				CurveSel |= 0x01;
			}

			if (stChargeCfgData.T5)
			{
				CurveSel |= 0x02;
			}

			pUData = (U8*)&abyCfgCurveUBuf[CurveSel][0];
			pIData = (U8*)&abyCfgCurveIBuf[CurveSel][0];

			MoveTo(DISPLAY_U_I_CO_OFFSET + 2, pUData[0] + DISPLAY_T_CO_OFFSET + 2);
			for ( i = 1; i < 100 ; i++)
			{
				LineToMode(i + DISPLAY_U_I_CO_OFFSET + 2, pUData[i] + DISPLAY_T_CO_OFFSET + 2, mode);
			}
			i = 100 - 1;
			
			DrawDotModeEx(2 + i + DISPLAY_U_I_CO_OFFSET, 2 + pUData[i] + DISPLAY_T_CO_OFFSET, DRAW_MODE_XOR);
			DrawDotModeEx(2 + i + DISPLAY_U_I_CO_OFFSET+1, 2 + pUData[i] + DISPLAY_T_CO_OFFSET, DRAW_MODE_OR);
			DrawDotModeEx(2 + i + DISPLAY_U_I_CO_OFFSET-1, 2 + pUData[i] + DISPLAY_T_CO_OFFSET, DRAW_MODE_OR);
			DrawDotModeEx(2 + i + DISPLAY_U_I_CO_OFFSET, 2 + pUData[i] + DISPLAY_T_CO_OFFSET+1, DRAW_MODE_OR);
			DrawDotModeEx(2 + i + DISPLAY_U_I_CO_OFFSET, 2 + pUData[i] + DISPLAY_T_CO_OFFSET-1, DRAW_MODE_OR);

			MoveTo(2 + DISPLAY_U_I_CO_OFFSET, 2 + pIData[0] + DISPLAY_T_CO_OFFSET);
			for ( i = 1; i < 100; i++)
			{
				LineToMode(2 + i + DISPLAY_U_I_CO_OFFSET, 2 + pIData[i] + DISPLAY_T_CO_OFFSET, mode);
			}
			
		}
	}

}


/****************************************************************
* Function:		DrawMenu
* Description:	刷新菜单区域的内容
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void DrawMenu(void)
{
	BYTE second;
	BYTE minute;
	WORD hour;
	static DWORD time = 0;
	DWORD tmp;
	WORD wdTmp;
	BYTE x;
	BYTE y;

	if (MID_TOP == byMID)
	{
		DrawFontL2R8x16(0,0,ALPH_M);
		DrawFontL2R8x16(8,0,ALPH_o);
		DrawFontL2R8x16(8*2,0,ALPH_d);
		DrawFontL2R8x16(8*3,0,ALPH_e);
		DrawFontL2R8x16(8*4,0,ALPH_l);
		DrawFontL2R8x16(8*5-2,0,SYM_COLON);
		DrawFontL2R8x16(8*6-4,0,ALPH_P);
		DrawFontL2R8x16(8*7-4,0,ALPH_R);
		DrawFontL2R8x16(8*8-4,0,ALPH_O);
		DrawFontL2R8x16(8*9-4,0,SPACE);
		DrawFontL2R8x16(8*10-4,0,DIG_7);
		DrawFontL2R8x16(8*11-4,0,DIG_5);
		DrawFontL2R8x16(8*12-4,0,DIG_0);
		//DrawFontL2R8x16(8*13-4,0,ALPH_X);
		//DrawFontL2R8x16(8*14-4,0,ALPH_X);

		DrawFontL2R8x16(0,16,ALPH_B);
		DrawFontL2R8x16(8,16,ALPH_a);
		DrawFontL2R8x16(8*2,16,ALPH_t);
		DrawFontL2R8x16(8*3,16,ALPH_t);
		DrawFontL2R8x16(8*4,16,ALPH_e);
		DrawFontL2R8x16(8*5,16,ALPH_r);
		DrawFontL2R8x16(8*6,16,ALPH_y);
		DrawFontL2R8x16(8*7-2,16,SYM_COLON);
		switch (byVoltType)
		{
			case 0:
				DrawFontL2R8x16(8*8-4,16,DIG_1);
				DrawFontL2R8x16(8*9-4,16,DIG_2);
				break;
			case 1:
				DrawFontL2R8x16(8*8-4,16,DIG_2);
				DrawFontL2R8x16(8*9-4,16,DIG_4);
				break;
			case 2:
				DrawFontL2R8x16(8*8-4,16,DIG_3);
				DrawFontL2R8x16(8*9-4,16,DIG_6);
				break;
			case 3:
				DrawFontL2R8x16(8*8-4,16,DIG_4);
				DrawFontL2R8x16(8*9-4,16,DIG_8);
				break;
		}
		
		DrawFontL2R8x16(8*10-4,16,ALPH_V);

		
		//DrawFontL2R8x16(8*11-4,0,ALPH_X);
		switch (byCurType)
		{
			case 0:
				//DrawFontL2R8x16(8*12-4,16,DIG_1);
				DrawFontL2R8x16(8*13-4,16,DIG_5);
				break;
			case 1:
				DrawFontL2R8x16(8*12-4,16,DIG_1);
				DrawFontL2R8x16(8*13-4,16,DIG_0);
				break;
			case 2:
				DrawFontL2R8x16(8*12-4,16,DIG_1);
				DrawFontL2R8x16(8*13-4,16,DIG_5);
				break;
			case 3:
				DrawFontL2R8x16(8*12-4,16,DIG_2);
				DrawFontL2R8x16(8*13-4,16,DIG_0);
				break;
			case 4:
				DrawFontL2R8x16(8*12-4,16,DIG_2);
				DrawFontL2R8x16(8*13-4,16,DIG_5);
				break;
			case 5:
				DrawFontL2R8x16(8*12-4,16,DIG_3);
				DrawFontL2R8x16(8*13-4,16,DIG_0);
				break;
		}
		DrawFontL2R8x16(8*14-4,16,ALPH_A);

		

		DrawFontL2R8x16(0,32,ALPH_C);
		DrawFontL2R8x16(8,32,ALPH_h);
		DrawFontL2R8x16(8*2,32,ALPH_g);
		DrawFontL2R8x16(8*3,32,ALPH_C);
		DrawFontL2R8x16(8*4,32,ALPH_f);
		DrawFontL2R8x16(8*5,32,ALPH_g);
		DrawFontL2R8x16(8*6-2,32,SYM_COLON);

		for (tmp = 0; tmp < 8; tmp++)
		{
			if (!stChargeCfgData.acName[tmp])
			{
				break;
			}
			DrawFontL2R8x16(8*(7+tmp)-4,32,GetAlphDigIndex(stChargeCfgData.acName[tmp]));
		}

		DrawFontL2R8x16(0,48,ALPH_S);
		DrawFontL2R8x16(8,48,ALPH_t);
		DrawFontL2R8x16(8*2,48,ALPH_a);
		DrawFontL2R8x16(8*3,48,ALPH_t);
		DrawFontL2R8x16(8*4,48,ALPH_u);
		DrawFontL2R8x16(8*5,48,ALPH_s);
		DrawFontL2R8x16(8*6-2,48,SYM_COLON);

		switch (byState)
		{
			case CTRL_STATE_IDLE:
				DrawFontL2R8x16(8*9-4,48,ALPH_S);
				DrawFontL2R8x16(8*10-4,48,ALPH_t);
				DrawFontL2R8x16(8*11-4,48,ALPH_o);
				DrawFontL2R8x16(8*12-4,48,ALPH_p);
				break;
			case CTRL_STATE_STAND_BY:
				DrawFontL2R8x16(8*7-4,48,ALPH_S);
				DrawFontL2R8x16(8*8-4,48,ALPH_t);
				DrawFontL2R8x16(8*9-4,48,ALPH_a);
				DrawFontL2R8x16(8*10-4,48,ALPH_n);
				DrawFontL2R8x16(8*11-4,48,ALPH_d);
				//DrawFontL2R8x16(8*12-4,48,ALPH_X);
				DrawFontL2R8x16(8*13-4,48,ALPH_b);
				DrawFontL2R8x16(8*14-4,48,ALPH_y);
				break;
			case CTRL_STATE_SELF_TEST:
				DrawFontL2R8x16(8*7-4,48,ALPH_S);
				DrawFontL2R8x16(8*8-4,48,ALPH_e);
				DrawFontL2R8x16(8*9-4,48,ALPH_l);
				DrawFontL2R8x16(8*10-4,48,ALPH_f);
				DrawFontL2R8x16(8*11-4,48,ALPH_t);
				DrawFontL2R8x16(8*12-4,48,ALPH_e);
				DrawFontL2R8x16(8*13-4,48,ALPH_s);
				DrawFontL2R8x16(8*14-4,48,ALPH_t);
				break;
			case CTRL_STATE_TEST:
				DrawFontL2R8x16(8*9-4,48,ALPH_T);
				DrawFontL2R8x16(8*10-4,48,ALPH_e);
				DrawFontL2R8x16(8*11-4,48,ALPH_s);
				DrawFontL2R8x16(8*12-4,48,ALPH_t);
				break;
			case CTRL_STATE_CHARGE:
				switch (byChgStep)
				{
					case CHARGE_STEP_PREV:
						DrawFontL2R8x16(8*9-4,48,ALPH_P);
						DrawFontL2R8x16(8*10-4,48,ALPH_r);
						DrawFontL2R8x16(8*11-4,48,ALPH_e);
						DrawFontL2R8x16(8*12-4,48,ALPH_v);
						break;
					case CHARGE_STEP_CC1:
						DrawFontL2R8x16(8*10-4,48,ALPH_C);
						DrawFontL2R8x16(8*11-4,48,ALPH_C);
						DrawFontL2R8x16(8*12-4,48,DIG_1);
						break;
					case CHARGE_STEP_CC2:
						DrawFontL2R8x16(8*10-4,48,ALPH_C);
						DrawFontL2R8x16(8*11-4,48,ALPH_C);
						DrawFontL2R8x16(8*12-4,48,DIG_2);
						break;
					case CHARGE_STEP_CV:
						DrawFontL2R8x16(8*10-4,48,ALPH_C);
						DrawFontL2R8x16(8*11-4,48,ALPH_V);
						break;
					case CHARGE_STEP_ABSO:
						DrawFontL2R8x16(8*9-4,48,ALPH_A);
						DrawFontL2R8x16(8*10-4,48,ALPH_b);
						DrawFontL2R8x16(8*11-4,48,ALPH_s);
						DrawFontL2R8x16(8*12-4,48,ALPH_o);
						break;
					case CHARGE_STEP_FLOAT:
						DrawFontL2R8x16(8*8-4,48,ALPH_F);
						DrawFontL2R8x16(8*9-4,48,ALPH_l);
						DrawFontL2R8x16(8*10-4,48,ALPH_o);
						DrawFontL2R8x16(8*11-4,48,ALPH_a);
						DrawFontL2R8x16(8*12-4,48,ALPH_t);
						break;
					default:
						DrawFontL2R8x16(8*10-4,48,SYM_PERIOD);
						DrawFontL2R8x16(8*11-4,48,SYM_PERIOD);
						DrawFontL2R8x16(8*12-4,48,SYM_PERIOD);
						break;
				}
				break;
			case CTRL_STATE_ERROR:
				DrawFontL2R8x16(8*7-4,48,ALPH_E);
				DrawFontL2R8x16(8*8-4,48,ALPH_r);
				DrawFontL2R8x16(8*9-4,48,ALPH_r);
				DrawFontL2R8x16(8*10-4,48,ALPH_o);
				DrawFontL2R8x16(8*11-4,48,ALPH_r);
				//DrawFontL2R8x16(8*12-4,48,ALPH_X);
				switch (byErr)
				{
					case ERROR_SELF_TEST:
						
						DrawFontL2R8x16(8*13-4,48,ALPH_S);
						DrawFontL2R8x16(8*14-4,48,ALPH_T);
						break;
					case ERROR_TEMP:
						DrawFontL2R8x16(8*13-4,48,ALPH_T);
						DrawFontL2R8x16(8*14-4,48,ALPH_P);
						break;
					case ERROR_BT_LOW:
						DrawFontL2R8x16(8*13-4,48,ALPH_B);
						DrawFontL2R8x16(8*14-4,48,ALPH_L);
						break;
					case ERROR_BT_REV:
						DrawFontL2R8x16(8*13-4,48,ALPH_B);
						DrawFontL2R8x16(8*14-4,48,ALPH_R);
						break;
					case ERROR_CFG:
						DrawFontL2R8x16(8*13-4,48,ALPH_C);
						DrawFontL2R8x16(8*14-4,48,ALPH_F);
						break;
					case ERROR_SAVE_DATA:
						DrawFontL2R8x16(8*13-4,48,ALPH_S);
						DrawFontL2R8x16(8*14-4,48,ALPH_D);
						break;
				}
				break;
		}

		
		DrawFontL2R8x16(0,64,ALPH_U);
		DrawFontL2R8x16(8-2,64,SYM_COLON);

		if ((byState == CTRL_STATE_CHARGE) || (CTRL_STATE_TEST == byState))
		{
			wdTmp = wdBtVoltage;
			if (wdTmp%10 >= 5)
			{
				wdTmp++; 
			}
			
			if (wdTmp >= 10000)
			{
				DrawFontL2R8x16(8*2-4,64,DIG_0 + wdTmp/10000);
			}
			if (wdTmp >= 1000)
			{
				DrawFontL2R8x16(8*3-4,64,DIG_0 + wdTmp/1000%10);
			}

			DrawFontL2R8x16(8*4-4,64,DIG_0 + wdTmp/100%10);
			DrawFontL2R8x16(8*5-4,64,SYM_PERIOD);
			DrawFontL2R8x16(8*6-8,64,DIG_0 + wdTmp/10%10);
			DrawFontL2R8x16(8*7-8,64,ALPH_V);
		}
		else
		{
			DrawFontL2R8x16(8*3-4,64,SYM_PERIOD);
			DrawFontL2R8x16(8*4-4,64,SYM_PERIOD);
		}
		//DrawFontL2R8x16(8*8-8,64,ALPH_X);

		DrawFontL2R8x16(8*8,64,ALPH_I);
		DrawFontL2R8x16(8*9-2,64,SYM_COLON);
		if ((byState == CTRL_STATE_CHARGE) || (CTRL_STATE_TEST == byState))
		{
			wdTmp = wdCurrent;
			if (wdTmp%10 >= 5)
			{
				wdTmp++; 
			}
			
			if (wdTmp >= 10000)
			{
				DrawFontL2R8x16(8*10-4,64,DIG_0 + wdTmp/10000);
			}
			if (wdTmp >= 1000)
			{
				DrawFontL2R8x16(8*11-4,64,DIG_0 + wdTmp/1000%10);
			}
			
			DrawFontL2R8x16(8*12-4,64,DIG_0 + wdTmp/100%10);
			DrawFontL2R8x16(8*13-4,64,SYM_PERIOD);
			
			DrawFontL2R8x16(8*14-8,64,DIG_0 + wdTmp/10%10);
			
			DrawFontL2R8x16(8*15-8,64,ALPH_A);
		}
		else
		{
			DrawFontL2R8x16(8*11-4,64,SYM_PERIOD);
			DrawFontL2R8x16(8*12-4,64,SYM_PERIOD);
		}

		DrawFontL2R8x16(0,80,ALPH_C);
		DrawFontL2R8x16(8,80,ALPH_a);
		DrawFontL2R8x16(8*2,80,ALPH_p);
		DrawFontL2R8x16(8*3,80,ALPH_a);
		DrawFontL2R8x16(8*4,80,ALPH_c);
		DrawFontL2R8x16(8*5,80,ALPH_i);
		DrawFontL2R8x16(8*6,80,ALPH_t);  
		DrawFontL2R8x16(8*7,80,ALPH_y);
		DrawFontL2R8x16(8*8-2,80,SYM_COLON);

		if (byState == CTRL_STATE_CHARGE)
		{
			tmp = dwdCapacity;
			tmp /= 7200*10;

			if (dwdCapacity%72000 >= 36000)
			{
				tmp ++;
			}

			if (tmp >= 9999)
			{
				DrawFontL2R8x16(8*9-4,80,DIG_9);
				DrawFontL2R8x16(8*10-4,80,DIG_9);
				DrawFontL2R8x16(8*11-4,80,DIG_9);
				DrawFontL2R8x16(8*12-4,80,SYM_PERIOD);
				DrawFontL2R8x16(8*13-8,80,DIG_9);
				DrawFontL2R8x16(8*14-8,80,ALPH_A);
				DrawFontL2R8x16(8*15-8,80,ALPH_H);
			}
			else
			{
				if (tmp >= 1000)
				{
					DrawFontL2R8x16(8*9-4,80,DIG_0 + tmp/1000);
				}
				if (tmp >= 100)
				{
					DrawFontL2R8x16(8*10-4,80,DIG_0 + tmp/100%10);
				}
				DrawFontL2R8x16(8*11-4,80,DIG_0 + tmp/10%10);
				DrawFontL2R8x16(8*12-4,80,SYM_PERIOD);
				DrawFontL2R8x16(8*13-8,80,DIG_0 + tmp%10);
				DrawFontL2R8x16(8*14-8,80,ALPH_A);
				DrawFontL2R8x16(8*15-8,80,ALPH_H);
			}
		}
		else
		{
			DrawFontL2R8x16(8*10,80,SYM_PERIOD);
			DrawFontL2R8x16(8*11,80,SYM_PERIOD);
			DrawFontL2R8x16(8*12,80,SYM_PERIOD);
		}

		tmp = RTC_GetCounter();
		tmp -= dwdChargeBeginTm;
		hour = tmp/3600;
		minute = tmp/60%60;
		second = tmp%60;
		

		DrawFontL2R8x16(0,96,ALPH_T);
		DrawFontL2R8x16(8,96,ALPH_i);
		DrawFontL2R8x16(8*2,96,ALPH_m);
		DrawFontL2R8x16(8*3,96,ALPH_e);
		DrawFontL2R8x16(8*4-2,96,SYM_COLON);

		if (hour > 999)
		{
			hour = 999;
		}
		if (hour >= 100)
		{
			DrawFontL2R8x16(8*5,96,DIG_0 + hour/100);
		}
		if (hour >= 10)
		{
			DrawFontL2R8x16(8*6,96,DIG_0 + hour/10%10);
		}
		DrawFontL2R8x16(8*7,96,DIG_0 + hour%10);
		DrawFontL2R8x16(8*8,96,SYM_COLON);
		
		DrawFontL2R8x16(8*9,96,DIG_0 + minute/10%10);
		DrawFontL2R8x16(8*10,96,DIG_0 + minute%10);
		DrawFontL2R8x16(8*11,96,SYM_COLON);
		DrawFontL2R8x16(8*12,96,DIG_0 + second/10%10);
		DrawFontL2R8x16(8*13,96,DIG_0 + second%10);
	}
	else if (MID_SET == byMID)
	{
		if (0 == wdML1Offset)
		{
			DrawFontL2R8x16Not(0,0,ALPH_C);
			DrawFontL2R8x16Not(8,0,ALPH_h);
			DrawFontL2R8x16Not(8*2,0,ALPH_a);
			DrawFontL2R8x16Not(8*3,0,ALPH_r);
			DrawFontL2R8x16Not(8*4,0,ALPH_g);
			DrawFontL2R8x16Not(8*5,0,ALPH_e);
			DrawFontL2R8x16Not(8*6,0,SPACE);
			DrawFontL2R8x16Not(8*7,0,ALPH_C);
			DrawFontL2R8x16Not(8*8,0,ALPH_o);
			DrawFontL2R8x16Not(8*9,0,ALPH_n);
			DrawFontL2R8x16Not(8*10,0,ALPH_f);
			DrawFontL2R8x16Not(8*11,0,ALPH_i);
			DrawFontL2R8x16Not(8*12,0,ALPH_g);
			//DrawFontL2R8x16(8*13,0,ALPH_X);
			//DrawFontL2R8x16(8*14,0,ALPH_X);
			DrawFontL2R8x16(0,16,ALPH_C);
			DrawFontL2R8x16(8,16,ALPH_h);
			DrawFontL2R8x16(8*2,16,ALPH_a);
			DrawFontL2R8x16(8*3,16,ALPH_r);
			DrawFontL2R8x16(8*4,16,ALPH_g);
			DrawFontL2R8x16(8*5,16,ALPH_e);
			DrawFontL2R8x16(8*6,16,SPACE);
			DrawFontL2R8x16(8*7,16,ALPH_D);
			DrawFontL2R8x16(8*8,16,ALPH_a);
			DrawFontL2R8x16(8*9,16,ALPH_t);
			DrawFontL2R8x16(8*10,16,ALPH_a);
			//DrawFontL2R8x16(8*11,0,ALPH_i);
			//DrawFontL2R8x16(8*12,0,ALPH_g);
			//DrawFontL2R8x16(8*13,0,ALPH_X);
			//DrawFontL2R8x16(8*14,0,ALPH_X);
		}
		else
		{
			DrawFontL2R8x16(0,0,ALPH_C);
			DrawFontL2R8x16(8,0,ALPH_h);
			DrawFontL2R8x16(8*2,0,ALPH_a);
			DrawFontL2R8x16(8*3,0,ALPH_r);
			DrawFontL2R8x16(8*4,0,ALPH_g);
			DrawFontL2R8x16(8*5,0,ALPH_e);
			DrawFontL2R8x16(8*6,0,SPACE);
			DrawFontL2R8x16(8*7,0,ALPH_C);
			DrawFontL2R8x16(8*8,0,ALPH_o);
			DrawFontL2R8x16(8*9,0,ALPH_n);
			DrawFontL2R8x16(8*10,0,ALPH_f);
			DrawFontL2R8x16(8*11,0,ALPH_i);
			DrawFontL2R8x16(8*12,0,ALPH_g);
			//DrawFontL2R8x16(8*13,0,ALPH_X);
			//DrawFontL2R8x16(8*14,0,ALPH_X);
			DrawFontL2R8x16Not(0,16,ALPH_C);
			DrawFontL2R8x16Not(8,16,ALPH_h);
			DrawFontL2R8x16Not(8*2,16,ALPH_a);
			DrawFontL2R8x16Not(8*3,16,ALPH_r);
			DrawFontL2R8x16Not(8*4,16,ALPH_g);
			DrawFontL2R8x16Not(8*5,16,ALPH_e);
			DrawFontL2R8x16Not(8*6,16,SPACE);
			DrawFontL2R8x16Not(8*7,16,ALPH_D);
			DrawFontL2R8x16Not(8*8,16,ALPH_a);
			DrawFontL2R8x16Not(8*9,16,ALPH_t);
			DrawFontL2R8x16Not(8*10,16,ALPH_a);
		}
		
	}
	else if (MID_CFG_SEL == byMID)
	{
		DrawFontL2R8x16(0,0,ALPH_C);
		DrawFontL2R8x16(8,0,ALPH_o);
		DrawFontL2R8x16(8*2,0,ALPH_n);
		DrawFontL2R8x16(8*3,0,ALPH_f);
		DrawFontL2R8x16(8*4,0,ALPH_i);
		DrawFontL2R8x16(8*5,0,ALPH_g);
		DrawFontL2R8x16(8*6,0,SPACE);
		DrawFontL2R8x16(8*7,0,ALPH_S);
		DrawFontL2R8x16(8*8,0,ALPH_e);
		DrawFontL2R8x16(8*9,0,ALPH_l);
		DrawFontL2R8x16(8*10,0,SYM_COLON);
		DrawFontL2R8x16Not(8*11,0,DIG_0 + (wdML2CfgOffset+1)/10%10);
		DrawFontL2R8x16Not(8*12,0,DIG_0 + (wdML2CfgOffset+1)%10);

		DrawFontL2R8x16(0,16,DIG_0 + (wdCurCfgDataIndex+1)/10%10);
		DrawFontL2R8x16(8,16,DIG_0 + (wdCurCfgDataIndex+1)%10);
		DrawFontL2R8x16(8*2,16,SYM_COLON);
		for (tmp = 0; tmp < 8; tmp++)
		{
			if (!stChargeCfgData.acName[tmp])
			{
				break;
			}
			DrawFontL2R8x16(8*(3+tmp),16,GetAlphDigIndex(stChargeCfgData.acName[tmp]));
		}
		y = 32;
		x = 0;
		DrawFontL2R8x16(x, y, ALPH_C);
		x += 8;
		DrawFontL2R8x16(x, y, ALPH_C);
		if (stChargeCfgData.I3)
		{
			x += 8;
			DrawFontL2R8x16(x, y, DIG_1);
		}
		x += 8;
		DrawFontL2R8x16(x, y, SYM_COLON);

		wdTmp = stChargeCfgData.I2;
		
		if (wdTmp >= 10000)
		{
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10000);
		}
		if (wdTmp >= 1000)
		{
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/1000%10);
		}
		x += 8;
		DrawFontL2R8x16(x,y,DIG_0 + wdTmp/100%10);
		x += 8;
		DrawFontL2R8x16(x,y,SYM_PERIOD);
		x += 8;
		DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10%10);
		x += 8;
		DrawFontL2R8x16(x,y,ALPH_A);

		if (stChargeCfgData.I3)
		{
			y += 16;
			x = 0;
			DrawFontL2R8x16(x, y, ALPH_C);
			x += 8;
			DrawFontL2R8x16(x, y, ALPH_C);
			x += 8;
			DrawFontL2R8x16(x, y, DIG_2);
			x += 8;
			DrawFontL2R8x16(x, y, SYM_COLON);

			wdTmp = stChargeCfgData.I3;
			
			if (wdTmp >= 10000)
			{
				x += 8;
				DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10000);
			}
			if (wdTmp >= 1000)
			{
				x += 8;
				DrawFontL2R8x16(x,y,DIG_0 + wdTmp/1000%10);
			}
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/100%10);
			x += 8;
			DrawFontL2R8x16(x,y,SYM_PERIOD);
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10%10);
			x += 8;
			DrawFontL2R8x16(x,y,ALPH_A);
		}

		y += 16;
		x = 0;
		DrawFontL2R8x16(x, y, ALPH_C);
		x += 8;
		DrawFontL2R8x16(x, y, ALPH_V);
		x += 8;
		DrawFontL2R8x16(x, y, SYM_COLON);

		wdTmp = stChargeCfgData.Ucv;
		
		if (wdTmp >= 10000)
		{
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10000);
		}
		if (wdTmp >= 1000)
		{
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/1000%10);
		}
		x += 8;
		DrawFontL2R8x16(x,y,DIG_0 + wdTmp/100%10);
		x += 8;
		DrawFontL2R8x16(x,y,SYM_PERIOD);
		x += 8;
		DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10%10);
		x += 8;
		DrawFontL2R8x16(x,y,ALPH_V);

		if (stChargeCfgData.T5)
		{
			y += 16;
			x = 0;
			DrawFontL2R8x16(x, y, ALPH_V);
			x += 8;
			DrawFontL2R8x16(x, y, ALPH_a);
			x += 8;
			DrawFontL2R8x16(x, y, ALPH_b);
			x += 8;
			DrawFontL2R8x16(x, y, ALPH_s);
			x += 8;
			DrawFontL2R8x16(x, y, ALPH_o);
			x += 8;
			DrawFontL2R8x16(x, y, SYM_COLON);

			wdTmp = stChargeCfgData.U5;
			
			if (wdTmp >= 10000)
			{
				x += 8;
				DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10000);
			}
			if (wdTmp >= 1000)
			{
				x += 8;
				DrawFontL2R8x16(x,y,DIG_0 + wdTmp/1000%10);
			}
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/100%10);
			x += 8;
			DrawFontL2R8x16(x,y,SYM_PERIOD);
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10%10);
			x += 8;
			DrawFontL2R8x16(x,y,ALPH_V);
		}

		y += 16;
		x = 0;
		DrawFontL2R8x16(x, y, ALPH_V);
		x += 8;
		DrawFontL2R8x16(x, y, ALPH_f);
		x += 8;
		DrawFontL2R8x16(x, y, ALPH_l);
		x += 8;
		DrawFontL2R8x16(x, y, ALPH_o);
		x += 8;
		DrawFontL2R8x16(x, y, ALPH_a);
		x += 8;
		DrawFontL2R8x16(x, y, ALPH_t);
		x += 8;
		DrawFontL2R8x16(x, y, SYM_COLON);

		wdTmp = stChargeCfgData.U6;
		
		if (wdTmp >= 10000)
		{
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10000);
		}
		if (wdTmp >= 1000)
		{
			x += 8;
			DrawFontL2R8x16(x,y,DIG_0 + wdTmp/1000%10);
		}
		x += 8;
		DrawFontL2R8x16(x,y,DIG_0 + wdTmp/100%10);
		x += 8;
		DrawFontL2R8x16(x,y,SYM_PERIOD);
		x += 8;
		DrawFontL2R8x16(x,y,DIG_0 + wdTmp/10%10);
		x += 8;
		DrawFontL2R8x16(x,y,ALPH_V);
		

	}
	else if (MID_SHOW_CHG_DATA == byMID)
	{
		DrawFontL2R8x16(0,0,ALPH_C);
		DrawFontL2R8x16(8,0,ALPH_h);
		DrawFontL2R8x16(8*2,0,ALPH_a);
		DrawFontL2R8x16(8*3,0,ALPH_r);
		DrawFontL2R8x16(8*4,0,ALPH_g);
		DrawFontL2R8x16(8*5,0,ALPH_e);
		DrawFontL2R8x16(8*6,0,SPACE);
		DrawFontL2R8x16(8*7,0,ALPH_N);
		DrawFontL2R8x16(8*8,0,ALPH_O);
		DrawFontL2R8x16(8*9,0,SYM_PERIOD);
		DrawFontL2R8x16(8*10,0,SYM_COLON);
		wdTmp = wdML2ChgOffset + 1;
		DrawFontL2R8x16Not(8*11,0,DIG_0 + wdTmp/1000%10);
		DrawFontL2R8x16Not(8*12,0,DIG_0 + wdTmp/100%10);
		DrawFontL2R8x16Not(8*13,0,DIG_0 + wdTmp/10%10);
		DrawFontL2R8x16Not(8*14,0,DIG_0 + wdTmp%10);

		if ((dwdDispChgCapacity != 0xffffffff) && (dwdDispChgTmLen != 0xffffffff))
		{
			DrawFontL2R8x16(0,16,ALPH_C);
			DrawFontL2R8x16(8,16,ALPH_a);
			DrawFontL2R8x16(8*2,16,ALPH_p);
			DrawFontL2R8x16(8*3,16,ALPH_a);
			DrawFontL2R8x16(8*4,16,ALPH_c);
			DrawFontL2R8x16(8*5,16,ALPH_i);
			DrawFontL2R8x16(8*6,16,ALPH_t);
			DrawFontL2R8x16(8*7,16,ALPH_y);
			DrawFontL2R8x16(8*8-2,16,SYM_COLON);


			tmp = dwdDispChgCapacity;
		
			if (tmp >= 9999)
			{
				DrawFontL2R8x16(8*9-4,16,DIG_9);
				DrawFontL2R8x16(8*10-4,16,DIG_9);
				DrawFontL2R8x16(8*11-4,16,DIG_9);
				DrawFontL2R8x16(8*12-4,16,SYM_PERIOD);
				DrawFontL2R8x16(8*13-8,16,DIG_9);
				DrawFontL2R8x16(8*14-8,16,ALPH_A);
				DrawFontL2R8x16(8*15-8,16,ALPH_H);
			}
			else
			{
				if (tmp >= 1000)
				{
					DrawFontL2R8x16(8*9-4,16,DIG_0 + tmp/1000);
				}
				if (tmp >= 100)
				{
					DrawFontL2R8x16(8*10-4,16,DIG_0 + tmp/100%10);
				}
				DrawFontL2R8x16(8*11-4,16,DIG_0 + tmp/10%10);
				DrawFontL2R8x16(8*12-4,16,SYM_PERIOD);
				DrawFontL2R8x16(8*13-8,16,DIG_0 + tmp%10);
				DrawFontL2R8x16(8*14-8,16,ALPH_A);
				DrawFontL2R8x16(8*15-8,16,ALPH_H);
			}

			tmp = dwdDispChgTmLen;
			hour = tmp/3600;
			minute = tmp/60%60;
			second = tmp%60;
			

			DrawFontL2R8x16(0,32,ALPH_T);
			DrawFontL2R8x16(8,32,ALPH_i);
			DrawFontL2R8x16(8*2,32,ALPH_m);
			DrawFontL2R8x16(8*3,32,ALPH_e);
			DrawFontL2R8x16(8*4-2,32,SYM_COLON);

			if (hour > 999)
			{
				hour = 999;
			}
			if (hour >= 100)
			{
				DrawFontL2R8x16(8*5,32,DIG_0 + hour/100);
			}
			if (hour >= 10)
			{
				DrawFontL2R8x16(8*6,32,DIG_0 + hour/10%10);
			}
			DrawFontL2R8x16(8*7,32,DIG_0 + hour%10);
			DrawFontL2R8x16(8*8,32,SYM_COLON);
			
			DrawFontL2R8x16(8*9,32,DIG_0 + minute/10%10);
			DrawFontL2R8x16(8*10,32,DIG_0 + minute%10);
			DrawFontL2R8x16(8*11,32,SYM_COLON);
			DrawFontL2R8x16(8*12,32,DIG_0 + second/10%10);
			DrawFontL2R8x16(8*13,32,DIG_0 + second%10);

			
		}
			
	}
	else
	{
		byMID = MID_TOP;
	}
}

/****************************************************************
* Function:		ClrDispBuf
* Description:	将显示缓存中的数据清零
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void ClrDispBuf(void)
{
	U16 i;
	U8 *pData = NULL;

	i = sizeof(LcdData);
	pData = (U8*)LcdData;
	while (i)
	{
		i--;
		*pData = 0x00;
		pData++;
	}
}
/****************************************************************
* Function:		SetDispBufEx
* Description:	设置显示缓存中的数据
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void SetDispBufEx(U8 Data)
{
	U16 i;
	U8 *pData = NULL;

	i = sizeof(LcdData);
	pData = (U8*)LcdData;
	while (i)
	{
		i--;
		*pData = Data;
		pData++;
	}
}


/****************************************************************
* Function:		Display
* Description:	显示函数
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void Display(void)
{
	static BYTE byCnt = 0;
	BYTE byTmp;
	
#if 1
	
	if (byCnt >= 66)
	{
		byCnt = 0;
	}
	
	if (byCnt < 8)
	{
		LcdRefresh(0,128 -2 - 2*byCnt,240,2,(BYTE *)(&abyLogoBmp[2*byCnt][0]));
	}
	else if (8 == byCnt)
	{
		ClrDispBuf();

		DrawMenu();
	}
	else if (byCnt == 8+(128-16)/4 + 1)
	{

		ClrDispBuf();

		DrawCoordinate();

		DrawCurve(DRAW_MODE_OR);
	}
	else
	{
		 if (byCnt > 8+(128-16)/4 + 1)
		 {
		 	byTmp = byCnt - (8+(128-16)/4 + 1 + 1);
			//byTmp = (128 - 16)/4 - 1 - byTmp;
			LcdRefresh(0,4*((128 - 16)/4 - 1 - byTmp),120,4,(BYTE *)(&LcdData[4*byTmp][0]));
		 }
		 else
		 {
		 	byTmp = byCnt - (8 + 1);
			LcdRefresh(120,4*((128 - 16)/4 - 1 - byTmp),120,4,(BYTE *)(&LcdData[4*byTmp][0]));
		 }
	}

	byCnt++;
	
#else
	
	static BYTE byDiv = 0;
	static BYTE bFirst = 1;
	static BYTE byTmDiv = 0;
	
	byTmDiv++;

	if (byTmDiv < 5)
	{
		return;
	}
	byTmDiv = 0;

	byDiv ++;
	
	if (byDiv >= 3)
	{
		byDiv = 0;
	}

	if ((0 == byDiv) || bFirst)
	{
		ClrDispBuf();

		DrawMenu();

		LcdRefresh(120,0,120,128-16,(BYTE *)LcdData);
		
	}
	
	if ((2 == byDiv) || bFirst)
	{
		LcdRefresh(0,128-16,240,16,(BYTE *)abyLogoBmp);
		
	}

	if ((1 == byDiv) || bFirst)
	{
		if (dwdUITCnt <= 1100)
		{
			dwdUITCnt ++;
			//dwdCurT += 20;
		}
		ClrDispBuf();

		DrawCoordinate();

		DrawCurve(DRAW_MODE_OR);

		/* 充电时才显示曲线 */
		if (CTRL_STATE_CHARGE == byState)
		{
			//DrawCurve(DRAW_MODE_OR);
		}
		else
		{
			
		}

		LcdRefresh(0,0,120,128-16,(BYTE *)LcdData);
	}

	bFirst = 0;
#endif
}
/****************************************************************
* Function:		ReadRTChargeDataTest
* Description:	调试时使用的，如不再需要可删除
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
BYTE ReadRTChargeDataTest(DWORD dwdPos, WORD *pwdVoltage, WORD *pwdCurrent)
{
#if 0
	static UIT_t stUIT; /* 如果读取失败，上一个数值会得到保留 */

	if (!dwdPos)
	{
		stUIT.U = 0;
		stUIT.I = 0;
	}
	
	DBLoadUITemp(wdCurUITFileInex, dwdPos, &stUIT);

	*pwdVoltage = stUIT.U;
	*pwdCurrent = stUIT.I;

#else
	if (dwdPos < 1000)
	{
		*pwdVoltage = testUData[dwdPos];
		*pwdCurrent = testIData[dwdPos];
	}
	else
	{
		*pwdVoltage = testUData[999];
		*pwdCurrent = testIData[999];
	}

	wdBtVoltage = *pwdVoltage;
	wdCurrent = *pwdCurrent;
#endif
	return 0;
}

/****************************************************************
* Function:		GetDispCurveValue
* Description:	获取要显示的曲线数据，目前电压和电流值都是从TF卡中对应文件中读取的
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void GetDispCurveValue(void)
{
	BYTE i;
	DWORD pos;
	
	byDispNum = dwdDispUITCnt/UnitT;

	DBLoadUI (wdDispCurUITFileInex,
				0,
				byDispNum,
				UnitT,
				CurveUBuf,
				CurveIBuf);
	#if 0
	else
	{
		for (i = 0, pos = 0; i < byDispNum; i++, pos += UnitT)
		{
			//ReadRTChargeData(pos, CurveUBuf + i, CurveIBuf + i);
			ReadRTChargeDataTest(pos, CurveUBuf + i, CurveIBuf + i);

			if (!(i%10)) /* 清看门狗 */
			{
				IWDG_ReloadCounter();
			}
		}
	}
	#endif

	AdjustUIValue();
}

/****************************************************************
* Function: 	AdjustUIValue
* Description:	根据范围调整电压电流值
* Input:		
* Output:		
* Return:		
* Other:		
****************************************************************/
void AdjustUIValue(void)
{
	BYTE i;
	S16 tmpMax;
	S16 tmpMin;
	S16 tmpUnit;
	S16 ModMin;
	S16 ModMax;
	S16 tmp;
	
	CurveUMax = CurveUBuf[0];
	CurveIMax = CurveIBuf[0];
	CurveUMin = CurveUMax;
	CurveIMin = CurveIMax;
	
	for (i = 1; i < byDispNum; i++)
	{
		if (CurveUBuf[i] > CurveUMax)
		{
			CurveUMax = CurveUBuf[i];
		}

		if (CurveUBuf[i] < CurveUMin)
		{
			CurveUMin = CurveUBuf[i];
		}
		

		if (CurveIBuf[i] > CurveIMax)
		{
			CurveIMax = CurveIBuf[i];
		}

		if (CurveIBuf[i] < CurveIMin)
		{
			CurveIMin = CurveIBuf[i];
		}
	}

#if 1
	if (CurveUMax == CurveUMin)
	{
		if (0 == CurveUMin)
		{
			CurveUMax = DISPLAY_CURVE_HEIGHT - 1;
		}
		else
		{
			//UnitU = 2*(CurveUMax + 1)/DISPLAY_CURVE_HEIGHT + 1;
			CurveUMin = 0;
		}
	}
	else
	{
		UnitU = (CurveUMax - CurveUMin + 2)/(DISPLAY_CURVE_HEIGHT) + 1;
		
		if (CurveUMin > 2*UnitU)
		{
			CurveUMin -= 2*UnitU;
		}
		else
		{
			CurveUMin = 0;
		}

		
	}

	UnitU = (CurveUMax - CurveUMin)/DISPLAY_CURVE_HEIGHT + 1;


	if (CurveIMax == CurveIMin)
	{
		if (0 == CurveIMin)
		{
			CurveIMax = DISPLAY_CURVE_HEIGHT - 1;
		}
		else
		{
			CurveIMin = 0;
		}

		UnitI = (CurveIMax - CurveIMin)/DISPLAY_CURVE_HEIGHT + 1;
	}
	else
	{
		UnitI = (CurveIMax - CurveIMin + 2)/DISPLAY_CURVE_HEIGHT + 1;
		if (CurveIMin > 2*UnitI)
		{
			CurveIMin -= 2*UnitI;
		}
		else
		{
			CurveIMin = 0;
		}
		
	}

	UnitI = (CurveIMax - CurveIMin)/DISPLAY_CURVE_HEIGHT + 1;

		
#else
	if (CurveUMax == CurveUMin)
	{
		if (CurveUMax < 2*DISPLAY_CURVE_HEIGHT)
		{
			CurveUMax = 2*DISPLAY_CURVE_HEIGHT;
		}
		else
		{
			CurveUMin = 0;
		}
	}

	if (CurveIMax == CurveIMin)
	{
		if (CurveIMax < 2*DISPLAY_CURVE_HEIGHT)
		{
			CurveIMax = 2*DISPLAY_CURVE_HEIGHT;
		}
		else
		{
			CurveIMin = 0;
		}
	}

	UnitU = (CurveUMax - CurveUMin + 1)/DISPLAY_CURVE_HEIGHT + 1;
	UnitI = (CurveIMax - CurveIMin + 1)/DISPLAY_CURVE_HEIGHT + 1;
#endif

#if 0
	if (UnitU >= 10000)
	{
		UnitU = (UnitU + 10000 - 1)/10000*10000;
	}
	else if (UnitU > 5000)
	{
		UnitU = 10000;
	}
	else if (UnitU >= 1000)
	{
		UnitU = (UnitU + 1000 - 1)/1000*1000;
	}
	else if (UnitU > 500)
	{
		UnitU = 1000;
	}
	else if (UnitU >= 100)
	{
		UnitU = (UnitU + 100 - 1)/100*100;
	}
	else if (UnitU > 50)
	{
		UnitU = 100;
	}
	else if (UnitU >= 10)
	{
		UnitU = (UnitU + 10 - 1)/10*10;
	}
	else if (UnitU > 5)
	{
		UnitU = 10;
	}

	if (UnitI >= 10000)
	{
		UnitI = (UnitI + 10000 - 1)/10000*10000;
	}
	else if (UnitI > 5000)
	{
		UnitI = 10000;
	}
	else if (UnitI >= 1000)
	{
		UnitI = (UnitI + 1000 - 1)/1000*1000;
	}
	else if (UnitI > 500)
	{
		UnitI = 1000;
	}
	else if (UnitI >= 100)
	{
		UnitI = (UnitI + 100 - 1)/100*100;
	}
	else if (UnitI > 50)
	{
		UnitI = 100;
	}
	else if (UnitI >= 10)
	{
		UnitI = (UnitI + 10 - 1)/10*10;
	}
	else if (UnitI > 5)
	{
		UnitI = 10;
	}
#endif

	for (i = 0; i < byDispNum; i++)
	{
		CurveUBuf[i] = (CurveUBuf[i] - CurveUMin)/UnitU;
		CurveIBuf[i] = (CurveIBuf[i] - CurveIMin)/UnitI;
	}


#if 1
	wdCoUMarkSpace = 20;
	wdCoIMarkSpace = 20;
#else
	tmpMin = CurveUMin + 20*UnitU*(CurveUMin != 0) - CurveUMin%(20*UnitU);	
	tmpMax = CurveUMax - CurveUMax%(20*UnitU);
	wdCoUMin = tmpMin/UnitU;
	wdCoUMax = tmpMax/UnitU;
	wdCoUMarkSpace = 20;

	for (wdCoUScale = 0,tmp = UnitU; tmp != 0; tmp /= 10)
	{
		wdCoUScale++;
	}

	tmpMin = CurveIMin + 20*UnitI*(CurveIMin != 0) - CurveIMin%(20*UnitI);
	tmpMax = CurveIMax - CurveIMax%(20*UnitI);
	wdCoIMax = tmpMax/UnitI;
	wdCoIMin = tmpMin/UnitI;
	wdCoIMarkSpace = 20;

	for (wdCoIScale = 0,tmp = UnitI; tmp != 0; tmp /= 10)
	{
		wdCoIScale++;
	}	
#endif

}


/* end of file */

