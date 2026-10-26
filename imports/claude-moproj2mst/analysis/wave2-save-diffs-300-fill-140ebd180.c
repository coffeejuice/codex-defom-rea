
void FUN_140ebd180(longlong *param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  longlong lVar4;
  int iStack_184;
  int iStack_180;
  undefined4 uStack_178;
  int iStack_168;
  undefined4 uStack_164;
  undefined1 auStack_160 [8];
  undefined1 auStack_158 [16];
  undefined1 auStack_148 [8];
  undefined1 auStack_140 [8];
  undefined4 auStack_138 [2];
  undefined1 auStack_130 [8];
  undefined1 auStack_128 [16];
  undefined1 auStack_118 [16];
  undefined1 auStack_108 [8];
  undefined1 auStack_100 [8];
  undefined1 auStack_f8 [16];
  undefined1 auStack_e8 [16];
  undefined1 auStack_d8 [8];
  undefined1 auStack_d0 [8];
  undefined1 auStack_c8 [16];
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
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
  
  uStack_b8 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_160);
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 == '\0') {
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x5a0));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x598));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x640));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x638));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x608));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x600));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x6f0));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x6e8));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x788));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x780));
    iVar2 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x7c0));
    if (iVar2 == 3) {
      __0QString__QEAA_PEBD_Z(auStack_148,&UNK_142b14840);
      uStack_b0 = __0QVariant__QEAA_XZ(auStack_158);
      uStack_a8 = uStack_b0;
      _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
                (*(undefined8 *)(param_1[0x28] + 0x7c0),3,auStack_148,uStack_b0);
      __1QVariant__QEAA_XZ(auStack_158);
      __1QString__QEAA_XZ(auStack_148);
    }
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x878));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x898));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x858));
  }
  else {
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x5a0));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x598));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x640));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x638));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x608));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x600));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x6f0));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x6e8));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x788));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x780));
    iVar2 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x7c0));
    if (iVar2 == 4) {
      _removeItem_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x7c0),3);
    }
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x878));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x898));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x858));
  }
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b1484c,*(undefined8 *)(param_2 + 0x178));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x548),auStack_160);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b14854,*(undefined8 *)(param_2 + 0x180));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x580),auStack_160);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b1485c,*(undefined8 *)(param_2 + 0x188));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x590),auStack_160);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b14864,*(undefined8 *)(param_2 + 400));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x5a0),auStack_160);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b1486c,*(undefined8 *)(param_2 + 0x198));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x620),auStack_160);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b14874,*(undefined8 *)(param_2 + 0x1a0));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x630),auStack_160);
  cVar1 = FUN_140130ee0(param_1);
  if (cVar1 != '\0') {
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b1487c,*(undefined8 *)(param_2 + 0x1a8));
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x640),auStack_160);
  }
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b14884,*(undefined8 *)(param_2 + 0x1b0));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x5e8),auStack_160);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b1488c,*(undefined8 *)(param_2 + 0x1b8));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x5f8),auStack_160);
  cVar1 = FUN_140130ee0(param_1);
  if (cVar1 != '\0') {
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b14894,*(undefined8 *)(param_2 + 0x1c0));
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x608),auStack_160);
  }
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b1489c,*(undefined8 *)(param_2 + 0x170));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x648),auStack_160);
  _clear_QComboBox__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x708));
  _clear_QComboBox__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x7a0));
  auStack_138[0] = 0;
  uStack_a0 = FUN_140f34710(auStack_140,&UNK_142b148a4,0);
  uStack_98 = uStack_a0;
  uStack_90 = FUN_140ec98e0(param_1,auStack_130,auStack_138,uStack_a0);
  uStack_88 = uStack_90;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_160,uStack_90);
  __1QString__QEAA_XZ(auStack_130);
  __1QString__QEAA_XZ(auStack_140);
  uStack_80 = __0QVariant__QEAA_XZ(auStack_128);
  uStack_78 = uStack_80;
  _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
            (*(undefined8 *)(param_1[0x28] + 0x708),0,auStack_160,uStack_80);
  __1QVariant__QEAA_XZ(auStack_128);
  uStack_70 = __0QVariant__QEAA_XZ(auStack_118);
  uStack_68 = uStack_70;
  _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
            (*(undefined8 *)(param_1[0x28] + 0x7a0),0,auStack_160,uStack_70);
  __1QVariant__QEAA_XZ(auStack_118);
  iStack_168 = 0;
  iStack_180 = 0;
  uStack_164 = 0;
  uStack_178 = 0;
  iStack_184 = 0;
  while( true ) {
    uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
    iVar2 = FUN_140c7c1a0(uVar3);
    if (iVar2 <= iStack_184) break;
    uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
    lVar4 = FUN_140c7aed0(uVar3,iStack_184 + 1);
    if ((lVar4 != 0) && (cVar1 = FUN_140834fa0(lVar4), cVar1 != '\0')) {
      __0QString__QEAA_PEBD_Z(auStack_108,lVar4 + 0x828);
      uStack_60 = FUN_140ec98e0(param_1,auStack_100,lVar4 + 8,auStack_108);
      uStack_58 = uStack_60;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_160,uStack_60);
      __1QString__QEAA_XZ(auStack_100);
      __1QString__QEAA_XZ(auStack_108);
      uStack_50 = __0QVariant__QEAA_XZ(auStack_f8);
      uStack_48 = uStack_50;
      _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
                (*(undefined8 *)(param_1[0x28] + 0x708),iStack_184 + 1,auStack_160,uStack_50);
      __1QVariant__QEAA_XZ(auStack_f8);
      uStack_40 = __0QVariant__QEAA_XZ(auStack_e8);
      uStack_38 = uStack_40;
      _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
                (*(undefined8 *)(param_1[0x28] + 0x7a0),iStack_184 + 1,auStack_160,uStack_40);
      __1QVariant__QEAA_XZ(auStack_e8);
      if (*(int *)(lVar4 + 8) == *(int *)(param_2 + 0x288)) {
        iStack_168 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x708));
        iStack_168 = iStack_168 + -1;
        uStack_164 = *(undefined4 *)(lVar4 + 8);
      }
      if (*(int *)(lVar4 + 8) == *(int *)(param_2 + 0x28c)) {
        iStack_180 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x7a0));
        iStack_180 = iStack_180 + -1;
        uStack_178 = *(undefined4 *)(lVar4 + 8);
      }
    }
    iStack_184 = iStack_184 + 1;
  }
  _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x708),iStack_168);
  _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x7a0),iStack_180);
  uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
  uVar3 = FUN_140c7ae80(uVar3,uStack_164);
  FUN_140eb6440(param_1,uVar3,0);
  uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
  uVar3 = FUN_140c7ae80(uVar3,uStack_178);
  FUN_140eb6440(param_1,uVar3,1);
  FUN_140ebe100(param_1,0);
  FUN_140ebe100(param_1,1);
  _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x7c0));
  _setCurrentIndex_QComboBox__QEAAXH_Z
            (*(undefined8 *)(param_1[0x28] + 0x7c0),*(undefined4 *)(param_2 + 0x290));
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_160,&UNK_142b148ac,*(undefined8 *)(param_2 + 0x298));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x7d8),auStack_160);
  _clear_QComboBox__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x838));
  *(undefined4 *)((longlong)param_1 + 0x11c) = 0;
  iStack_168 = 0;
  iStack_184 = 0;
  while( true ) {
    uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
    iVar2 = FUN_140c7c1a0(uVar3);
    if (iVar2 <= iStack_184) break;
    uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
    lVar4 = FUN_140c7aed0(uVar3);
    if ((lVar4 != 0) && (cVar1 = FUN_140834fa0(lVar4), cVar1 != '\0')) {
      __0QString__QEAA_PEBD_Z(auStack_d8,lVar4 + 0x828);
      uStack_30 = FUN_140ec98e0(param_1,auStack_d0,lVar4 + 8,auStack_d8);
      uStack_28 = uStack_30;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_160,uStack_30);
      __1QString__QEAA_XZ(auStack_d0);
      __1QString__QEAA_XZ(auStack_d8);
      uStack_20 = __0QVariant__QEAA_XZ(auStack_c8);
      uStack_18 = uStack_20;
      _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
                (*(undefined8 *)(param_1[0x28] + 0x838),iStack_184,auStack_160,uStack_20);
      __1QVariant__QEAA_XZ(auStack_c8);
      if (*(int *)(lVar4 + 8) == 0) {
        iStack_168 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x838));
        iStack_168 = iStack_168 + -1;
      }
    }
    iStack_184 = iStack_184 + 1;
  }
  iVar2 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x838));
  if (iVar2 < 1) {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x820),0);
  }
  else {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x820),1);
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x838),iStack_168);
    *(undefined4 *)((longlong)param_1 + 0x11c) = 0;
  }
  FUN_140ebe800(param_1,0);
  __1QString__QEAA_XZ(auStack_160);
  return;
}

