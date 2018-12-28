/******************************************************************************************************
*            CanOpenOdTypes.h
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: CanOpenOdTypes.h
*
*    DESCRIPTION: Header file for object dictionary
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/24 21:48:19
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                      2018/5/24 21:48:23
******************************************************************************************************/

#ifndef _CANOPENODTYPES_H
#define _CANOPENODTYPES_H
#ifdef __cplusplus
 extern "C" {
#endif
#include  <stdint.h>

/************************* CONSTANTES **********************************/
/** this are static defined datatypes taken fCODE the canopen standard. They
 *  are located at index 0x0001 to 0x001B. As described in the standard, they
 *  are in the object dictionary for definition purpose only. a device does not
 *  to support all of this datatypes.
 */
typedef enum
{
    BOOLEAN_1 = 0x0,
    UNSIGNED8,
    UNSIGNED16,
    UNSIGNED24,
    UNSIGNED32,
    UNSIGNED40,
    UNSIGNED48,
    UNSIGNED56,
    UNSIGNED64,
    INTEGER8,
    INTEGER16,
    INTEGER24,
    INTEGER32,
    INTEGER40,
    INTEGER48,
    INTEGER56,
    INTEGER64,
    REAL32,
    REAL64,
    OCTET_STRING,
    VISIBLE_STRING,
    UNICODE_STRING,
    TIME_OF_DAY,
    TIME_DIFFERENCE,
    DOMAIN_1
}enOdDataType;

typedef enum
{
    CHANGE_TO_PRE_OPERATION = 0x0,
    NO_CHANGE_STATE,
    CHANGE_TO_STOPPED
}enErrorBehavior;

#define pdo_communication_parameter 0x20
#define pdo_mapping                 0x21
#define sdo_parameter               0x22
#define identity                    0x23

/* CanFestival is using 0x24 to 0xFF to define some types containing a
 value range (See how it works in objdict.c)
 */


/** Each entry of the object dictionary can be READONLY (RO), READ/WRITE (RW),
 *  WRITE-ONLY (WO)
 */
#define RW     0x00
#define WO     0x01
#define RO     0x02

#define TO_BE_SAVE  0x04
#define DCF_TO_SEND 0x08

#define OD_RW_SUCCESS 0u
#define OD_RW_OUT_OF_RANGE 1u
#define OD_RW_LENGTH_ERROR 2u
#define OD_RW_SUB_INDEX_ERROR 3u


/************************ STRUCTURES ****************************/
/** This are some structs which are neccessary for creating the entries
 *  of the object dictionary.
 */
typedef struct
{
    uint8_t                 u8AccessType;
    uint8_t                 u8DataType; /* Defines of what datatype the entry is */
    uint8_t                 u8size;      /* The size (in Byte) of the variable */
    void*                   pObject;   /* This is the pointer of the Variable */
}stOdSubInfo_t;

/** Struct for creating entries in the communictaion profile
 */
typedef struct
{
    stOdSubInfo_t*   pSubIndex;   /* Pointer to the subindex */
    uint8_t   u8SubCount;   /* the count of valid entries for this subindex
                         * This count here defines how many memory has been
                         * allocated. this memory does not have to be used.
                         */
    uint16_t   u16OdIndex;
}stOdSubInfoMap_t;

/*typedef struct of error register: 0x1001 */
typedef struct
{
    uint8_t bGenericError: 1;
    uint8_t bCurrent: 1;
    uint8_t bVoltage: 1;
    uint8_t bTemperature: 1;

    uint8_t bCommError: 1;
    uint8_t bReserved0: 1;
    uint8_t bReserved1: 1;
    uint8_t bUnused0: 1;
}stErrorRegister_t;

/*typedef struct of COB-ID time stamp object: 0x1012 */
typedef struct
{
    uint32_t StdCanId: 11;
    uint32_t ExtCanId: 18;

    uint32_t Frame: 1;
    uint32_t Produce: 1;
    uint32_t Consume: 1;
}stTimeStamp_t;

#ifdef __cplusplus
 }
#endif
/************************** MACROS *********************************/
#endif
