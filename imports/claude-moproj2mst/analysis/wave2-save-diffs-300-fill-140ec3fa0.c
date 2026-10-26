
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ec3fa0(longlong param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 uVar4;
  int iStack_34;
  undefined1 auStack_28 [8];
  undefined8 uStack_20;
  undefined1 uStack_18;
  
  uStack_20 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_28);
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 == '\0') {
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xda0));
  }
  else {
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xda0));
  }
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcd0),1);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14c70,*(undefined8 *)(param_2 + 0x3d0));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd00),auStack_28);
  FUN_140eccb50(param_1,*(int *)(param_2 + 0x3cc) != 0);
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd38),1);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14c78,*(undefined8 *)(param_2 + 0x4c8));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd60),auStack_28);
  uVar4 = *(int *)(param_2 + 0x4c4) != 0;
  FUN_140eccc90(param_1,uVar4);
  iVar2 = func_0x0001412e6910();
  if (iVar2 == 0) {
    iVar2 = FUN_1412fae90(0);
    if (iVar2 == 0) {
      _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xdd8),0);
    }
  }
  else {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xdd8),1);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14c80,*(undefined8 *)(param_2 + 0x540));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe18),auStack_28);
    uVar4 = *(int *)(param_2 + 0x53c) != 0;
    FUN_140eccdd0(param_1,uVar4);
  }
  iVar2 = func_0x0001412e6910();
  if (iVar2 == 0) {
    iVar2 = FUN_1412fae90(0);
    if (iVar2 == 0) {
      _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe40),0);
    }
  }
  else {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe40),1);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14c88,*(undefined8 *)(param_2 + 0x5b8));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe60),auStack_28);
    if (*(int *)(param_2 + 0x5c0) == 3) {
      uVar4 = 1;
    }
    else if (*(int *)(param_2 + 0x5c0) == 0xc) {
      uVar4 = 2;
    }
    else if (*(int *)(param_2 + 0x5c0) == 1) {
      uVar4 = 3;
    }
    else if (*(int *)(param_2 + 0x5b4) == 0) {
      uVar4 = 0;
    }
    FUN_140eccf10(param_1,uVar4);
  }
  cVar1 = cRam00000001446ee586;
  if (cRam00000001446ee586 == '\0') {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xed0),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xec8),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xed8),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xee8),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xee0),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xef0),0);
  }
  else {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xed0),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xec8),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xed8),1);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14c90,*(undefined8 *)(param_2 + 0x628));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xed0),auStack_28);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xee8),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xee0),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xef0),1);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14c98,*(undefined8 *)(param_2 + 0x630));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xee8),auStack_28);
  }
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf00),1);
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xef8),1);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14ca0,*(undefined8 *)(param_2 + 0x638));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf00),auStack_28);
  if ((cVar1 == '\0') ||
     (cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xa8)), cVar1 == '\0')) {
    uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),
                       *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xeb8));
    _setTabEnabled_QTabWidget__QEAAXH_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),uVar3,0);
  }
  else {
    uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),
                       *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xeb8));
    _setTabEnabled_QTabWidget__QEAAXH_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),uVar3,1);
    cVar1 = FUN_140130ea0(param_1);
    if (cVar1 == '\0') {
      if (0 < *(int *)(param_2 + 0x118)) {
        _setCurrentIndex_QComboBox__QEAAXH_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8),
                   *(int *)(param_2 + 0x118) + -1);
      }
    }
    else if (0 < *(int *)(param_2 + 0x118)) {
      _setCurrentIndex_QComboBox__QEAAXH_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8),
                 *(int *)(param_2 + 0x118) + -1);
    }
  }
  func_0x0001407fb750();
  iVar2 = func_0x0001407fb750();
  uStack_18 = iVar2 == 2;
  uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),
                     *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf10));
  _setTabEnabled_QTabWidget__QEAAXH_N_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),uVar3,uStack_18);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14ca8,*(undefined8 *)(param_2 + 0x3a8));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf40),auStack_28);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14cb0,*(undefined8 *)(param_2 + 0x3b0));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf70),auStack_28);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14cb8,*(undefined8 *)(param_2 + 0x3b8));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf50),auStack_28);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b14cc0,*(undefined8 *)(param_2 + 0x4b8));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf60),auStack_28);
  if (*(int *)(param_2 + 0x9d8) == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd98),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xda0),0);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd98),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xda0),1);
  }
  *(undefined4 *)(param_1 + 0x160) = 0;
  if (0 < *(int *)(param_2 + 0xa74)) {
    for (iStack_34 = 0; iStack_34 < *(int *)(param_2 + 0xa74); iStack_34 = iStack_34 + 1) {
      if (*(double *)(*(longlong *)(param_2 + 0xa80) + (longlong)iStack_34 * 8) == _DAT_1421249e0) {
        *(undefined4 *)(param_1 + 0x160) = 1;
        _setChecked_QAbstractButton__QEAAX_N_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xdb8),1);
        *(undefined4 *)(param_1 + 0x160) = 0;
        break;
      }
    }
  }
  FUN_140ec4970(param_1,param_2);
  _removeTab_QTabWidget__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcb8),4);
  __1QString__QEAA_XZ(auStack_28);
  return;
}

