
void FUN_140ebb3f0(longlong *param_1)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  int iStack_d8;
  undefined1 auStack_d0 [8];
  longlong lStack_c8;
  longlong lStack_c0;
  int iStack_b8;
  undefined1 auStack_b0 [8];
  undefined1 auStack_a8 [16];
  undefined1 auStack_98 [8];
  undefined4 auStack_90 [2];
  undefined1 auStack_88 [8];
  undefined1 auStack_80 [8];
  undefined1 auStack_78 [8];
  undefined1 auStack_70 [16];
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
  undefined8 uStack_10;
  
  uStack_60 = 0xfffffffffffffffe;
  lStack_c8 = FUN_140130f90(param_1);
  if (lStack_c8 != 0) {
    __0QString__QEAA_XZ(auStack_b0);
    __0QString__QEAA_XZ(auStack_d0);
    iStack_b8 = 0;
    _blockSignals_QObject__QEAA_N_N_Z(*(undefined8 *)(param_1[0x28] + 0x16c8),1);
    _clear_QComboBox__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x16c8));
    auStack_90[0] = 0;
    uStack_58 = __0QVariant__QEAA_XZ(auStack_a8);
    uStack_50 = uStack_58;
    uStack_48 = FUN_140f34710(auStack_98,&UNK_142b14658,0);
    uStack_40 = uStack_48;
    uStack_38 = FUN_140ec98e0(param_1,auStack_88,auStack_90,uStack_48);
    uStack_30 = uStack_38;
    _addItem_QComboBox__QEAAXAEBVQString__AEBVQVariant___Z
              (*(undefined8 *)(param_1[0x28] + 0x16c8),uStack_38,uStack_50);
    __1QString__QEAA_XZ(auStack_88);
    __1QString__QEAA_XZ(auStack_98);
    __1QVariant__QEAA_XZ(auStack_a8);
    iStack_d8 = 0;
    while( true ) {
      uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
      iVar2 = FUN_140c7c1a0(uVar3);
      if (iVar2 <= iStack_d8) break;
      uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
      lStack_c0 = FUN_140c7aed0(uVar3,iStack_d8 + 1);
      if ((lStack_c0 != 0) && (cVar1 = FUN_140834fa0(lStack_c0), cVar1 != '\0')) {
        __0QString__QEAA_PEBD_Z(auStack_80,lStack_c0 + 0x828);
        uStack_28 = FUN_140ec98e0(param_1,auStack_78,lStack_c0 + 8,auStack_80);
        uStack_20 = uStack_28;
        __4QString__QEAAAEAV0_AEBV0__Z(auStack_b0,uStack_28);
        __1QString__QEAA_XZ(auStack_78);
        __1QString__QEAA_XZ(auStack_80);
        uStack_18 = __0QVariant__QEAA_XZ(auStack_70);
        uStack_10 = uStack_18;
        _addItem_QComboBox__QEAAXAEBVQString__AEBVQVariant___Z
                  (*(undefined8 *)(param_1[0x28] + 0x16c8),auStack_b0,uStack_18);
        __1QVariant__QEAA_XZ(auStack_70);
        if (*(int *)(lStack_c0 + 8) == *(int *)(lStack_c8 + 0x168)) {
          iStack_b8 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x16c8));
          iStack_b8 = iStack_b8 + -1;
        }
      }
      iStack_d8 = iStack_d8 + 1;
    }
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x16c8),iStack_b8);
    _blockSignals_QObject__QEAA_N_N_Z(*(undefined8 *)(param_1[0x28] + 0x16c8),0);
    __1QString__QEAA_XZ(auStack_d0);
    __1QString__QEAA_XZ(auStack_b0);
  }
  return;
}

