
void FUN_140eb6210(longlong param_1,undefined4 *param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined1 auStack_48 [8];
  undefined1 auStack_40 [8];
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  uStack_38 = 0xfffffffffffffffe;
  uStack_30 = _currentText_QComboBox__QEBA_AVQString__XZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x708),auStack_48);
  uStack_28 = uStack_30;
  uVar1 = FUN_140ec9980(param_1,uStack_30);
  *param_2 = uVar1;
  __1QString__QEAA_XZ(auStack_48);
  uStack_20 = _currentText_QComboBox__QEBA_AVQString__XZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x7a0),auStack_40);
  uStack_18 = uStack_20;
  uVar1 = FUN_140ec9980(param_1,uStack_20);
  *param_3 = uVar1;
  __1QString__QEAA_XZ(auStack_40);
  return;
}

