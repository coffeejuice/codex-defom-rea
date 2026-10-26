
void FUN_1408bfe60(longlong param_1)

{
  char cVar1;
  int iStack_28;
  undefined8 uStack_20;
  longlong lStack_18;
  
  uStack_20 = 0;
  uStack_20 = FUN_1408dfc30(param_1 + 0x750);
  lStack_18 = 0;
  for (iStack_28 = 0; iStack_28 < *(int *)(param_1 + 0x1378); iStack_28 = iStack_28 + 1) {
    lStack_18 = FUN_1408302a0(param_1,iStack_28 + 1);
    if (((lStack_18 != 0) && (cVar1 = FUN_140834fa0(lStack_18), cVar1 != '\0')) &&
       (cVar1 = FUN_140892370(lStack_18 + 0x2160), cVar1 != '\0')) {
      FUN_140874630(lStack_18,&uStack_20);
    }
  }
  return;
}

