
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ecb340(longlong *param_1,char param_2)

{
  undefined8 uVar1;
  undefined1 auStack_c48 [48];
  undefined1 auStack_c18 [3024];
  longlong *plStack_48;
  undefined4 uStack_40;
  longlong lStack_38;
  undefined4 uStack_30;
  undefined1 auStack_28 [8];
  undefined8 uStack_20;
  ulonglong uStack_18;
  
  uStack_20 = 0xfffffffffffffffe;
  uStack_18 = uRam0000000143b5e030 ^ (ulonglong)auStack_c48;
  uStack_30 = _currentRow_QListWidget__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 8));
  __0QString__QEAA_XZ(auStack_28);
  lStack_38 = FUN_140130f90(param_1);
  if (lStack_38 == 0) {
    __1QString__QEAA_XZ(auStack_28);
  }
  else {
    if (param_2 != '\0') {
      FUN_1408d3f50(lStack_38);
      func_0x0001408d45f0(lStack_38);
      FUN_1408d47a0(lStack_38);
      FUN_1408d5800(lStack_38);
      FUN_1408d6800(lStack_38);
      FUN_1408d6eb0(lStack_38);
    }
    plStack_48 = (longlong *)func_0x0001402c8090(0x143d766b8);
    uStack_40 = (**(code **)(*plStack_48 + 0x260))(plStack_48);
    FUN_1408d3bc0(auStack_c18);
    uVar1 = (**(code **)(*param_1 + 0x198))(param_1);
    uVar1 = FUN_140c7acd0(uVar1);
    FUN_1408d3cd0(auStack_c18,uVar1);
    FUN_1408df7b0(auStack_c18,lStack_38);
    FUN_140eb9630(param_1,lStack_38);
    FUN_140ebb110(param_1,lStack_38);
    FUN_14089ad60(param_1[0x1d],lStack_38 + 0x1e8);
    FUN_140ebb950(param_1,lStack_38);
    FUN_140ec19f0(param_1,lStack_38);
    FUN_14089ad60(param_1[0x19],lStack_38 + 0x3c8);
    FUN_14089ad60(param_1[0x1a],lStack_38 + 0x4c0);
    FUN_14089ad60(param_1[0x1b],lStack_38 + 0x538);
    FUN_14089ad60(param_1[0x1c],lStack_38 + 0x5b0);
    FUN_14089ad60(param_1[0x1e],lStack_38 + 0x9e0);
    *(undefined4 *)(param_1 + 0x27) = *(undefined4 *)(lStack_38 + 0x9dc);
    FUN_140ec3fa0(param_1,lStack_38);
    FUN_140ec5bc0(param_1,lStack_38);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b15124,_UNK_142b1b5b0);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x14c0),auStack_28);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_28,&UNK_142b1512c,_UNK_142b1b5a8);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x14e0),auStack_28);
    FUN_1408df7b0(lStack_38,auStack_c18);
    FUN_1408d3d70(auStack_c18);
    __1QString__QEAA_XZ(auStack_28);
  }
  func_0x00014146df10(uStack_18 ^ (ulonglong)auStack_c48);
  return;
}

