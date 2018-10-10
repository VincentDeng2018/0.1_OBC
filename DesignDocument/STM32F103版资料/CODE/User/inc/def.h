/****************************************************************
 Copyright (C),2014-2016,SZZKJ Technology
 File name:		lcd.h
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

#ifndef __DEF_H
#define	__DEF_H

#ifdef __cplusplus
	extern "C" {
#endif

/* This type MUST be 8 bit */
#ifndef CHAR
typedef char			CHAR;
#endif


#ifndef BYTE
typedef unsigned char	BYTE;
#endif

/* These types MUST be 16 bit */
#ifndef SHORT
typedef short			SHORT;
#endif

#ifndef WORD
typedef unsigned short	WORD;
#endif

#ifndef WCHAR
typedef unsigned short	WCHAR;
#endif

/* These types MUST be 16 bit or 32 bit */
#ifndef INT
typedef int				INT;
#endif

#ifndef UINT
typedef unsigned int	UINT;
#endif

/* These types MUST be 32 bit */
#ifndef LONG
typedef long			LONG;
#endif

#ifndef DWORD
typedef unsigned long	DWORD;
#endif

#ifndef U8
#define U8		BYTE
#endif


#ifndef U16
#define U16		WORD
#endif

#ifndef U32
#define U32		DWORD
#endif

#ifndef BOOL
#define BOOL	U8
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif


#ifdef __cplusplus
}
#endif

#endif
/* end of file */

