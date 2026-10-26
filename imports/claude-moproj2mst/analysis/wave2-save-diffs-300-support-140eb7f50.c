
void FUN_140eb7f50(longlong param_1)

{
  int iStack_40;
  undefined1 auStack_30 [8];
  undefined8 *puStack_28;
  undefined8 *puStack_20;
  undefined8 uStack_18;
  undefined8 *puStack_10;
  
  uStack_18 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_30);
  _setColumnCount_QTableWidget__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x11c0),1)
  ;
  _setShowGrid_QTableView__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x11c0),1);
  _setRowCount_QTableWidget__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x11c0),10);
  for (iStack_40 = 0; iStack_40 < 10; iStack_40 = iStack_40 + 1) {
    puStack_20 = (undefined8 *)FUN_14146eabc(0x30);
    if (puStack_20 == (undefined8 *)0x0) {
      puStack_10 = (undefined8 *)0x0;
    }
    else {
      __0QTableWidgetItem__QEAA_H_Z(puStack_20,0);
      *puStack_20 = &UNK_14218d828;
      puStack_10 = puStack_20;
    }
    puStack_28 = puStack_10;
    _setItem_QTableWidget__QEAAXHHPEAVQTableWidgetItem___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x11c0),iStack_40,0,puStack_10);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30);
  }
  _setColumnWidth_QTableView__QEAAXHH_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x11c0),0,0x118);
  __1QString__QEAA_XZ(auStack_30);
  return;
}

