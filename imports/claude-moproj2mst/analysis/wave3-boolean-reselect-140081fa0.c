
void FUN_140081fa0(longlong param_1,int param_2,int param_3)

{
  longlong *plVar1;
  int iVar2;
  int iVar3;
  undefined8 *puVar4;
  int iStack_54;
  undefined1 auStack_40 [8];
  undefined8 uStack_38;
  undefined1 auStack_30 [8];
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  uStack_28 = 0xfffffffffffffffe;
  if ((*(longlong *)(param_1 + 0x18) != 0) && (-1 < param_3 + -1)) {
    for (iStack_54 = 0; iVar2 = FUN_140004ae0(param_1 + 0x20), iStack_54 < iVar2;
        iStack_54 = iStack_54 + 1) {
      puVar4 = (undefined8 *)FUN_140160c10(param_1 + 0x20,iStack_54);
      plVar1 = (longlong *)*puVar4;
      iVar2 = (**(code **)(*plVar1 + 0x28))(plVar1);
      if (iVar2 == param_2) {
        iVar2 = param_3 + -1 + iRam000000014358133c;
        (**(code **)(*plVar1 + 0x50))(plVar1,auStack_40);
        iVar3 = FUN_140004ae0(auStack_40);
        if (iVar2 < iVar3) {
          puVar4 = (undefined8 *)FUN_140160c10(auStack_40,iVar2);
          uStack_38 = *puVar4;
          (**(code **)(**(longlong **)(param_1 + 0x18) + 0x2e0))
                    (*(undefined8 *)(param_1 + 0x18),plVar1,uStack_38);
        }
        else {
          uStack_20 = _qDebug__YA_AVQDebug__XZ(auStack_30);
          uStack_18 = uStack_20;
          __6QDebug__QEAAAEAV0_PEBD_Z(uStack_20,&UNK_14213ac50);
          __1QDebug__QEAA_XZ(auStack_30);
        }
        FUN_1402081a0(auStack_40);
      }
    }
  }
  return;
}

