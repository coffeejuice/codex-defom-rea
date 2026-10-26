
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ec4970(longlong param_1,longlong param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined8 *puVar6;
  int iStack_108;
  undefined1 auStack_c8 [8];
  undefined8 *puStack_c0;
  undefined8 *puStack_b8;
  undefined1 auStack_b0 [8];
  undefined8 *puStack_a8;
  undefined8 *puStack_a0;
  undefined1 auStack_98 [8];
  undefined8 *puStack_90;
  undefined8 *puStack_88;
  undefined1 auStack_80 [8];
  uint uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 *puStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 *puStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 *puStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 *puStack_10;
  
  uStack_70 = 0xfffffffffffffffe;
  uStack_78 = 0;
  _clearContents_QTableWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 4000));
  _setRowCount_QTableWidget__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 4000),0);
  _clearContents_QTableWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfc0));
  _setRowCount_QTableWidget__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfc0),0);
  _clearContents_QTableWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfe0));
  _setRowCount_QTableWidget__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfe0),0);
  if (0 < *(int *)(param_2 + 0xa74)) {
    iVar3 = *(int *)(param_2 + 0xa98);
    for (iStack_108 = 0; iStack_108 < *(int *)(param_2 + 0xa74); iStack_108 = iStack_108 + 1) {
      if (*(double *)(*(longlong *)(param_2 + 0xa80) + (longlong)iStack_108 * 8) == _UNK_142101520)
      {
        iVar4 = _rowCount_QTableWidget__QEBAHXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 4000));
        _setRowCount_QTableWidget__QEAAXH_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 4000),iVar4 + 1);
        uVar1 = *(undefined8 *)(*(longlong *)(param_2 + 0xaa0) + (longlong)(iStack_108 * iVar3) * 8)
        ;
        uVar2 = *(undefined8 *)
                 (*(longlong *)(param_2 + 0xaa0) + (longlong)(iStack_108 * iVar3 + 1) * 8);
        puVar6 = (undefined8 *)FUN_14146eabc(0x30);
        if (puVar6 == (undefined8 *)0x0) {
          puStack_58 = (undefined8 *)0x0;
        }
        else {
          uStack_68 = _number_QString__SA_AV1_NDH_Z(auStack_c8,uVar1,0x67,6);
          uStack_78 = uStack_78 | 1;
          uStack_60 = uStack_68;
          __0QTableWidgetItem__QEAA_AEBVQString__H_Z(puVar6,uStack_68,0);
          *puVar6 = &UNK_14218d828;
          puStack_58 = puVar6;
        }
        puVar6 = puStack_58;
        if ((uStack_78 & 1) != 0) {
          uStack_78 = uStack_78 & 0xfffffffe;
          __1QString__QEAA_XZ(auStack_c8);
        }
        _setItem_QTableWidget__QEAAXHHPEAVQTableWidgetItem___Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 4000),iVar4,0,puVar6);
        puStack_b8 = (undefined8 *)FUN_14146eabc(0x30);
        if (puStack_b8 == (undefined8 *)0x0) {
          puStack_40 = (undefined8 *)0x0;
        }
        else {
          uStack_50 = _number_QString__SA_AV1_NDH_Z(auStack_b0,uVar2,0x67,6);
          uStack_78 = uStack_78 | 2;
          uStack_48 = uStack_50;
          __0QTableWidgetItem__QEAA_AEBVQString__H_Z(puStack_b8,uStack_50,0);
          *puStack_b8 = &UNK_14218d828;
          puStack_40 = puStack_b8;
        }
        puVar6 = puStack_40;
        puStack_c0 = puStack_40;
        if ((uStack_78 & 2) != 0) {
          uStack_78 = uStack_78 & 0xfffffffd;
          __1QString__QEAA_XZ(auStack_b0);
        }
        _setItem_QTableWidget__QEAAXHHPEAVQTableWidgetItem___Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 4000),iVar4,1,puVar6);
        _rowCount_QTableWidget__QEBAHXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 4000));
      }
      else if (*(double *)(*(longlong *)(param_2 + 0xa80) + (longlong)iStack_108 * 8) ==
               _UNK_1421ce530) {
        uVar5 = _rowCount_QTableWidget__QEBAHXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfc0));
        _insertRow_QTableWidget__QEAAXH_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfc0),uVar5);
        uVar1 = *(undefined8 *)(*(longlong *)(param_2 + 0xaa0) + (longlong)(iStack_108 * iVar3) * 8)
        ;
        puStack_a0 = (undefined8 *)FUN_14146eabc(0x30);
        if (puStack_a0 == (undefined8 *)0x0) {
          puStack_28 = (undefined8 *)0x0;
        }
        else {
          uStack_38 = _number_QString__SA_AV1_NDH_Z(auStack_98,uVar1,0x67,6);
          uStack_78 = uStack_78 | 4;
          uStack_30 = uStack_38;
          __0QTableWidgetItem__QEAA_AEBVQString__H_Z(puStack_a0,uStack_38);
          *puStack_a0 = &UNK_14218d828;
          puStack_28 = puStack_a0;
        }
        puVar6 = puStack_28;
        puStack_a8 = puStack_28;
        if ((uStack_78 & 4) != 0) {
          uStack_78 = uStack_78 & 0xfffffffb;
          __1QString__QEAA_XZ(auStack_98);
        }
        _setItem_QTableWidget__QEAAXHHPEAVQTableWidgetItem___Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfc0),uVar5,0,puVar6);
      }
      else if (*(double *)(*(longlong *)(param_2 + 0xa80) + (longlong)iStack_108 * 8) ==
               _UNK_1421d3c70) {
        uVar5 = _rowCount_QTableWidget__QEBAHXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfe0));
        _insertRow_QTableWidget__QEAAXH_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfe0),uVar5);
        uVar1 = *(undefined8 *)(*(longlong *)(param_2 + 0xaa0) + (longlong)(iStack_108 * iVar3) * 8)
        ;
        puStack_88 = (undefined8 *)FUN_14146eabc(0x30);
        if (puStack_88 == (undefined8 *)0x0) {
          puStack_10 = (undefined8 *)0x0;
        }
        else {
          uStack_20 = _number_QString__SA_AV1_NDH_Z(auStack_80,uVar1,0x67,6);
          uStack_78 = uStack_78 | 8;
          uStack_18 = uStack_20;
          __0QTableWidgetItem__QEAA_AEBVQString__H_Z(puStack_88,uStack_20,0);
          *puStack_88 = &UNK_14218d828;
          puStack_10 = puStack_88;
        }
        puVar6 = puStack_10;
        puStack_90 = puStack_10;
        if ((uStack_78 & 8) != 0) {
          uStack_78 = uStack_78 & 0xfffffff7;
          __1QString__QEAA_XZ(auStack_80);
        }
        _setItem_QTableWidget__QEAAXHHPEAVQTableWidgetItem___Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xfe0),uVar5,0,puVar6);
      }
    }
  }
  return;
}

