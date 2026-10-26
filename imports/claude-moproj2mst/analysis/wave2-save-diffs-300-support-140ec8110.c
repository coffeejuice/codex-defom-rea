
void FUN_140ec8110(longlong *param_1)

{
  int iVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  longlong alStack_1a8 [2];
  undefined8 uStack_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  undefined1 auStack_148 [16];
  undefined4 auStack_138 [10];
  char cStack_110;
  int iStack_10c;
  undefined1 auStack_108 [8];
  undefined8 uStack_100;
  int iStack_f8;
  undefined1 auStack_f0 [8];
  undefined1 auStack_e8 [8];
  undefined1 auStack_e0 [8];
  undefined1 auStack_d8 [8];
  undefined1 auStack_d0 [8];
  undefined1 auStack_c8 [8];
  undefined1 auStack_c0 [8];
  undefined1 auStack_b8 [8];
  undefined1 auStack_b0 [8];
  undefined1 auStack_a8 [8];
  undefined1 auStack_a0 [8];
  undefined1 auStack_98 [8];
  undefined1 auStack_90 [8];
  undefined8 uStack_88;
  longlong lStack_80;
  undefined1 auStack_78 [64];
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined1 uStack_28;
  
  uStack_38 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_108);
  FUN_1410da4d0(auStack_148);
  _removeTab_QTabWidget__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1468),2);
  __0QString__QEAA_PEBD_Z(auStack_f0,&UNK_142b14e60);
  cStack_110 = FUN_1410da510(auStack_148,auStack_f0,&uStack_198,2);
  __1QString__QEAA_XZ(auStack_f0);
  if (cStack_110 == '\0') {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x14f0),0);
    __0QString__QEAA_PEBD_Z(auStack_e8,&UNK_142b14e68);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x14c0),auStack_e8);
    __1QString__QEAA_XZ(auStack_e8);
    __0QString__QEAA_PEBD_Z(auStack_e0,&UNK_142b14e70);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x14e0),auStack_e0);
    __1QString__QEAA_XZ(auStack_e0);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x14f0),1);
    _setNum_QString__QEAAAEAV1_NDH_Z(auStack_108,uStack_198,0x67,6);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x14c0),auStack_108);
    _setNum_QString__QEAAAEAV1_NDH_Z(auStack_108,uStack_190,0x67,6);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x14e0),auStack_108);
  }
  __0QString__QEAA_PEBD_Z(auStack_d8,&UNK_142b14e78);
  cStack_110 = FUN_1410da690(auStack_148,auStack_d8,auStack_138,1);
  __1QString__QEAA_XZ(auStack_d8);
  if (cStack_110 == '\0') {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1480),0);
    __0QString__QEAA_PEBD_Z(auStack_d0,&UNK_142b14e84);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1510),auStack_d0);
    __1QString__QEAA_XZ(auStack_d0);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1480),1);
    _setNum_QString__QEAAAEAV1_HH_Z(auStack_108,auStack_138[0],10);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1510),auStack_108);
  }
  __0QString__QEAA_PEBD_Z(auStack_c8,&UNK_142b14e90);
  cStack_110 = FUN_1410da690(auStack_148,auStack_c8,auStack_138,1);
  __1QString__QEAA_XZ(auStack_c8);
  if (cStack_110 == '\0') {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1538),0);
    __0QString__QEAA_PEBD_Z(auStack_c0,&UNK_142b14e9c);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1550),auStack_c0);
    __1QString__QEAA_XZ(auStack_c0);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1538),1);
    _setNum_QString__QEAAAEAV1_HH_Z(auStack_108,auStack_138[0],10);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1550),auStack_108);
  }
  if (cStack_110 != '\0') {
    __0QString__QEAA_PEBD_Z(auStack_b8,&UNK_142b14ea0);
    cStack_110 = FUN_1410da510(auStack_148,auStack_b8,&uStack_198,3);
    __1QString__QEAA_XZ(auStack_b8);
  }
  if (cStack_110 == '\0') {
    __0QString__QEAA_PEBD_Z(auStack_b0,&UNK_142b14eac);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1560),auStack_b0);
    __1QString__QEAA_XZ(auStack_b0);
    __0QString__QEAA_PEBD_Z(auStack_a8,&UNK_142b14eb0);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1570),auStack_a8);
    __1QString__QEAA_XZ(auStack_a8);
  }
  else {
    _setNum_QString__QEAAAEAV1_NDH_Z(auStack_108,uStack_190,0x67,6);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1560),auStack_108);
    _setNum_QString__QEAAAEAV1_NDH_Z(auStack_108,uStack_188,0x67,6);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1570),auStack_108);
  }
  __0QString__QEAA_PEBD_Z(auStack_a0,&UNK_142b14eb8);
  cStack_110 = FUN_1410da510(auStack_148,auStack_a0,&uStack_198,1);
  __1QString__QEAA_XZ(auStack_a0);
  if (cStack_110 == '\0') {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x15c0),0);
    __0QString__QEAA_PEBD_Z(auStack_98,&UNK_142b14ec8);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x15b8),auStack_98);
    __1QString__QEAA_XZ(auStack_98);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x15c0),1);
    _setNum_QString__QEAAAEAV1_NDH_Z(auStack_108,uStack_198,0x67,6);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x15b8),auStack_108);
  }
  __0QString__QEAA_PEBD_Z(auStack_90,&UNK_142b14ed0);
  cStack_110 = FUN_1410da510(auStack_148,auStack_90,&uStack_198,0);
  __1QString__QEAA_XZ(auStack_90);
  if (cStack_110 == '\0') {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x15c8),0);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x15c8),1);
  }
  FUN_140ec8b00(param_1);
  FUN_140ec8e70(param_1);
  alStack_1a8[0] = 0;
  iStack_10c = 0;
  _clear_QListWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x1610));
  cStack_110 = FUN_1410dab40(auStack_148,alStack_1a8,&iStack_10c);
  if (cStack_110 != '\0') {
    for (iStack_f8 = 0; iStack_f8 < iStack_10c; iStack_f8 = iStack_f8 + 1) {
      lStack_80 = FUN_14146eabc(0x78);
      if (lStack_80 == 0) {
        uStack_30 = 0;
      }
      else {
        uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
        puVar4 = (undefined1 *)(alStack_1a8[0] + (longlong)iStack_f8 * 0x40);
        puVar5 = auStack_78;
        for (lVar3 = 0x40; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar5 = *puVar4;
          puVar4 = puVar4 + 1;
          puVar5 = puVar5 + 1;
        }
        uStack_30 = FUN_140eb2470(lStack_80,*(undefined8 *)(param_1[0x28] + 0x1610),auStack_78,uVar2
                                 );
      }
      uStack_88 = uStack_30;
      uStack_100 = uStack_30;
    }
  }
  iVar1 = _count_QListWidget__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x1610));
  uStack_28 = 0 < iVar1;
  _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x15f0),uStack_28);
  *(undefined4 *)(param_1 + 0x23) = 0xffffffff;
  iVar1 = _count_QListWidget__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x1610));
  if (0 < iVar1) {
    _setCurrentRow_QListWidget__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1610),0);
    uVar2 = _item_QListWidget__QEBAPEAVQListWidgetItem__H_Z
                      (*(undefined8 *)(param_1[0x28] + 0x1610),0);
    FUN_140ecdee0(param_1,uVar2);
  }
  FUN_140f34760(auStack_148);
  __1QString__QEAA_XZ(auStack_108);
  return;
}

