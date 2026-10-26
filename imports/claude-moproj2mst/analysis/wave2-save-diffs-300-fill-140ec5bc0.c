
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ec5bc0(longlong *param_1,longlong param_2)

{
  undefined8 *puVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  undefined8 uVar5;
  longlong lVar6;
  int iStack_1c8;
  int iStack_1c4;
  int iStack_1a4;
  undefined1 auStack_1a0 [8];
  undefined4 uStack_198;
  uint uStack_194;
  undefined1 auStack_190 [8];
  undefined1 auStack_188 [8];
  undefined1 auStack_180 [16];
  undefined1 auStack_170 [8];
  undefined4 auStack_168 [2];
  undefined1 auStack_160 [8];
  undefined1 auStack_158 [8];
  undefined1 auStack_150 [8];
  undefined1 auStack_148 [16];
  undefined8 *puStack_138;
  undefined8 *puStack_130;
  undefined1 auStack_128 [8];
  undefined1 auStack_120 [8];
  undefined1 auStack_118 [8];
  undefined1 auStack_110 [8];
  undefined1 auStack_108 [8];
  undefined1 auStack_100 [8];
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  int iStack_e0;
  undefined1 uStack_dc;
  undefined1 uStack_db;
  int iStack_d8;
  int iStack_d4;
  int iStack_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 *puStack_78;
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
  
  uStack_f8 = 0xfffffffffffffffe;
  uStack_198 = 0;
  __0QString__QEAA_XZ(auStack_1a0);
  iStack_1c8 = 0;
  uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                    (*(undefined8 *)(param_1[0x28] + 0x1008),*(undefined8 *)(param_1[0x28] + 0x11c8)
                    );
  _removeTab_QTabWidget__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1008),uVar3);
  uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                    (*(undefined8 *)(param_1[0x28] + 0x1008),*(undefined8 *)(param_1[0x28] + 0x11c8)
                    );
  _setTabEnabled_QTabWidget__QEAAXH_N_Z(*(undefined8 *)(param_1[0x28] + 0x1008),uVar3,0);
  cVar2 = FUN_140130ea0(param_1);
  if (cVar2 == '\0') {
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x1168));
  }
  else {
    _show_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x1168));
    uVar3 = _indexOf_QTabWidget__QEBAHPEAVQWidget___Z
                      (*(undefined8 *)(param_1[0x28] + 0x1008),
                       *(undefined8 *)(param_1[0x28] + 0x11c8));
    cVar2 = _isTabEnabled_QTabWidget__QEBA_NH_Z(*(undefined8 *)(param_1[0x28] + 0x1008),uVar3);
    if (cVar2 == '\0') {
      uStack_f0 = FUN_140f34710(auStack_190,&UNK_142b14cc8,0);
      uStack_e8 = uStack_f0;
      _addTab_QTabWidget__QEAAHPEAVQWidget__AEBVQString___Z
                (*(undefined8 *)(param_1[0x28] + 0x1008),*(undefined8 *)(param_1[0x28] + 0x11c8),
                 uStack_f0);
      __1QString__QEAA_XZ(auStack_190);
    }
  }
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_1a0,&UNK_142b14cdc,*(undefined8 *)(param_2 + 0x640));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1070),auStack_1a0);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_1a0,&UNK_142b14ce4,*(undefined8 *)(param_2 + 0x648));
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1068),auStack_1a0);
  iStack_e0 = *(int *)(param_2 + 0x650);
  if (iStack_e0 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1030),1);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1038),0);
  }
  else if (iStack_e0 == 1) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1030),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1038),1);
  }
  else {
    FUN_141472790(&UNK_142b14cf0);
  }
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1030),1);
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1038),1);
  _sprintf_QString__QEAAAEAV1_PEBDZZ
            (auStack_1a0,&UNK_142b14d38,*(double *)(param_2 + 0x660) * _UNK_1421de200);
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1110),auStack_1a0);
  _sprintf_QString__QEAAAEAV1_PEBDZZ
            (auStack_1a0,&UNK_142b14d40,*(double *)(param_2 + 0x668) * _UNK_1421de200);
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1128),auStack_1a0);
  _sprintf_QString__QEAAAEAV1_PEBDZZ
            (auStack_1a0,&UNK_142b14d48,*(double *)(param_2 + 0x670) * _UNK_1421de200);
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1140),auStack_1a0);
  _sprintf_QString__QEAAAEAV1_PEBDZZ
            (auStack_1a0,&UNK_142b14d50,*(double *)(param_2 + 0x678) * _UNK_1421de200);
  _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1158),auStack_1a0);
  for (iStack_1c4 = 0; iStack_1c4 < 10; iStack_1c4 = iStack_1c4 + 1) {
    uVar5 = _item_QTableWidget__QEBAPEAVQTableWidgetItem__HH_Z
                      (*(undefined8 *)(param_1[0x28] + 0x11c0),iStack_1c4,0);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_1a0,&UNK_142b14d58);
    _setText_QTableWidgetItem__QEAAXAEBVQString___Z(uVar5);
    _update_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x11b0));
  }
  cVar2 = FUN_140130ea0(param_1);
  if (cVar2 != '\0') {
    if (param_2 != 0) {
      iStack_1c8 = *(int *)(param_2 + 0x9c8);
    }
    if (iStack_1c8 == 0) {
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x11a0),1);
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1180),0);
      _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1190),0);
      __0QString__QEAA_PEBD_Z(auStack_188,&UNK_142b0ffd6);
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1190),auStack_188);
      __1QString__QEAA_XZ(auStack_188);
    }
    else {
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x11a0),0);
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1180),1);
      _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1190),1);
      _sprintf_QString__QEAAAEAV1_PEBDZZ
                (auStack_1a0,&UNK_142b14d5c,*(undefined8 *)(param_2 + 0x9d0));
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1190),auStack_1a0);
    }
  }
  for (iStack_1c4 = 0; iStack_1c4 < 7; iStack_1c4 = iStack_1c4 + 1) {
    uStack_194 = (uint)bRam00000001446ee587;
    if ((iStack_1c4 == 3) || (iStack_1c4 == 6)) {
      iVar4 = FUN_1412fae10(uStack_194);
      uStack_dc = iVar4 != 0;
      _setEnabled_QWidget__QEAAX_N_Z(param_1[(longlong)iStack_1c4 + 0x10]);
    }
    if (iStack_1c4 != 4) {
      uStack_db = *(int *)(param_2 + 0x11c + (longlong)iStack_1c4 * 4) != 0;
      _setChecked_QAbstractButton__QEAAX_N_Z(param_1[(longlong)iStack_1c4 + 0x10]);
    }
  }
  iStack_d8 = *(int *)(param_2 + 0x138);
  if (iStack_d8 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x12d8),1);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x12e0),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x12e8),0);
  }
  else if (iStack_d8 == 2) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x12d8),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x12e0),1);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x12e8),0);
  }
  else if (iStack_d8 == 3) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x12d8),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x12e0),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x12e8),1);
  }
  else {
    FUN_141472790(&UNK_142b14d70);
  }
  iStack_d4 = *(int *)(param_2 + 0x13c);
  if (iStack_d4 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1300),1);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1308),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1310),0);
  }
  else if (iStack_d4 == 2) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1300),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1308),1);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1310),0);
  }
  else if (iStack_d4 == 3) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1300),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1308),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1310),1);
  }
  else {
    FUN_141472790(&UNK_142b14dc0);
  }
  iStack_d0 = *(int *)(param_2 + 0x140);
  if (iStack_d0 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1328),1);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1330),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1338),0);
  }
  else if (iStack_d0 == 2) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1328),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1330),1);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1338),0);
  }
  else if (iStack_d0 == 3) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1328),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1330),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1338),1);
  }
  else {
    FUN_141472790(&UNK_142b14e10);
  }
  _clear_QComboBox__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x10c0));
  auStack_168[0] = 0;
  uStack_c8 = __0QVariant__QEAA_XZ(auStack_180);
  uStack_c0 = uStack_c8;
  uStack_b8 = FUN_140f34710(auStack_170,&UNK_142b14e58,0);
  uStack_b0 = uStack_b8;
  uStack_a8 = FUN_140ec98e0(param_1,auStack_160,auStack_168,uStack_b8);
  uStack_a0 = uStack_a8;
  _addItem_QComboBox__QEAAXAEBVQString__AEBVQVariant___Z
            (*(undefined8 *)(param_1[0x28] + 0x10c0),uStack_a8,uStack_c0);
  __1QString__QEAA_XZ(auStack_160);
  __1QString__QEAA_XZ(auStack_170);
  __1QVariant__QEAA_XZ(auStack_180);
  iStack_1a4 = 0;
  iStack_1c4 = 0;
  while( true ) {
    uVar5 = (**(code **)(*param_1 + 0x198))(param_1);
    iVar4 = FUN_140c7c1a0(uVar5);
    if (iVar4 <= iStack_1c4) break;
    uVar5 = (**(code **)(*param_1 + 0x198))(param_1);
    lVar6 = FUN_140c7aed0(uVar5,iStack_1c4 + 1);
    if (((lVar6 != 0) && (cVar2 = FUN_140834fa0(lVar6), cVar2 != '\0')) &&
       (cVar2 = FUN_140ed97c0(param_1,lVar6), cVar2 != '\0')) {
      __0QString__QEAA_PEBD_Z(auStack_158,lVar6 + 0x828);
      uStack_98 = FUN_140ec98e0(param_1,auStack_150,lVar6 + 8,auStack_158);
      uStack_90 = uStack_98;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_1a0,uStack_98);
      __1QString__QEAA_XZ(auStack_150);
      __1QString__QEAA_XZ(auStack_158);
      uStack_88 = __0QVariant__QEAA_XZ(auStack_148);
      uStack_80 = uStack_88;
      _addItem_QComboBox__QEAAXAEBVQString__AEBVQVariant___Z
                (*(undefined8 *)(param_1[0x28] + 0x10c0),auStack_1a0,uStack_88);
      __1QVariant__QEAA_XZ(auStack_148);
      if (*(int *)(lVar6 + 8) == *(int *)(param_2 + 0x9b4)) {
        iStack_1a4 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x10c0));
        iStack_1a4 = iStack_1a4 + -1;
      }
    }
    iStack_1c4 = iStack_1c4 + 1;
  }
  _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x10c0),iStack_1a4);
  cVar2 = FUN_140130ea0(param_1);
  if (cVar2 != '\0') {
    _setValue_QSpinBox__QEAAXH_Z
              (*(undefined8 *)(param_1[0x28] + 0x1200),*(undefined4 *)(param_2 + 0x9b8));
    _setValue_QSpinBox__QEAAXH_Z
              (*(undefined8 *)(param_1[0x28] + 0x11f8),*(undefined4 *)(param_2 + 0x9bc));
    if (*(int *)(param_2 + 0x9c0) == 0) {
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1228),0);
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1220),1);
    }
    else {
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1228),1);
      _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1220),0);
    }
    _setValue_QSpinBox__QEAAXH_Z
              (*(undefined8 *)(param_1[0x28] + 0x1238),*(undefined4 *)(param_2 + 0x9c4));
  }
  puStack_130 = (undefined8 *)FUN_14146eabc(0x18);
  if (puStack_130 == (undefined8 *)0x0) {
    puStack_78 = (undefined8 *)0x0;
  }
  else {
    __0QIntValidator__QEAA_PEAVQObject___Z(puStack_130,param_1);
    *puStack_130 = &UNK_142380968;
    puStack_78 = puStack_130;
  }
  puVar1 = puStack_78;
  puStack_138 = puStack_78;
  _setBottom_QIntValidator__QEAAXH_Z(puStack_78,1);
  _setValidator_QLineEdit__QEAAXPEBVQValidator___Z(*(undefined8 *)(param_1[0x28] + 0x1370),puVar1);
  _setValidator_QLineEdit__QEAAXPEBVQValidator___Z(*(undefined8 *)(param_1[0x28] + 0x1398),puVar1);
  _setValidator_QLineEdit__QEAAXPEBVQValidator___Z(*(undefined8 *)(param_1[0x28] + 0x13c0),puVar1);
  _setValidator_QLineEdit__QEAAXPEBVQValidator___Z(*(undefined8 *)(param_1[0x28] + 0x13f0),puVar1);
  _setValidator_QLineEdit__QEAAXPEBVQValidator___Z(*(undefined8 *)(param_1[0x28] + 0x1418),puVar1);
  _setValidator_QLineEdit__QEAAXPEBVQValidator___Z(*(undefined8 *)(param_1[0x28] + 0x1440),puVar1);
  if (((*(int *)(param_2 + 0x328) < 6) || (*(int *)(param_2 + 0x32c) < 1)) ||
     (*(int *)(param_2 + 0x370) < 6)) {
    *(undefined4 *)(param_2 + 0x328) = 6;
    *(undefined4 *)(param_2 + 0x32c) = 1;
    *(undefined4 *)(param_2 + 0x370) = 6;
    FUN_14089b840(param_2 + 0x330);
  }
  if ((int)**(double **)(param_2 + 0x378) == 0) {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1360),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1370),0);
  }
  else if ((int)**(double **)(param_2 + 0x378) == -1) {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1360),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1370),0);
  }
  else {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1360),2);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1370),1);
    uStack_70 = _number_QString__SA_AV1_HH_Z(auStack_128,(int)**(double **)(param_2 + 0x378),10);
    uStack_68 = uStack_70;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1370),uStack_70);
    __1QString__QEAA_XZ(auStack_128);
  }
  iVar4 = *(int *)(param_2 + 0x32c);
  if ((int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8) == 0) {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 5000),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1398),0);
  }
  else {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 5000),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1398),1);
    uStack_60 = _number_QString__SA_AV1_HH_Z
                          (auStack_120,
                           (int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8),10
                          );
    uStack_58 = uStack_60;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1398),uStack_60);
    __1QString__QEAA_XZ(auStack_120);
  }
  iVar4 = *(int *)(param_2 + 0x32c) << 1;
  if ((int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8) == 0) {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x13b0),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x13c0),0);
  }
  else {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x13b0),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x13c0),1);
    uStack_50 = _number_QString__SA_AV1_HH_Z
                          (auStack_118,
                           (int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8),10
                          );
    uStack_48 = uStack_50;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x13c0),uStack_50);
    __1QString__QEAA_XZ(auStack_118);
  }
  iVar4 = *(int *)(param_2 + 0x32c) * 3;
  if ((int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8) == 0) {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x13e0),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x13f0),0);
  }
  else {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x13e0),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x13f0),1);
    uStack_40 = _number_QString__SA_AV1_HH_Z
                          (auStack_110,
                           (int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8),10
                          );
    uStack_38 = uStack_40;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x13f0),uStack_40);
    __1QString__QEAA_XZ(auStack_110);
  }
  iVar4 = *(int *)(param_2 + 0x32c) << 2;
  if ((int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8) == 0) {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1408),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1418),0);
  }
  else {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1408),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1418),1);
    uStack_30 = _number_QString__SA_AV1_HH_Z
                          (auStack_108,
                           (int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8),10
                          );
    uStack_28 = uStack_30;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1418),uStack_30);
    __1QString__QEAA_XZ(auStack_108);
  }
  iVar4 = *(int *)(param_2 + 0x32c) * 5;
  if ((int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8) == 0) {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1430),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1440),0);
  }
  else {
    _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1430),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(param_1[0x28] + 0x1440),1);
    uStack_20 = _number_QString__SA_AV1_HH_Z
                          (auStack_100,
                           (int)*(double *)(*(longlong *)(param_2 + 0x378) + (longlong)iVar4 * 8),10
                          );
    uStack_18 = uStack_20;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x1440),uStack_20);
    __1QString__QEAA_XZ(auStack_100);
  }
  __1QString__QEAA_XZ(auStack_1a0);
  return;
}

