
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140eb5370(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  longlong lStack_18;
  
  lVar1 = FUN_14146eabc(0x9dc);
  if (lVar1 == 0) {
    lStack_18 = 0;
  }
  else {
    lStack_18 = FUN_1402d14f0(lVar1);
  }
  _setForegroundRole_QWidget__QEAAXW4ColorRole_QPalette___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x700),0,param_3,param_4,
             (int)(*(float *)(lStack_18 + 0x334) * _UNK_1421ebb18),lStack_18,
             (int)(*(float *)(lStack_18 + 0x338) * _UNK_1421ebb18));
  _setForegroundRole_QWidget__QEAAXW4ColorRole_QPalette___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x798),0,param_3,param_4,
             (int)(*(float *)(lStack_18 + 0x344) * _UNK_1421ebb18),lStack_18,
             (int)(*(float *)(lStack_18 + 0x348) * _UNK_1421ebb18));
  if (lStack_18 != 0) {
    FUN_140f346d0(lStack_18,1);
  }
  return;
}

