
void FUN_1400f47e0(longlong param_1)

{
  undefined4 uVar1;
  undefined8 uVar2;
  undefined1 auStack_100 [8];
  longlong lStack_f8;
  undefined8 uStack_f0;
  longlong lStack_e8;
  int iStack_e0;
  undefined1 auStack_d8 [8];
  undefined1 auStack_d0 [8];
  undefined1 auStack_c8 [8];
  undefined1 auStack_c0 [8];
  undefined1 auStack_b8 [8];
  undefined1 auStack_b0 [8];
  undefined1 auStack_a8 [8];
  undefined1 auStack_a0 [8];
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  uStack_98 = 0xfffffffffffffffe;
  *(undefined1 *)(param_1 + 0x98) = 1;
  if (*(int *)(param_1 + 0xa0) < 0) {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x30),0);
  }
  else {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x30),1);
  }
  uVar2 = func_0x0001402c8090(0x143d766b8);
  lStack_f8 = FUN_14146f8fc(uVar2,0,0x143569280,0x143569258,0);
  if (lStack_f8 == 0) {
    *(undefined8 *)(param_1 + 0x48) = 0;
    *(undefined8 *)(param_1 + 0x50) = 0;
  }
  else {
    uVar2 = FUN_14001d510(lStack_f8);
    *(undefined8 *)(param_1 + 0x40) = uVar2;
    if (*(longlong *)(param_1 + 0x40) == 0) {
      *(undefined4 *)(param_1 + 0x58) = 2;
      *(undefined8 *)(param_1 + 0x48) = 0;
      *(undefined8 *)(param_1 + 0x50) = 0;
    }
    else {
      uVar1 = func_0x0001400a2180(*(undefined8 *)(param_1 + 0x40));
      *(undefined4 *)(param_1 + 0x58) = uVar1;
      uVar2 = FUN_1400a7390(*(undefined8 *)(param_1 + 0x40));
      *(undefined8 *)(param_1 + 0x48) = uVar2;
      uVar2 = func_0x0001400b5a90(*(undefined8 *)(param_1 + 0x48));
      *(undefined8 *)(param_1 + 0x50) = uVar2;
    }
    lStack_e8 = FUN_14001d7d0(lStack_f8);
    if ((lStack_e8 != 0) && (iStack_e0 = FUN_140047a40(lStack_e8), -1 < iStack_e0)) {
      *(int *)(param_1 + 0x9c) = iStack_e0;
    }
  }
  _clearContents_QTableWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x90));
  _setRowCount_QTableWidget__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x90),0);
  if (*(longlong *)(param_1 + 0x50) != 0) {
    _setSortingEnabled_QTableWidget__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x90),0);
    _setColumnCount_QTableWidget__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x90),8);
    FUN_1400653e0(auStack_100);
    uStack_90 = FUN_1400ffc10(auStack_a0,&UNK_14218b398,0);
    uStack_88 = uStack_90;
    uStack_80 = FUN_1400ffc10(auStack_a8,&UNK_14218b390,0);
    uStack_78 = uStack_80;
    uStack_70 = FUN_1400ffc10(auStack_b0,&UNK_14218b388,0);
    uStack_68 = uStack_70;
    uStack_60 = FUN_1400ffc10(auStack_b8,&UNK_14218b380,0);
    uStack_58 = uStack_60;
    uStack_50 = FUN_1400ffc10(auStack_c0,&UNK_14218b378,0);
    uStack_48 = uStack_50;
    uStack_40 = FUN_1400ffc10(auStack_c8,&UNK_14218b368,0);
    uStack_38 = uStack_40;
    uStack_30 = FUN_1400ffc10(auStack_d0,&UNK_14218b358,0);
    uStack_28 = uStack_30;
    uStack_20 = FUN_1400ffc10(auStack_d8,&UNK_14218b348,0);
    uStack_18 = uStack_20;
    uVar2 = FUN_140cca3c0(auStack_100,uStack_20);
    uVar2 = FUN_140cca3c0(uVar2,uStack_28);
    uVar2 = FUN_140cca3c0(uVar2,uStack_38);
    uVar2 = FUN_140cca3c0(uVar2,uStack_48);
    uVar2 = FUN_140cca3c0(uVar2,uStack_58);
    uVar2 = FUN_140cca3c0(uVar2,uStack_68);
    uVar2 = FUN_140cca3c0(uVar2,uStack_78);
    FUN_140cca3c0(uVar2,uStack_88);
    __1QString__QEAA_XZ(auStack_d8);
    __1QString__QEAA_XZ(auStack_d0);
    __1QString__QEAA_XZ(auStack_c8);
    __1QString__QEAA_XZ(auStack_c0);
    __1QString__QEAA_XZ(auStack_b8);
    __1QString__QEAA_XZ(auStack_b0);
    __1QString__QEAA_XZ(auStack_a8);
    __1QString__QEAA_XZ(auStack_a0);
    _setHorizontalHeaderLabels_QTableWidget__QEAAXAEBVQStringList___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x90),auStack_100);
    uStack_f0 = _horizontalHeader_QTableView__QEBAPEAVQHeaderView__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x90));
    _setMinimumSectionSize_QHeaderView__QEAAXH_Z(uStack_f0,0x14);
    _resizeSection_QHeaderView__QEAAXHH_Z(uStack_f0,0,100);
    _resizeSection_QHeaderView__QEAAXHH_Z(uStack_f0,1,0x88);
    _resizeSection_QHeaderView__QEAAXHH_Z(uStack_f0,2,0x3c);
    _resizeSection_QHeaderView__QEAAXHH_Z(uStack_f0,6,0xa8);
    _resizeSection_QHeaderView__QEAAXHH_Z(uStack_f0,7,100);
    _resizeSection_QHeaderView__QEAAXHH_Z(uStack_f0,5,0x54);
    _resizeSection_QHeaderView__QEAAXHH_Z(uStack_f0,3,0x54);
    _resizeSection_QHeaderView__QEAAXHH_Z(uStack_f0,4,0x54);
    _setStretchLastSection_QHeaderView__QEAAX_N_Z(uStack_f0,1);
    if (*(int *)(param_1 + 0x5c) == 0) {
      (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x38) + 0x28) + 0x58))
                (*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x28),0);
      FUN_1400f4250(param_1);
    }
    else {
      FUN_1400f31b0(param_1);
    }
    *(undefined1 *)(param_1 + 0x98) = 0;
    FUN_1400048d0(auStack_100);
  }
  return;
}

