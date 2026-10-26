
void FUN_1408e00d0(longlong param_1)

{
  int iVar1;
  
  iVar1 = func_0x0001408dfd60(param_1);
  if ((iVar1 != 0) && (iVar1 = func_0x0001408dfd80(param_1), iVar1 != 0)) {
    *(undefined4 *)(param_1 + 0xf4) = 3;
    return;
  }
  iVar1 = func_0x0001408dfd60(param_1);
  if (iVar1 == 0) {
    *(undefined4 *)(param_1 + 0xf4) = 1;
  }
  else {
    *(undefined4 *)(param_1 + 0xf4) = 2;
  }
  return;
}

