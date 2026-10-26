
void FUN_140d3a880(undefined8 param_1,longlong param_2)

{
  undefined1 uVar1;
  int iVar2;
  undefined8 uVar3;
  int iStack_44;
  undefined1 auStack_38 [24];
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  uStack_20 = 0xfffffffffffffffe;
  if (param_2 != 0) {
    uVar1 = func_0x000140d3d570(param_2);
    uStack_18 = _index_QStandardItem__QEBA_AVQModelIndex__XZ(param_2,auStack_38);
    uStack_10 = uStack_18;
    _setExpanded_QTreeView__QEAAXAEBVQModelIndex___N_Z(param_1,uStack_18,uVar1);
    __1QModelIndex__QEAA_XZ(auStack_38);
    iVar2 = _rowCount_QStandardItem__QEBAHXZ(param_2);
    for (iStack_44 = 0; iStack_44 < iVar2; iStack_44 = iStack_44 + 1) {
      uVar3 = _child_QStandardItem__QEBAPEAV1_HH_Z(param_2,iStack_44,0);
      FUN_140d3a880(param_1,uVar3);
    }
  }
  return;
}

