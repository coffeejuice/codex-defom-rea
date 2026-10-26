
void FUN_140eb6da0(longlong *param_1,int param_2)

{
  longlong lVar1;
  char cVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined8 uStack_88;
  int iStack_80;
  int iStack_7c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  longlong alStack_60 [4];
  undefined4 auStack_40 [2];
  undefined1 auStack_38 [8];
  undefined1 auStack_30 [8];
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  uStack_28 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_38);
  uStack_64 = 0;
  FUN_140eb6210(param_1,auStack_40,&uStack_68);
  if (param_2 == 0) {
    uStack_88 = *(undefined8 *)(param_1[0x28] + 0x6a8);
    alStack_60[0] = *(longlong *)(param_1[0x28] + 0x6d0);
    alStack_60[1] = *(undefined8 *)(param_1[0x28] + 0x6e0);
    cVar2 = FUN_140130ea0(param_1);
    if (cVar2 == '\0') {
      alStack_60[2] = *(undefined8 *)(param_1[0x28] + 0x6f0);
    }
    else {
      alStack_60[2] = 0;
    }
    uVar4 = (**(code **)(*param_1 + 0x198))(param_1);
    alStack_60[3] = FUN_140c7ae80(uVar4,auStack_40[0]);
    if (alStack_60[3] == 0) {
      __1QString__QEAA_XZ(auStack_38);
      return;
    }
  }
  else {
    uStack_88 = *(undefined8 *)(param_1[0x28] + 0x740);
    alStack_60[0] = *(longlong *)(param_1[0x28] + 0x768);
    alStack_60[1] = *(undefined8 *)(param_1[0x28] + 0x778);
    cVar2 = FUN_140130ea0(param_1);
    if (cVar2 == '\0') {
      alStack_60[2] = *(undefined8 *)(param_1[0x28] + 0x788);
    }
    else {
      alStack_60[2] = 0;
    }
    uVar4 = (**(code **)(*param_1 + 0x198))(param_1);
    alStack_60[3] = FUN_140c7ae80(uVar4,uStack_68);
    if (alStack_60[3] == 0) {
      __1QString__QEAA_XZ(auStack_38);
      return;
    }
  }
  lVar1 = alStack_60[3];
  if (param_2 == 0) {
    cVar2 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x6a0));
    if (cVar2 == '\0') {
      cVar2 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x6b0));
      if (cVar2 != '\0') {
        uStack_64 = 1;
      }
    }
    else {
      uStack_64 = 2;
    }
  }
  else {
    cVar2 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x738));
    if (cVar2 == '\0') {
      cVar2 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x748));
      if (cVar2 != '\0') {
        uStack_64 = 1;
      }
    }
    else {
      uStack_64 = 2;
    }
  }
  *(undefined4 *)(lVar1 + 0x1ec8) = uStack_64;
  cVar2 = _isEnabled_QWidget__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x6a0));
  if ((cVar2 == '\0') &&
     (cVar2 = _isEnabled_QWidget__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0x738)), cVar2 == '\0'))
  {
    *(undefined4 *)(lVar1 + 0x1ee8) = 0;
  }
  else {
    uVar3 = _value_QSpinBox__QEBAHXZ(uStack_88);
    *(undefined4 *)(lVar1 + 0x1ee8) = uVar3;
  }
  cVar2 = FUN_140130ea0(param_1);
  if (cVar2 == '\0') {
    iStack_80 = 3;
  }
  else {
    iStack_80 = 2;
  }
  for (iStack_7c = 0; iStack_7c < iStack_80; iStack_7c = iStack_7c + 1) {
    uStack_20 = _text_QLineEdit__QEBA_AVQString__XZ(alStack_60[iStack_7c],auStack_30);
    uStack_18 = uStack_20;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_38,uStack_20);
    __1QString__QEAA_XZ(auStack_30);
    uVar4 = _toDouble_QString__QEBANPEA_N_Z(auStack_38,0);
    *(undefined8 *)(lVar1 + 0x1ed0 + (longlong)iStack_7c * 8) = uVar4;
  }
  __1QString__QEAA_XZ(auStack_38);
  return;
}

