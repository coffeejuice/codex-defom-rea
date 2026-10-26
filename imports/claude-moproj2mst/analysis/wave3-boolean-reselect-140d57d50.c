
void FUN_140d57d50(longlong param_1)

{
  undefined1 uVar1;
  char cVar2;
  longlong lVar3;
  longlong lVar4;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 auStack_14 [5];
  
  lVar3 = FUN_1402c80f0(0x143d766b8);
  if ((lVar3 != 0) && (lVar4 = FUN_140c7b0e0(lVar3), lVar4 != 0)) {
    uStack_28 = 1;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_28);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x28),uVar1);
    uStack_24 = 2;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_24);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x30),uVar1);
    uStack_20 = 3;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_20);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x38),uVar1);
    uStack_1c = 0xc;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_1c);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x48),uVar1);
    cVar2 = FUN_140c7c0a0(lVar3);
    if (cVar2 == '\0') {
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x50) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x50),0);
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x58) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x58),0);
    }
    else {
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x50) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x50),1);
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x58) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x58),1);
      uStack_18 = 4;
      uVar1 = func_0x0001411eea40(lVar4,&uStack_18);
      FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x50),uVar1);
      auStack_14[0] = 6;
      uVar1 = func_0x0001411eea40(lVar4,auStack_14);
      FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x58),uVar1);
    }
  }
  return;
}

