
uint FUN_1412fae10(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined *puVar4;
  undefined *puVar5;
  
  iVar1 = func_0x0001412e6910();
  if (iVar1 != 0) {
    if (param_1 == 2) {
      uVar2 = func_0x0001412fd7f0();
      uVar3 = func_0x0001413058a0();
      puVar5 = &UNK_142db5428;
      puVar4 = &UNK_142db56f0;
    }
    else {
      uVar2 = func_0x000141330840();
      uVar3 = func_0x00014138d440();
      puVar5 = &UNK_142db5300;
      puVar4 = &UNK_142db56e0;
    }
    iVar1 = FUN_1412eef80(puVar4,uVar3,uVar2,puVar5);
    return (uint)(0 < iVar1);
  }
  iVar1 = func_0x0001412faad0(uRam00000001446f2054);
  if (iVar1 == 2) {
    return *puRam0000000143ad2fb8;
  }
  return puRam0000000143ad2fb8[7];
}

