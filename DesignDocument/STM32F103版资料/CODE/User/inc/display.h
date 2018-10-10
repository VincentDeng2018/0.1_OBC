/****************************************************************
 Copyright (C),2014-2016,SZZKJ Technology
 File name:		display.h
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

#ifndef __DISPLAY_H
#define	__DISPLAY_H

#include "def.h"
#include "stm3210x_eval.h"


#ifdef __cplusplus
	extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

#define DRAW_MODE_OR	0
#define DRAW_MODE_AND	1
#define DRAW_MODE_XOR	2

enum MID_e 
{
	MID_TOP = 0,
	MID_SET,
	MID_CFG_SEL,
	MID_SHOW_CHG_DATA
};

enum MID_LVL1_e 
{
	MID_LVL1_CFG = 0,
	MID_LVL1_CHG
};

enum CFG_CURVE_e 
{
	CCE_IUUA3 = 0,
	CCE_IUUA3_2CC,
	CCE_IUIA4,
	CCE_IUIA4_2CC,
	CCE_MAX
};




extern U8 byMID;
extern U16 wdML1Offset;
extern U16 wdML1OffsetMax;
extern U16 wdML1OffsetSel; /* 0xffff 表示未选定 */
extern U16 wdML2CfgOffset;
extern U16 wdML2CfgOffsetMax;
extern U16 wdML2CfgOffsetSel; /* 0xffff 表示未选定 */
extern U16 wdML2ChgOffset;
extern U16 wdML2ChgOffsetMax;
extern U16 wdML2ChgOffsetSel; /* 0xffff 表示未选定 */


extern int iDispCurErr;
extern U32 dwdDispChgTmLen;
extern U32 dwdDispChgCapacity;

void Display(void);

#ifdef __cplusplus
}
#endif

#endif
/* end of file */

