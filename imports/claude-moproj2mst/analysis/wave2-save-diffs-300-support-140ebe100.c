
void FUN_140ebe100(longlong *param_1,int param_2)

{
  char cVar1;
  undefined8 uVar2;
  longlong lStack_78;
  uint auStack_70 [2];
  longlong lStack_68;
  uint auStack_60 [2];
  undefined1 auStack_58 [8];
  undefined1 auStack_50 [8];
  undefined1 auStack_48 [8];
  undefined1 auStack_40 [8];
  undefined1 auStack_38 [8];
  undefined1 auStack_30 [8];
  undefined1 auStack_28 [8];
  undefined8 uStack_20;
  int iStack_18;
  int iStack_14;
  int iStack_10;
  
  uStack_20 = 0xfffffffffffffffe;
  lStack_68 = 0;
  lStack_78 = 0;
  __0QString__QEAA_XZ(auStack_58);
  FUN_140eb6210(param_1,auStack_60,auStack_70);
  if ((param_2 != 0) || ((int)auStack_60[0] < 1)) {
    if ((int)auStack_70[0] < 1) goto code_r0x000140ebe7e5;
    uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
    lStack_68 = FUN_140c7ae80(uVar2,auStack_70[0]);
    if (lStack_68 == 0) {
      FUN_141472790(&UNK_142b14970);
      __1QString__QEAA_XZ(auStack_58);
      return;
    }
    if (lStack_68 != 0) {
      lStack_78 = lStack_68 + 0x1ea0;
    }
    if (lStack_78 == 0) {
      iStack_10 = iRam0000000000000028;
      if (iRam0000000000000028 == 0) {
        _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x738),1);
      }
      else if (iRam0000000000000028 == 1) {
        _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x748),1);
      }
      else {
        FUN_141472790(&UNK_142b14a20);
      }
      _setValue_QSpinBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x740),0);
      __0QString__QEAA_PEBD_Z(auStack_38,&UNK_142b13d54);
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x768),auStack_38);
      __1QString__QEAA_XZ(auStack_38);
      __0QString__QEAA_PEBD_Z(auStack_30,&UNK_142b14a68);
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x778),auStack_30);
      __1QString__QEAA_XZ(auStack_30);
      cVar1 = FUN_140130ee0(param_1);
      if (cVar1 != '\0') {
        __0QString__QEAA_PEBD_Z(auStack_28,&UNK_142b14a6c);
        _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x788),auStack_28);
        __1QString__QEAA_XZ(auStack_28);
      }
      goto code_r0x000140ebe7e5;
    }
    iStack_14 = *(int *)(lStack_78 + 0x28);
    if (iStack_14 == 0) {
code_r0x000140ebe526:
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x738),1);
    }
    else if (iStack_14 == 1) {
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x748),1);
    }
    else {
      if (iStack_14 == 2) goto code_r0x000140ebe526;
      FUN_141472790(&UNK_142b149c0);
    }
    if (*(int *)(lStack_78 + 0x28) == 2) {
      _setValue_QSpinBox__QEAAXH_Z
                (*(undefined8 *)(param_1[0x28] + 0x740),*(undefined4 *)(lStack_78 + 0x48));
    }
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_58,&UNK_142b14a08,*(undefined8 *)(lStack_78 + 0x30));
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x768),auStack_58);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_58,&UNK_142b14a10,*(undefined8 *)(lStack_78 + 0x38));
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x778),auStack_58);
    cVar1 = FUN_140130ee0(param_1);
    if (cVar1 != '\0') {
      _sprintf_QString__QEAAAEAV1_PEBDZZ
                (auStack_58,&UNK_142b14a18,*(undefined8 *)(lStack_78 + 0x40));
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x788),auStack_58);
    }
    goto code_r0x000140ebe7e5;
  }
  uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
  lStack_68 = FUN_140c7ae80(uVar2,auStack_60[0]);
  if (lStack_68 == 0) {
    FUN_141472790(&UNK_142b148c0);
    __1QString__QEAA_XZ(auStack_58);
    return;
  }
  if (lStack_68 != 0) {
    lStack_78 = lStack_68 + 0x1ea0;
  }
  if (lStack_78 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x6a0),1);
    _setValue_QSpinBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x6a8),0);
    __0QString__QEAA_PEBD_Z(auStack_50,&UNK_142b13854);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x6d0),auStack_50);
    __1QString__QEAA_XZ(auStack_50);
    __0QString__QEAA_PEBD_Z(auStack_48,&UNK_142b13864);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x6e0),auStack_48);
    __1QString__QEAA_XZ(auStack_48);
    cVar1 = FUN_140130ee0(param_1);
    if (cVar1 != '\0') {
      __0QString__QEAA_PEBD_Z(auStack_40,&UNK_142b138c4);
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x6f0),auStack_40);
      __1QString__QEAA_XZ(auStack_40);
    }
    goto code_r0x000140ebe7e5;
  }
  iStack_18 = *(int *)(lStack_78 + 0x28);
  if (iStack_18 == 0) {
code_r0x000140ebe209:
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x6a0),1);
  }
  else if (iStack_18 == 1) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x6b0),1);
  }
  else {
    if (iStack_18 == 2) goto code_r0x000140ebe209;
    FUN_141472790(&UNK_142b14910);
  }
  if (*(int *)(lStack_78 + 0x28) == 2) {
    _setValue_QSpinBox__QEAAXH_Z
              (*(undefined8 *)(param_1[0x28] + 0x6a8),*(undefined4 *)(lStack_78 + 0x48));
  }
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_58,&UNK_142b14958,*(undefined8 *)(lStack_78 + 0x30));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x6d0),auStack_58);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_58,&UNK_142b14960,*(undefined8 *)(lStack_78 + 0x38));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x6e0),auStack_58);
  cVar1 = FUN_140130ee0(param_1);
  if (cVar1 != '\0') {
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_58,&UNK_142b14968,*(undefined8 *)(lStack_78 + 0x40));
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x6f0),auStack_58);
  }
code_r0x000140ebe7e5:
  __1QString__QEAA_XZ(auStack_58);
  return;
}

