
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ecf980(longlong *param_1,int param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if ((int)param_1[0x2c] == 0) {
    lVar1 = FUN_140130f90(param_1);
    if (param_2 == 0) {
      if (0 < *(int *)(lVar1 + 0xa74)) {
        FUN_14089b1f0(lVar1 + 0xa58);
      }
      *(undefined4 *)(lVar1 + 0xa74) = 0;
      *(undefined4 *)(lVar1 + 0xa98) = 0;
    }
    else if (param_2 == 2) {
      if (0 < *(int *)(lVar1 + 0xa74)) {
        FUN_14089b1f0(lVar1 + 0xa58);
      }
      *(undefined4 *)(lVar1 + 0xa74) = 1;
      *(undefined4 *)(lVar1 + 0xa98) = 1;
      FUN_14089b840(lVar1 + 0xa58);
      **(undefined8 **)(lVar1 + 0xa80) = _DAT_1421249e0;
      **(undefined8 **)(lVar1 + 0xaa0) = 0;
    }
    uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
    uVar2 = FUN_140c7acd0(uVar2);
    FUN_14082aaa0(uVar2,0x22);
  }
  return;
}

