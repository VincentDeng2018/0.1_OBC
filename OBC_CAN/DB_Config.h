/*******************************************************************************
*            DB_Config.h
********************************************************************************
********************************************************************************
*
* THIS INFORMATION IS PROPRIETARY TO BYD Corporation
*
********************************************************************************
*
*
********************************************************************************
********************************************************************************
*    FILE NAME: DB_Config.h
*
*    DESCRIPTION: Header file for database configure parameters
*
*    ORIGINATOR: DYH
*
*    DATE: 2017/08/22
*
*             Version/Date   Author     Modification
*    HISTORY:  V1.0/20170822  DYH       Configure some customer parameters
********************************************************************************/

#ifndef _DB_CONFIG_H
#define _DB_CONFIG_H



/*******************************************************************************
* Macros
*******************************************************************************/
#define TextStringSuffixNoText ""
#define TextStringSuffixFail "Fail"
#define TextStringSuffixOk "OK"
#define TextStringSuffixOn "On"
#define TextStringSuffixOff  "Off"
#define TextStringSuffixClear  "Clear"
#define TextStringSuffixOpen  "Open"
#define TextStringSuffixClose "Close"

/******************** Diagnostic ID ******************************/
typedef enum
{
    PACK_CHG_OV_ALARM,                  //0 
    PACK_CHG_UV_FAULT,                  //1 
    PACK_DISCHG_OV_ALARM,               //2 
    PACK_DISCHG_UV_ALARM,               //3 
    PACK_SENSOR_FAULT,                  //4 
    PACK_SENSOR_NOT_STABLE,             //5 
    CELL_OV_WARNING,                    //6 
    CELL_OV_ALARM,                      //7 
    CELL_OV_FAULT,                      //8 
    CELL_UV_WARNING,                    //9 
    CELL_UV_ALARM,                      //10
    CELL_UV_FAULT,                      //11
    CELL_NOT_BALANCE,                   //12
    SOC_LOW_WARNING,                    //13
    BATT_EOL_FAULT,                     //14
    CELL_SENSOR_FAULT,                  //15
    CELL_SENSOR_NOT_STABLE,             //16
    BATT_CHG_OT_ALARM,                  //17
    BATT_CHG_UT_ALARM,                  //18
    BATT_DISCHG_OT_ALARM,               //19
    BATT_DISCHG_UT_ALARM,               //20
    BATT_TEMP_SENSOR_FAULT,             //21
    BATT_TEMP_SENSOR_NOT_STABLE,        //22
    CHG_OC_WARNING,                     //23
    CHG_OC_ALARM,                       //24
    DISCHG_OC_WARNING,                  //25
    DISCHG_OC_ALARM,                    //26
    BATT_CHG_CURR_REVERSE,              //27
    BATT_DISCHG_CURR_REVERSE,           //28
    LEG_CURRRENT_INEQUALITY,            //29
    LEG_CURRENT_SENSOR_FAULT,           //30
    LEG_CURRENT_SENSOR_NOT_STABLE,      //31
    OUTLV_OV_ALARM,                     //32
    OUTLV_UV_ALARM,                     //33
    OUTLV_SENSOR_NOT_STABLE,            //34
    OUTLV_SENSOR_FAULT,                 //35
    OUTHV_OV_ALARM,                     //36
    OUTHV_UV_ALARM,                     //37
    OUTHV_SENSOR_NOT_STABLE,            //38
    OUTHV_SENSOR_FAULT,                 //39
    BUCK_HW_OC_ALARM,                   //40
    BOOST_HW_OC_ALARM,                  //41
    OUTHV_HW_OV_ALARM,                  //42
    OUTHV_OC_ALARM,                     //43
    OUTHV_CURRENT_SENSOR_FAULT,         //44
    OUTHV_CURRENT_SENSOR_NOT_STABLE,    //45
    LEVEL1_OL_WARNING,                  //46
    LEVEL2_OL_ALARM,                    //47
    CTRL_OT_WARNING,                    //48
    CTRL_OT_ALARM,                      //49
    CTRL_TEMP_SENSOR_NOT_STABLE,        //50
    CTRL_TEMP_SENSOR_FAULT,             //51
    HV_OT_ALARM,                        //52
    HV_TEMP_SENSOR_NOT_STABLE,          //53
    HV_TEMP_SENSOR_FAULT,               //54
    LV_OT_ALARM,                        //55
    LV_TEMP_SENSOR_NOT_STABLE,          //56
    LV_TEMP_SENSOR_FAULT,               //57
    SPI_FAIL,                           //58
    SPI_DATA_INVALID,                   //59
    CAN_LOST,                           //60
    CAN_DATA_INVALID,                   //61
    EEPROM_COMMUNICATION_LOST,          //62
    EEPROM_RW_FAIL,                     //63
    CONTACTOR_FAIL,                     //64
    POWER_CIRCUIT_FAULT,                //65
    AUX_POWER_FAULT,                    //66
    CPU_LOAD_HIGH,                      //67
    MEMORY_LOW,                         //68
    SW_INCOMPATIBLE,                    //69
    ADC_MODULE_FAULT,                   //70
    CONFIGURE_ERROR,                    //71
    CONTACTOR_STATUS,                   //72
    BALANCE_STATUS,                     //73
    CONFIGURE_RESET,                    //74
    POWER_ON,                           //75
    BMS_MODE_TRANSFER,                  //76
    SOC_CALIBRATE,                      //77
    PAR_CHANGE,                         //78
    DIAG_ID_END
}enDiagEventID;

typedef struct
{
    AnsiString TextString;
    AnsiString TextStringAct;     //NB active suffix
    AnsiString TextStringIna;     //NB inactive suffix
}LogString_tag;


typedef struct
{
    /* first 32 bit */
    UINT32 hourFormat: 1; /* 24h or 12h */
    UINT32 amPmIndicator: 1; /* am or pm */
    UINT32 sortFormat: 1; /* dd/mm/yyyy or mm/dd/yyyy */
    UINT32 year: 13; 
        
    UINT32 month: 4;
    UINT32 reserved: 12;
    
    /* second 32 bit */
    UINT32 day: 5;
    UINT32 hour: 6;
    UINT32 minute: 5;
    UINT32 second: 6;
    UINT32 mSecond: 10;
}RTC_Time;


typedef struct
{
    UINT32  eventId: 10;
    UINT32  eventType: 5;
    UINT32  activeOrNot: 1;
    UINT32  data: 16;
}EventInfo;


typedef struct
{
    RTC_Time timeInfo;
    EventInfo logInfo;
}EventLog_tag;

/*******************************************************************************
* data & structure
*******************************************************************************/
extern const LogString_tag LogString[DIAG_ID_END];

#endif /* DB_Config.h */
