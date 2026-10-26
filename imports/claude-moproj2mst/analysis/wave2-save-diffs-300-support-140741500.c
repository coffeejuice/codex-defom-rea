
void FUN_140741500(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  int *piVar3;
  undefined4 auStack_10 [4];
  
  *(undefined4 *)(param_1 + 0xb8) = 0;
  FUN_1414036e0(param_1);
  uVar2 = func_0x00014140c710(param_1);
  cVar1 = FUN_140307410(uVar2);
  if (cVar1 == '\0') {
    piVar3 = (int *)FUN_14014b5b0(uVar2,0);
    if (*piVar3 != 0) goto code_r0x000140741574;
  }
  FUN_1400424b0(uVar2);
  auStack_10[0] = 0;
  FUN_140025100(uVar2,auStack_10);
code_r0x000140741574:
  *(undefined1 *)(param_1 + 0xa8) = 1;
  return;
}

