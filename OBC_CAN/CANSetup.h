//----------------------------------------------------------------------------
#ifndef CANSetupH
#define CANSetupH
//----------------------------------------------------------------------------
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Graphics.hpp>
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <Winapi.Windows.hpp>
#include <System.hpp>
#include "CanDriver.h"
//----------------------------------------------------------------------------
class TCanSetupDlg : public TForm
{
__published:
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
	TComboBox *CanToolSelect;
	TLabel *Label1;
	TComboBox *ChannelSet;
	TLabel *Label2;
	TComboBox *BaudRate;
	TLabel *Label3;
	TComboBox *DeviceIndex;
	TLabel *Label4;
	void __fastcall OKBtnClick(TObject *Sender);

	void __fastcall CancelBtnClick(TObject *Sender);
	void __fastcall CanSetupInit(TObject *Sender);
private:

public:
    USHORT DeviceReady;
	virtual __fastcall TCanSetupDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TCanSetupDlg *CanSetupDlg;
//----------------------------------------------------------------------------
#endif    
