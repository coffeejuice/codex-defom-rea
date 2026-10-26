
void FUN_1412ef8b0(longlong param_1,char param_2,char param_3)

{
  byte bVar1;
  int iVar2;
  undefined8 uVar3;
  undefined1 auStack_638 [32];
  ulonglong auStack_618 [2];
  undefined1 auStack_608 [532];
  undefined1 auStack_3f4 [988];
  ulonglong uStack_18;
  
  auStack_618[1] = 0xfffffffffffffffe;
  uStack_18 = uRam0000000143b5e030 ^ (ulonglong)auStack_638;
  if (param_3 != '\0') {
    iVar2 = func_0x0001412e6cb0();
    if (0 < iVar2) {
      FUN_14130b040(auStack_608,iVar2);
      bVar1 = FUN_14130c1a0(auStack_608,0x15);
      *(uint *)(param_1 + 0x3c) = (uint)bVar1;
      bVar1 = FUN_14130c1a0(auStack_608,0x2f);
      *(uint *)(param_1 + 0x40) = (uint)bVar1;
      if ((*(int *)(param_1 + 0x3c) == 0) && (bVar1 == 0)) {
        uVar3 = 0;
      }
      else {
        uVar3 = 1;
      }
      func_0x0001412e6fb0(param_1,uVar3);
      func_0x0001412e6ff0();
      func_0x0001412e7020();
      FUN_14130b180(auStack_608);
    }
  }
  if (*(int *)(param_1 + 0x3c) == 0) {
    FUN_1412ef050(param_1,0xffffffff,0xffffffff,2);
  }
  if (*(int *)(param_1 + 0x40) == 0) {
    FUN_1412ef050(param_1,0xffffffff,0xffffffff,3);
  }
  if (((*(int *)(param_1 + 0x3c) == 0) && (*(int *)(param_1 + 0x40) == 0)) && (param_2 != '\0')) {
    FUN_1412f7e90(auStack_608);
    FUN_141472790(&UNK_142db6ca8,auStack_3f4);
    if (cRam00000001446f0748 != '\0') {
      auStack_618[0] = 0;
      FUN_141306850(auStack_618,&UNK_142db6c78,1);
      FUN_1412f4f60(0x1446f02f0,auStack_618[0],&UNK_142db4b00,0x15b8);
      FUN_1412f51b0(0x1446efe90,auStack_618[0] & 0xffffffff);
      func_0x00014146d898(auStack_618[0]);
    }
    func_0x00014146e9bc(0);
  }
  func_0x00014146df10(uStack_18 ^ (ulonglong)auStack_638);
  return;
}

