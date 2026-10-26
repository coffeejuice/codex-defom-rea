
void FUN_140ed7e10(longlong param_1,undefined4 param_2)

{
  int iVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  longlong lVar4;
  int iStack_58;
  longlong lStack_50;
  int iStack_48;
  int iStack_44;
  undefined1 auStack_40 [32];
  longlong lStack_20;
  undefined8 uStack_18;
  undefined4 uStack_10;
  
  uStack_18 = 0xfffffffffffffffe;
  uVar2 = FUN_140130af0(param_1);
  lStack_20 = FUN_14005a400(uVar2);
  uVar2 = FUN_140130af0(param_1);
  lStack_50 = func_0x0001401cc700(uVar2);
  iStack_44 = _currentRow_QTableWidget__QEBAHXZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1800));
  if (iStack_44 < 0) {
    iStack_44 = 0;
    _blockSignals_QObject__QEAA_N_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1800),1);
    _setCurrentCell_QTableWidget__QEAAXHH_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1800),0,0);
    _blockSignals_QObject__QEAA_N_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1800),0);
  }
  FUN_140d49e30(auStack_40);
  iStack_48 = 0;
  if (lStack_20 != 0) {
    iStack_48 = FUN_140004ae0(lStack_20);
  }
  if ((iStack_44 < iStack_48) || (iVar1 = FUN_140004ae0(lStack_50), iStack_48 + iVar1 <= iStack_44))
  {
    lStack_50 = lStack_20;
    iStack_58 = iStack_44;
  }
  else {
    iStack_58 = iStack_44 - iStack_48;
  }
  uStack_10 = param_2;
  switch(param_2) {
  case 0:
    (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x140) + 0x17c0) + 0x58))
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x17c0),0);
    puVar3 = (undefined4 *)FUN_140067100(lStack_50,iStack_58);
    *puVar3 = 0;
    lVar4 = FUN_140067100(lStack_50,iStack_58);
    *(undefined4 *)(lVar4 + 4) = 2;
    break;
  case 1:
    (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x140) + 0x17c0) + 0x58))
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x17c0),0);
    puVar3 = (undefined4 *)FUN_140067100(lStack_50,iStack_58);
    *puVar3 = 0;
    lVar4 = FUN_140067100(lStack_50,iStack_58);
    *(undefined4 *)(lVar4 + 4) = 3;
    break;
  case 2:
    (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x140) + 0x17c0) + 0x58))
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x17c0),1);
    puVar3 = (undefined4 *)FUN_140067100(lStack_50,iStack_58);
    *puVar3 = 0;
    lVar4 = FUN_140067100(lStack_50,iStack_58);
    *(undefined4 *)(lVar4 + 4) = 4;
    break;
  case 3:
    (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x140) + 0x17c0) + 0x58))
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x17c0),1);
    puVar3 = (undefined4 *)FUN_140067100(lStack_50,iStack_58);
    *puVar3 = 0;
    lVar4 = FUN_140067100(lStack_50,iStack_58);
    *(undefined4 *)(lVar4 + 4) = 5;
    break;
  case 4:
    (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x140) + 0x17c0) + 0x58))
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x17c0),1);
    puVar3 = (undefined4 *)FUN_140067100(lStack_50,iStack_58);
    *puVar3 = 0;
    lVar4 = FUN_140067100(lStack_50,iStack_58);
    *(undefined4 *)(lVar4 + 4) = 6;
    break;
  case 5:
    (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x140) + 0x17c0) + 0x58))
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x17c0),1);
    puVar3 = (undefined4 *)FUN_140067100(lStack_50,iStack_58);
    *puVar3 = 0;
    lVar4 = FUN_140067100(lStack_50,iStack_58);
    *(undefined4 *)(lVar4 + 4) = 7;
    break;
  case 6:
    (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x140) + 0x17c0) + 0x58))
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x17c0),1);
    puVar3 = (undefined4 *)FUN_140067100(lStack_50,iStack_58);
    *puVar3 = 1;
    lVar4 = FUN_140067100(lStack_50,iStack_58);
    *(undefined4 *)(lVar4 + 4) = 1;
    break;
  case 7:
    (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x140) + 0x17c0) + 0x58))
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x17c0),1);
    puVar3 = (undefined4 *)FUN_140067100(lStack_50,iStack_58);
    *puVar3 = 1;
    lVar4 = FUN_140067100(lStack_50,iStack_58);
    *(undefined4 *)(lVar4 + 4) = 2;
    break;
  case 8:
    (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x140) + 0x17c0) + 0x58))
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x17c0),1);
    puVar3 = (undefined4 *)FUN_140067100(lStack_50,iStack_58);
    *puVar3 = 1;
    lVar4 = FUN_140067100(lStack_50,iStack_58);
    *(undefined4 *)(lVar4 + 4) = 3;
  }
  FUN_140ed4bb0(param_1);
  _selectRow_QTableView__QEAAXH_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1800),iStack_44);
  FUN_140ed6460(param_1,iStack_44,0);
  func_0x000140cac130(auStack_40);
  return;
}

