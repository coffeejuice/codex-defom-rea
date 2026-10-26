
char FUN_1410db030(undefined8 param_1,longlong param_2,int param_3)

{
  char cVar1;
  undefined8 uVar2;
  char cStack_88;
  undefined1 auStack_80 [8];
  int iStack_78;
  undefined1 auStack_70 [8];
  char cStack_68;
  undefined1 auStack_60 [8];
  char cStack_58;
  undefined1 auStack_50 [8];
  undefined1 auStack_48 [8];
  undefined1 auStack_40 [8];
  undefined1 auStack_38 [8];
  undefined1 auStack_30 [8];
  undefined1 auStack_28 [8];
  char cStack_20;
  uint uStack_1c;
  undefined8 uStack_18;
  undefined4 uStack_10;
  
  uStack_18 = 0xfffffffffffffffe;
  uStack_1c = 0;
  FUN_1400653e0(auStack_60);
  FUN_1400653e0(auStack_80);
  __0QString__QEAA_XZ(auStack_70);
  if (param_3 == 0) {
    cStack_58 = '\0';
    __1QString__QEAA_XZ(auStack_70);
    FUN_1400048d0(auStack_80);
    FUN_1400048d0(auStack_60);
    return cStack_58;
  }
  cStack_68 = '\0';
  for (iStack_78 = 0; iStack_78 < param_3; iStack_78 = iStack_78 + 1) {
    if (*(int *)(param_2 + 0x38 + (longlong)iStack_78 * 0x40) != 0) {
      cStack_68 = '\x01';
      break;
    }
  }
  uVar2 = _setNum_QString__QEAAAEAV1_HH_Z(auStack_70,param_3,10);
  FUN_140cca3c0(auStack_60,uVar2);
  __0QString__QEAA_PEBD_Z(auStack_50,&UNK_142c143dc);
  FUN_140cca3c0(auStack_60,auStack_50);
  __1QString__QEAA_XZ(auStack_50);
  for (iStack_78 = 0; iStack_78 < param_3; iStack_78 = iStack_78 + 1) {
    uVar2 = _setNum_QString__QEAAAEAV1_HH_Z
                      (auStack_70,*(undefined4 *)(param_2 + (longlong)iStack_78 * 0x40),10);
    FUN_140cca3c0(auStack_60,uVar2);
    __0QString__QEAA_PEBD_Z(auStack_48,&UNK_142c143e0);
    FUN_140cca3c0(auStack_60,auStack_48);
    __1QString__QEAA_XZ(auStack_48);
    uVar2 = _setNum_QString__QEAAAEAV1_NDH_Z
                      (auStack_70,*(undefined8 *)(param_2 + 8 + (longlong)iStack_78 * 0x40),0x67,6);
    FUN_140cca3c0(auStack_60,uVar2);
    uVar2 = _setNum_QString__QEAAAEAV1_NDH_Z
                      (auStack_70,*(undefined8 *)(param_2 + 0x10 + (longlong)iStack_78 * 0x40),0x67,
                       6);
    FUN_140cca3c0(auStack_60,uVar2);
    uVar2 = _setNum_QString__QEAAAEAV1_HH_Z
                      (auStack_70,*(undefined4 *)(param_2 + 0x18 + (longlong)iStack_78 * 0x40),10);
    FUN_140cca3c0(auStack_60,uVar2);
    uVar2 = _setNum_QString__QEAAAEAV1_NDH_Z
                      (auStack_70,*(undefined8 *)(param_2 + 0x20 + (longlong)iStack_78 * 0x40),0x67,
                       6);
    FUN_140cca3c0(auStack_60,uVar2);
    __0QString__QEAA_PEBD_Z(auStack_40,&UNK_142c143e4);
    FUN_140cca3c0(auStack_60,auStack_40);
    __1QString__QEAA_XZ(auStack_40);
    uVar2 = _setNum_QString__QEAAAEAV1_NDH_Z
                      (auStack_70,*(undefined8 *)(param_2 + 0x28 + (longlong)iStack_78 * 0x40),0x67,
                       6);
    FUN_140cca3c0(auStack_60,uVar2);
    uVar2 = _setNum_QString__QEAAAEAV1_NDH_Z
                      (auStack_70,*(undefined8 *)(param_2 + 0x30 + (longlong)iStack_78 * 0x40),0x67,
                       6);
    FUN_140cca3c0(auStack_60,uVar2);
    __0QString__QEAA_PEBD_Z(auStack_38,&UNK_142c143e8);
    FUN_140cca3c0(auStack_60,auStack_38);
    __1QString__QEAA_XZ(auStack_38);
    uVar2 = _setNum_QString__QEAAAEAV1_HH_Z
                      (auStack_70,*(undefined4 *)(param_2 + 0x38 + (longlong)iStack_78 * 0x40),10);
    FUN_140cca3c0(auStack_80,uVar2);
  }
  __0QString__QEAA_PEBD_Z(auStack_30,&UNK_142c143f0);
  cStack_88 = FUN_1410da9e0(param_1,auStack_30,auStack_60);
  __1QString__QEAA_XZ(auStack_30);
  if (cStack_68 == '\0') {
    FUN_1407fbc10(&UNK_142c14410);
    goto code_r0x0001410db4a5;
  }
  if (cStack_88 == '\0') {
code_r0x0001410db45c:
    uStack_10 = 0;
  }
  else {
    __0QString__QEAA_PEBD_Z(auStack_28,&UNK_142c14400);
    uStack_1c = uStack_1c | 1;
    cVar1 = FUN_1410da9e0(param_1,auStack_28,auStack_80);
    if (cVar1 == '\0') goto code_r0x0001410db45c;
    uStack_10 = 1;
  }
  cStack_88 = (char)uStack_10;
  if ((uStack_1c & 1) != 0) {
    uStack_1c = uStack_1c & 0xfffffffe;
    __1QString__QEAA_XZ(auStack_28);
  }
code_r0x0001410db4a5:
  cStack_20 = cStack_88;
  __1QString__QEAA_XZ(auStack_70);
  FUN_1400048d0(auStack_80);
  FUN_1400048d0(auStack_60);
  return cStack_20;
}

