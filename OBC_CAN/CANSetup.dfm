object CanSetupDlg: TCanSetupDlg
  Left = 227
  Top = 108
  BorderStyle = bsDialog
  Caption = 'CAN Setup'
  ClientHeight = 179
  ClientWidth = 384
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poMainFormCenter
  OnShow = CanSetupInit
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 281
    Height = 161
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 18
    Top = 41
    Width = 32
    Height = 13
    Caption = 'Device'
  end
  object Label2: TLabel
    Left = 192
    Top = 41
    Width = 39
    Height = 13
    Caption = 'Channel'
  end
  object Label3: TLabel
    Left = 147
    Top = 113
    Width = 47
    Height = 13
    Caption = 'BaudRate'
  end
  object Label4: TLabel
    Left = 19
    Top = 113
    Width = 63
    Height = 13
    Caption = 'Device Index'
  end
  object OKBtn: TButton
    Left = 301
    Top = 11
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 301
    Top = 41
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object CanToolSelect: TComboBox
    Left = 64
    Top = 38
    Width = 113
    Height = 21
    TabOrder = 2
    Text = 'ZLG USBCAN1'
    Items.Strings = (
      'ZLG USBCAN1'
      'ZLG USBCAN2'
      'ZLG USBCAN_E_U'
      'ZLG USBCAN_2E_U'
      'ZHCX USBCAN1'
      'ZHCX USBCAN2'
      'ZHCX USBCAN_E_U'
      'ZHCX USBCAN_2E_U')
  end
  object ChannelSet: TComboBox
    Left = 240
    Top = 38
    Width = 41
    Height = 21
    TabOrder = 3
    Text = '0'
    Items.Strings = (
      '0'
      '1')
  end
  object BaudRate: TComboBox
    Left = 200
    Top = 110
    Width = 81
    Height = 21
    TabOrder = 4
    Text = '125kbps'
    Items.Strings = (
      '125kpbs'
      '250kpbs'
      '500kpbs'
      '1000kpbs')
  end
  object DeviceIndex: TComboBox
    Left = 88
    Top = 110
    Width = 34
    Height = 21
    TabOrder = 5
    Text = '0'
    Items.Strings = (
      '0'
      '1'
      '2'
      '3'
      '4')
  end
end
