
ulonglong FUN_1405efe40(longlong *param_1,longlong *param_2)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  ulonglong in_RAX;
  ulonglong uVar4;
  undefined8 uVar5;
  longlong *plVar6;
  longlong *plVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  bool bVar10;
  byte bStack_8c;
  int iStack_50;
  int iStack_48;
  undefined1 auStack_40 [24];
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  uint uStack_10;
  
  uStack_28 = 0xfffffffffffffffe;
  bStack_8c = 1;
  if (param_2 == (longlong *)0x0) {
    uVar4 = in_RAX & 0xffffffffffffff00;
  }
  else {
    uStack_20 = __0QModelIndex__QEAA_XZ(auStack_40);
    uStack_18 = uStack_20;
    iVar2 = (**(code **)(*param_2 + 0x68))(param_2,uStack_20);
    __1QModelIndex__QEAA_XZ(auStack_40);
    uVar5 = _invisibleRootItem_QStandardItemModel__QEBAPEAVQStandardItem__XZ(param_2);
    plVar6 = (longlong *)FUN_140d41a00(param_2);
    for (iStack_50 = 0; iStack_50 < iVar2; iStack_50 = iStack_50 + 1) {
      plVar7 = (longlong *)
               _item_QStandardItemModel__QEBAPEAVQStandardItem__HH_Z(param_2,iStack_50,0);
      uVar8 = _child_QStandardItem__QEBAPEAV1_HH_Z(uVar5,iStack_50);
      iVar3 = (**(code **)(*plVar7 + 0x20))(plVar7);
      bVar10 = plVar7 == plVar6;
      uStack_10 = (uint)bVar10;
      if (iVar3 == 0x3f1) {
        bVar1 = FUN_1405efc70(param_1,plVar7);
        bStack_8c = bStack_8c & bVar1;
      }
      else {
        bVar1 = (**(code **)(*param_1 + 0x268))(param_1,plVar7,uVar8,bVar10);
        bStack_8c = bStack_8c & bVar1;
        iVar3 = _rowCount_QStandardItem__QEBAHXZ(plVar7);
        for (iStack_48 = 0; iStack_48 < iVar3; iStack_48 = iStack_48 + 1) {
          uVar8 = _child_QStandardItem__QEBAPEAV1_HH_Z(plVar7,iStack_48,0);
          uVar9 = _child_QStandardItem__QEBAPEAV1_HH_Z(plVar7,iStack_48,1);
          bVar1 = (**(code **)(*param_1 + 0x268))(param_1,uVar8,uVar9,bVar10,uVar9);
          bStack_8c = bStack_8c & bVar1;
        }
      }
    }
    uVar4 = (ulonglong)CONCAT31((int3)((uint)iVar2 >> 8),bStack_8c);
  }
  return uVar4;
}

