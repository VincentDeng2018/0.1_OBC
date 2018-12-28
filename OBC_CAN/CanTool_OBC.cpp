//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CanTool_OBC.h"
#include "CanOpenBasic.h"
#include "CanOpenProtocol.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;


//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
    CanDriverObj = new CanDriver();
    CanProtocolObj = new CanProtocol();
}

void __fastcall TMainForm::oolInfo1Click(TObject *Sender)
{
    AboutBox->Show();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::CAN1Click(TObject *Sender)
{
     CanSetupDlg->Show();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::MainFormShow(TObject *Sender)
{
    CanSetupDlg->Show();
    ScreenInitial();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::ScreenInitial(void)
{
    StringGrid_CanFrame->ColCount = 6;
    StringGrid_CanFrame->RowCount = 4096;
    StringGrid_CanFrame->ColWidths[0] = 40;
    StringGrid_CanFrame->ColWidths[1] = 80;
    StringGrid_CanFrame->ColWidths[2] = 80;
    StringGrid_CanFrame->ColWidths[3] = 50;
    StringGrid_CanFrame->ColWidths[4] = 40;
    StringGrid_CanFrame->ColWidths[5] = 170;
    StringGrid_CanFrame->DefaultRowHeight = 23;
    
    StringGrid_CanFrame->Cells[0][0] = "序号";
    StringGrid_CanFrame->Cells[1][0] = "时间";
    StringGrid_CanFrame->Cells[2][0] = "类型";
    StringGrid_CanFrame->Cells[3][0] = "ID";
    StringGrid_CanFrame->Cells[4][0] = "长度";
    StringGrid_CanFrame->Cells[5][0] = "数据";
    
    CanFrameIndex = 1;
    StopUpdate = 0;
    CanFrameGridBusy = 0;
}


void __fastcall TMainForm::TimerTask(TObject *Sender)
{
    TDateTime  PresentTime;
    
    if((SystemTime_ms % 1000u) == 0u)
    {
        PresentTime = Now();
        L_DateAndTime->Caption = DateTimeToStr(PresentTime);
    }

    SystemTime_ms = SystemTime_ms + 5u;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::MainFormClose(TObject *Sender, TCloseAction &Action)
{
       CanDriverObj->ReleaseCanDevice();
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::InsertOneCanFrame(VCI_CAN_OBJ *pCanMsg)
{
    AnsiString tmpstr;

    CanFrameGridBusy = 1;
    
    if(StopUpdate == 0)
    {
        StringGrid_CanFrame->Cells[0][CanFrameIndex] = CanFrameIndex;
        if(pCanMsg->TimeStamp)
        {
            StringGrid_CanFrame->Cells[1][CanFrameIndex] = pCanMsg->TimeStamp;
        }
        else
        {
            StringGrid_CanFrame->Cells[1][CanFrameIndex] = SystemTime_ms;
        }
        StringGrid_CanFrame->Cells[2][CanFrameIndex] = "N/A";
        StringGrid_CanFrame->Cells[3][CanFrameIndex] = IntToHex((int)pCanMsg->ID, 1);
        StringGrid_CanFrame->Cells[4][CanFrameIndex] = pCanMsg->DataLen;
        
        if(pCanMsg->RemoteFlag == 0)
        {
            for(int j=0; j < pCanMsg->DataLen; j++)
            {
                tmpstr += IntToHex((int)pCanMsg->Data[j], 2) + " ";
            }
            
            StringGrid_CanFrame->Cells[5][CanFrameIndex] = tmpstr;
        }
        else
        {
            StringGrid_CanFrame->Cells[5][CanFrameIndex] = "   ";
        }
    
        if(++CanFrameIndex >= 4095)
        {
            CanFrameIndex = 1;
        }
    }
    
    CanFrameGridBusy = 0;
}


void __fastcall TMainForm::StopUpdateCanFrame(void)
{
    StopUpdate = 1;
}



void __fastcall TMainForm::ClearCanFrame(void)
{
    CanFrameGridBusy = 1;
    for(int i = 1; i <= CanFrameIndex; i++)
    {
        StringGrid_CanFrame->Cells[0][i] = "  ";
        StringGrid_CanFrame->Cells[1][i] = "  ";
        StringGrid_CanFrame->Cells[2][i] = "  ";
        StringGrid_CanFrame->Cells[3][i] = "  ";
        StringGrid_CanFrame->Cells[4][i] = "  ";
        StringGrid_CanFrame->Cells[5][i] = "  ";
    }
    
    CanFrameIndex = 1;
    
    CanFrameGridBusy = 0;
}

void __fastcall TMainForm::Button_ClearClick(TObject *Sender)
{
     ClearCanFrame();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button_StopUpdateClick(TObject *Sender)
{
    if(StopUpdate)
    {
        StopUpdate = 0;
        Button_StopUpdate->Caption = "暂停";
    }
    else
    {
        StopUpdate = 1;
        Button_StopUpdate->Caption = "更新";
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::B_BMSRefreshClick(TObject *Sender)
{
     UINT8 ReadLength = 0u;
     UINT16 u16ReadData = 0u;
     UINT8 u8ReadData = 0u;
     UINT32 u32ReadData = 0u;
     
     ReadLength = 1u;
     u8ReadData = 0;
     ReadOdMap(0x6000 , 0, &ReadLength, (uint8_t *)&u8ReadData);
     Edit_6000->Text = u8ReadData;
     
     ReadLength = 1u;
     u8ReadData = 0;
     ReadOdMap(0x6001 , 0, &ReadLength, (uint8_t *)&u8ReadData);
     Edit_6001->Text = u8ReadData;
     
     ReadLength = 2u;
     u16ReadData = 0;
     ReadOdMap(0x6010 , 0, &ReadLength, (uint8_t *)&u16ReadData);
     Edit_6010->Text = (INT16)u16ReadData * 0.125f;
     
     ReadLength = 4u;
     u32ReadData = 0;
     ReadOdMap(0x6060 , 0, &ReadLength, (uint8_t *)&u32ReadData);
     Edit_6060->Text = u32ReadData / 1024.0f;
     
     ReadLength = 2u;
     u16ReadData = 0;
     ReadOdMap(0x6070 , 0, &ReadLength, (uint8_t *)&u16ReadData);
     Edit_6070->Text = u16ReadData / 16.0f;
     
     ReadLength = 1u;
     u8ReadData = 0;
     ReadOdMap(0x6081 , 0, &ReadLength, (uint8_t *)&u8ReadData);
     Edit_6081->Text = u8ReadData;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button_WriteClick(TObject *Sender)
{
    UINT8 ReadLength = 0u;
    UINT16 u16ReadData = 0u;
    UINT8 u8ReadData = 0u;
    UINT32 u32ReadData = 0u;
    
    ReadLength = 1u;
    u8ReadData = 0;
    ReadOdMap(0x6000 , 0, &ReadLength, (uint8_t *)&u8ReadData);
    
    if(Edit_6000->Text.ToInt() != u8ReadData)
    {
        u8ReadData = Edit_6000->Text.ToInt();
        WriteOdMap(0x6000 , 0, ReadLength, (uint8_t *)&u8ReadData);
    }
    
    
    ReadLength = 1u;
    u8ReadData = 0;
    ReadOdMap(0x6001 , 0, &ReadLength, (uint8_t *)&u8ReadData);
    if(Edit_6001->Text.ToInt() != u8ReadData)
    {
        u8ReadData = Edit_6001->Text.ToInt();
        WriteOdMap(0x6001 , 0, ReadLength, (uint8_t *)&u8ReadData);
    }
    
    ReadLength = 2u;
    u16ReadData = 0;
    ReadOdMap(0x6010 , 0, &ReadLength, (uint8_t *)&u16ReadData);
    u16ReadData = u16ReadData / 8;
    if(Edit_6010->Text.ToInt() != u16ReadData)
    {
        u16ReadData = (INT16)(Edit_6010->Text.ToInt() * 8);
        WriteOdMap(0x6010 , 0, ReadLength, (uint8_t *)&u16ReadData);
    }
    
    ReadLength = 4u;
    u32ReadData = 0;
    ReadOdMap(0x6060 , 0, &ReadLength, (uint8_t *)&u32ReadData);
    u32ReadData = u32ReadData / 1024;
    if(Edit_6060->Text.ToInt() != u32ReadData)
    {
        u32ReadData = Edit_6060->Text.ToInt() * 1024;
        WriteOdMap(0x6060 , 0, ReadLength, (uint8_t *)&u32ReadData);
    }
    
    ReadLength = 2u;
    u16ReadData = 0;
    ReadOdMap(0x6070 , 0, &ReadLength, (uint8_t *)&u16ReadData);
    u16ReadData = u16ReadData / 16;
    if(Edit_6070->Text.ToInt() != u16ReadData)
    {
        u16ReadData = Edit_6070->Text.ToInt() * 16;
        WriteOdMap(0x6070 , 0, ReadLength, (uint8_t *)&u16ReadData);
    }
    
    ReadLength = 1u;
    u8ReadData = 0;
    ReadOdMap(0x6081 , 0, &ReadLength, (uint8_t *)&u8ReadData);
    if(Edit_6081->Text.ToInt() != u8ReadData)
    {
        u8ReadData = Edit_6081->Text.ToInt();
        WriteOdMap(0x6081 , 0, ReadLength, (uint8_t *)&u8ReadData);
    }
}
//---------------------------------------------------------------------------

