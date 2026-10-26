
void FUN_1401160b0(longlong param_1)

{
  undefined1 uVar1;
  char cVar2;
  longlong lVar3;
  longlong lVar4;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 auStack_c [3];
  
  lVar3 = FUN_1402c80f0(0x143d766b8);
  if ((lVar3 != 0) && (lVar4 = FUN_140c7b0e0(lVar3), lVar4 != 0)) {
    uStack_38 = 1;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_38);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x68),uVar1);
    uStack_34 = 2;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_34);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x70),uVar1);
    uStack_30 = 3;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_30);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x78),uVar1);
    uStack_2c = 0xc;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_2c);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x88),uVar1);
    uStack_28 = 1;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_28);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x1d8),uVar1);
    uStack_24 = 2;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_24);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x1e0),uVar1);
    uStack_20 = 3;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_20);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x1e8),uVar1);
    uStack_1c = 0xc;
    uVar1 = func_0x0001411eea40(lVar4,&uStack_1c);
    FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x1f8),uVar1);
    cVar2 = FUN_140c7c0a0(lVar3);
    if (cVar2 == '\0') {
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x90) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x90),0);
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x98) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x98),0);
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x200) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x200),0);
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x208) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x208),0);
    }
    else {
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x90) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x90),1);
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x98) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x98),1);
      uStack_18 = 4;
      uVar1 = func_0x0001411eea40(lVar4,&uStack_18);
      FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x90),uVar1);
      uStack_14 = 6;
      uVar1 = func_0x0001411eea40(lVar4,&uStack_14);
      FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x98),uVar1);
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x200) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x200),1);
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x28) + 0x208) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x208),1);
      uStack_10 = 4;
      uVar1 = func_0x0001411eea40(lVar4,&uStack_10);
      FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x200),uVar1);
      auStack_c[0] = 6;
      uVar1 = func_0x0001411eea40(lVar4,auStack_c);
      FUN_140d57d00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x208),uVar1);
    }
  }
  return;
}

