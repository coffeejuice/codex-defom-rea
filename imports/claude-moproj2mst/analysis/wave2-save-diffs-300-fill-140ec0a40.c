
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ec0a40(longlong *param_1)

{
  undefined1 auVar1 [16];
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  longlong lVar7;
  undefined4 auStack_f8 [2];
  undefined4 auStack_f0 [2];
  undefined8 uStack_e8;
  longlong lStack_e0;
  int iStack_d8;
  undefined1 auStack_d0 [8];
  undefined1 auStack_c8 [8];
  undefined8 uStack_c0;
  undefined1 auStack_b8 [8];
  longlong lStack_b0;
  longlong lStack_a8;
  ulonglong uStack_a0;
  undefined4 *puStack_98;
  undefined4 *puStack_90;
  undefined1 auStack_88 [8];
  undefined1 auStack_80 [8];
  undefined1 auStack_78 [8];
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined4 *puStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined1 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  uStack_70 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_d0);
  FUN_1400653e0(auStack_f8);
  *(undefined4 *)(param_1 + 0x20) = 0xffffffff;
  _clear_QComboBox__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x9b8));
  uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                    (*(undefined8 *)(param_1[0x28] + 0x8d8),*(undefined8 *)(param_1[0x28] + 0x9d0));
  _removeTab_QTabWidget__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x8d8),uVar3);
  uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                    (*(undefined8 *)(param_1[0x28] + 0x8d8),*(undefined8 *)(param_1[0x28] + 0x9d0));
  _setTabEnabled_QTabWidget__QEAAXH_N_Z(*(undefined8 *)(param_1[0x28] + 0x8d8),uVar3,0);
  cVar2 = FUN_140130ea0(param_1);
  if (cVar2 == '\0') {
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x960));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x958));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x950));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x948));
    uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                      (*(undefined8 *)(param_1[0x28] + 0x8d8),*(undefined8 *)(param_1[0x28] + 0x9d0)
                      );
    cVar2 = _isTabEnabled_QTabWidget__QEBA_NH_Z(*(undefined8 *)(param_1[0x28] + 0x8d8),uVar3);
    if (cVar2 == '\0') {
      uStack_68 = FUN_140f34710(auStack_b8,&UNK_142b14c18,0);
      uStack_60 = uStack_68;
      _addTab_QTabWidget__QEAAHPEAVQWidget__AEBVQString___Z
                (*(undefined8 *)(param_1[0x28] + 0x8d8),*(undefined8 *)(param_1[0x28] + 0x9d0),
                 uStack_68);
      __1QString__QEAA_XZ(auStack_b8);
    }
  }
  else {
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x960));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x958));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x950));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x948));
  }
  iStack_d8 = 0;
  while( true ) {
    uVar5 = (**(code **)(*param_1 + 0x198))(param_1);
    iVar4 = FUN_140c7c1a0(uVar5);
    if (iVar4 <= iStack_d8) break;
    iVar4 = iStack_d8 + 1;
    uVar5 = (**(code **)(*param_1 + 0x198))(param_1);
    lStack_e0 = FUN_140c7aed0(uVar5,iVar4);
    cVar2 = FUN_140ed97c0(param_1);
    if (cVar2 != '\0') {
      _sprintf_QString__QEAAAEAV1_PEBDZZ
                (auStack_d0,&UNK_142b14c30,*(undefined4 *)(lStack_e0 + 8),lStack_e0 + 0x828);
      FUN_140cca3c0(auStack_f8);
    }
    iStack_d8 = iStack_d8 + 1;
  }
  FUN_1400fcc50(auStack_f8);
  _insertItems_QComboBox__QEAAXHAEBVQStringList___Z
            (*(undefined8 *)(param_1[0x28] + 0x9b8),0,auStack_f8);
  iVar4 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x9b8));
  if (iVar4 < 1) {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x8c8),0);
  }
  else {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x8c8),1);
    if (param_1[0x1f] != 0) {
      lStack_b0 = param_1[0x1f];
      lStack_a8 = lStack_b0;
      if (lStack_b0 == 0) {
        uStack_58 = 0;
      }
      else {
        uStack_58 = FUN_140f34640(lStack_b0,3);
      }
    }
    iVar4 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x9b8));
    uStack_a0 = (ulonglong)iVar4;
    auVar1._8_8_ = 0;
    auVar1._0_8_ = uStack_a0;
    uVar6 = SUB168(ZEXT816(0x158) * auVar1,0);
    if (SUB168(ZEXT816(0x158) * auVar1,8) != 0) {
      uVar6 = 0xffffffffffffffff;
    }
    lVar7 = uVar6 + 8;
    if (0xfffffffffffffff7 < uVar6) {
      lVar7 = -1;
    }
    puStack_90 = (undefined4 *)func_0x00014146c25c(lVar7);
    if (puStack_90 == (undefined4 *)0x0) {
      puStack_50 = (undefined4 *)0x0;
    }
    else {
      *puStack_90 = (undefined4)uStack_a0;
      FUN_14146ed50(puStack_90 + 2,0x158,uStack_a0 & 0xffffffff,FUN_14085d660,FUN_14085d740);
      puStack_50 = puStack_90 + 2;
    }
    puStack_98 = puStack_50;
    param_1[0x1f] = (longlong)puStack_50;
    iStack_d8 = 0;
    while (iVar4 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x9b8)),
          iStack_d8 < iVar4) {
      uStack_48 = _itemText_QComboBox__QEBA_AVQString__H_Z
                            (*(undefined8 *)(param_1[0x28] + 0x9b8),auStack_88,iStack_d8);
      uStack_40 = uStack_48;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_d0,uStack_48);
      __1QString__QEAA_XZ(auStack_88);
      _toLocal8Bit_QString__QEBA_AVQByteArray__XZ(auStack_d0,auStack_c8);
      uStack_e8 = _data_QByteArray__QEAAPEADXZ(auStack_c8);
      FUN_14146dfd4(uStack_e8,&UNK_142b14c38,auStack_f0);
      uVar5 = (**(code **)(*param_1 + 0x198))(param_1);
      lStack_e0 = FUN_140c7ae80(uVar5,auStack_f0[0]);
      func_0x00014085e050((longlong)iStack_d8 * 0x158 + param_1[0x1f]);
      __1QByteArray__QEAA_XZ(auStack_c8);
      iStack_d8 = iStack_d8 + 1;
    }
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x9b8),0);
    FUN_140eceb60(param_1,0);
    *(undefined4 *)(param_1 + 0x20) = 0;
  }
  FUN_140ec17c0(param_1);
  cVar2 = FUN_140130ee0(param_1);
  if (cVar2 != '\0') {
    uStack_c0 = func_0x0001402c7ed0();
    cVar2 = func_0x0001400abcb0(uStack_c0);
    if (cVar2 == '\0') {
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0xa40),1);
      uStack_20 = _number_QString__SA_AV1_NDH_Z(auStack_78,_UNK_1422d3a88,0x67,6);
      uStack_18 = uStack_20;
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0xa10),uStack_20);
      __1QString__QEAA_XZ(auStack_78);
    }
    else {
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0xa48),1);
      iVar4 = func_0x0001413ee240(uStack_c0);
      if (iVar4 == 1) {
        _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0xa00),1);
      }
      else {
        iVar4 = func_0x0001413ee240(uStack_c0);
        if (iVar4 == 3) {
          _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0xa08),1);
        }
      }
      uVar5 = func_0x000140b7bfc0(uStack_c0);
      uStack_38 = _number_QString__SA_AV1_NDH_Z(auStack_80,uVar5,0x67,6);
      uStack_30 = uStack_38;
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0xa10),uStack_38);
      __1QString__QEAA_XZ(auStack_80);
      cVar2 = func_0x000140e7b830(uStack_c0);
      uStack_28 = cVar2 == '\0';
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0xa28),uStack_28);
    }
  }
  FUN_1400048d0(auStack_f8);
  __1QString__QEAA_XZ(auStack_d0);
  return;
}

