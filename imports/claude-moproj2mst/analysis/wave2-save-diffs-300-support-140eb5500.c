
void FUN_140eb5500(longlong *param_1)

{
  undefined8 uVar1;
  undefined4 uStack_20;
  uint uStack_1c;
  int iStack_18;
  undefined4 auStack_14 [5];
  
  auStack_14[0] = 0;
  uStack_20 = 0;
  (**(code **)(*param_1 + 0x198))(param_1);
  FUN_140eb6210(param_1,auStack_14,&uStack_20);
  iStack_18 = _currentRow_QListWidget__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 8));
  iStack_18 = iStack_18 + 1;
  uStack_1c = *(int *)((longlong)param_1 + 0xbc) + 1;
  if ((iStack_18 == 4) && ((uStack_1c & 2) != 0)) {
    FUN_140ecd430(param_1,1);
    FUN_140ecd2f0(param_1,1);
    FUN_140ece670(param_1,1,1);
    FUN_140ece670(param_1,2,0);
  }
  else {
    FUN_140ecd430(param_1,1);
    FUN_140ecd2f0(param_1,0);
    uVar1 = (**(code **)(*param_1 + 0x1b0))(param_1);
    FUN_14120b2c0(uVar1);
    uVar1 = (**(code **)(*param_1 + 0x198))(param_1);
    FUN_140c83ca0(uVar1,0,0,1);
  }
  return;
}

