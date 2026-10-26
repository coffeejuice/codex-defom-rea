
void FUN_140eb71e0(longlong *param_1)

{
  char cVar1;
  undefined8 uVar2;
  int iStack_a8;
  int iStack_a4;
  longlong alStack_a0 [4];
  int aiStack_80 [2];
  undefined1 auStack_78 [8];
  longlong alStack_70 [4];
  longlong lStack_50;
  undefined1 auStack_48 [8];
  undefined1 auStack_40 [8];
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  uStack_38 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_78);
  FUN_140eb62e0(param_1,aiStack_80);
  uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
  alStack_a0[3] = FUN_140c7ae80(uVar2,aiStack_80[0]);
  if (alStack_a0[3] == 0) {
    __1QString__QEAA_XZ(auStack_78);
  }
  else {
    alStack_a0[3] = alStack_a0[3] + 0x2160;
    if ((*(int *)((longlong)param_1 + 0x11c) != 0) &&
       (*(int *)((longlong)param_1 + 0x11c) != aiStack_80[0])) {
      uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
      lStack_50 = FUN_140c7ae80(uVar2,*(undefined4 *)((longlong)param_1 + 0x11c));
      alStack_70[3] = lStack_50 + 0x2160;
      cVar1 = FUN_140130ea0(param_1);
      if (cVar1 == '\0') {
        iStack_a8 = 3;
      }
      else {
        iStack_a8 = 2;
      }
      for (iStack_a4 = 0; iStack_a4 < iStack_a8; iStack_a4 = iStack_a4 + 1) {
        *(undefined8 *)(alStack_70[3] + 0x888 + (longlong)iStack_a4 * 8) = 0;
        *(undefined8 *)(alStack_70[3] + 0x8a0 + (longlong)iStack_a4 * 8) = 0;
      }
    }
    alStack_a0[0] = *(longlong *)(param_1[0x28] + 0x888);
    alStack_a0[1] = *(undefined8 *)(param_1[0x28] + 0x890);
    cVar1 = FUN_140130ea0(param_1);
    if (cVar1 == '\0') {
      alStack_a0[2] = *(undefined8 *)(param_1[0x28] + 0x898);
    }
    else {
      alStack_a0[2] = 0;
    }
    cVar1 = FUN_140130ea0(param_1);
    if (cVar1 == '\0') {
      iStack_a8 = 3;
    }
    else {
      iStack_a8 = 2;
    }
    for (iStack_a4 = 0; iStack_a4 < iStack_a8; iStack_a4 = iStack_a4 + 1) {
      uStack_30 = _text_QLineEdit__QEBA_AVQString__XZ(alStack_a0[iStack_a4],auStack_48);
      uStack_28 = uStack_30;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_78,uStack_30);
      __1QString__QEAA_XZ(auStack_48);
      uVar2 = _toDouble_QString__QEBANPEA_N_Z(auStack_78,0);
      *(undefined8 *)(alStack_a0[3] + 0x888 + (longlong)iStack_a4 * 8) = uVar2;
    }
    alStack_70[0] = *(longlong *)(param_1[0x28] + 0x848);
    alStack_70[1] = *(undefined8 *)(param_1[0x28] + 0x850);
    cVar1 = FUN_140130ea0(param_1);
    if (cVar1 == '\0') {
      alStack_70[2] = *(undefined8 *)(param_1[0x28] + 0x858);
    }
    else {
      alStack_70[2] = 0;
    }
    cVar1 = FUN_140130ea0(param_1);
    if (cVar1 == '\0') {
      iStack_a8 = 3;
    }
    else {
      iStack_a8 = 2;
    }
    for (iStack_a4 = 0; iStack_a4 < iStack_a8; iStack_a4 = iStack_a4 + 1) {
      uStack_20 = _text_QLineEdit__QEBA_AVQString__XZ(alStack_70[iStack_a4],auStack_40);
      uStack_18 = uStack_20;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_78,uStack_20);
      __1QString__QEAA_XZ(auStack_40);
      uVar2 = _toDouble_QString__QEBANPEA_N_Z(auStack_78,0);
      *(undefined8 *)(alStack_a0[3] + 0x8a0 + (longlong)iStack_a4 * 8) = uVar2;
    }
    __1QString__QEAA_XZ(auStack_78);
  }
  return;
}

