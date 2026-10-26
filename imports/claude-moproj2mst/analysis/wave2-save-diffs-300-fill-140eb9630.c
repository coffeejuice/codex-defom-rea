
void FUN_140eb9630(longlong param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 auStack_1c8 [8];
  undefined8 uStack_1c0;
  undefined1 auStack_1b8 [16];
  undefined1 auStack_1a8 [8];
  undefined1 auStack_1a0 [16];
  undefined1 auStack_190 [8];
  undefined1 auStack_188 [16];
  undefined1 auStack_178 [8];
  undefined1 auStack_170 [16];
  undefined1 auStack_160 [8];
  undefined1 auStack_158 [16];
  undefined1 auStack_148 [8];
  undefined1 auStack_140 [16];
  undefined1 auStack_130 [8];
  undefined1 auStack_128 [8];
  undefined1 auStack_120 [8];
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
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
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  int iStack_30;
  int iStack_2c;
  undefined1 uStack_28;
  undefined1 uStack_27;
  undefined1 uStack_26;
  int iStack_24;
  undefined1 uStack_20;
  undefined1 uStack_1f;
  undefined1 uStack_1e;
  undefined1 uStack_1d;
  undefined1 uStack_1c;
  undefined1 uStack_1b;
  undefined1 uStack_1a;
  undefined1 uStack_19;
  undefined1 uStack_18;
  
  uStack_118 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_1c8);
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 == '\0') {
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x168));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf8));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe0));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x100));
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x108));
    _clear_QComboBox__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8));
    uStack_b0 = __0QVariant__QEAA_XZ(auStack_170);
    uStack_a8 = uStack_b0;
    uStack_a0 = FUN_140f34710(auStack_160,&UNK_142b14628,0);
    uStack_98 = uStack_a0;
    _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8),0,uStack_a0,uStack_a8);
    __1QString__QEAA_XZ(auStack_160);
    __1QVariant__QEAA_XZ(auStack_170);
    uStack_90 = __0QVariant__QEAA_XZ(auStack_158);
    uStack_88 = uStack_90;
    uStack_80 = FUN_140f34710(auStack_148,&UNK_142b14638,0);
    uStack_78 = uStack_80;
    _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8),1,uStack_80,uStack_88);
    __1QString__QEAA_XZ(auStack_148);
    __1QVariant__QEAA_XZ(auStack_158);
    uStack_70 = __0QVariant__QEAA_XZ(auStack_140);
    uStack_68 = uStack_70;
    uStack_60 = FUN_140f34710(auStack_130,&UNK_142b14648,0);
    uStack_58 = uStack_60;
    _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8),2,uStack_60,uStack_68);
    __1QString__QEAA_XZ(auStack_130);
    __1QVariant__QEAA_XZ(auStack_140);
  }
  else {
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x168));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf8));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe0));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x100));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x108));
    _clear_QComboBox__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8));
    uStack_110 = __0QVariant__QEAA_XZ(auStack_1b8);
    uStack_108 = uStack_110;
    uStack_100 = FUN_140f34710(auStack_1a8,&UNK_142b145f8,0);
    uStack_f8 = uStack_100;
    _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8),0,uStack_100,uStack_108);
    __1QString__QEAA_XZ(auStack_1a8);
    __1QVariant__QEAA_XZ(auStack_1b8);
    uStack_f0 = __0QVariant__QEAA_XZ(auStack_1a0);
    uStack_e8 = uStack_f0;
    uStack_e0 = FUN_140f34710(auStack_190,&UNK_142b14608,0);
    uStack_d8 = uStack_e0;
    _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8),1,uStack_e0,uStack_e8);
    __1QString__QEAA_XZ(auStack_190);
    __1QVariant__QEAA_XZ(auStack_1a0);
    uStack_d0 = __0QVariant__QEAA_XZ(auStack_188);
    uStack_c8 = uStack_d0;
    uStack_c0 = FUN_140f34710(auStack_178,&UNK_142b14618,0);
    uStack_b8 = uStack_c0;
    _insertItem_QComboBox__QEAAXHAEBVQString__AEBVQVariant___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8),2,uStack_c0,uStack_c8);
    __1QString__QEAA_XZ(auStack_178);
    __1QVariant__QEAA_XZ(auStack_188);
  }
  _blockSignals_QObject__QEAA_N_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x128),1);
  uStack_50 = _fromLocal8Bit_QString__SA_AV1_PEBDH_Z(auStack_128,param_2,0xffffffff);
  uStack_48 = uStack_50;
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x128),uStack_50);
  __1QString__QEAA_XZ(auStack_128);
  _blockSignals_QObject__QEAA_N_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x128),0);
  _blockSignals_QObject__QEAA_N_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x138),1);
  uStack_40 = _fromLocal8Bit_QString__SA_AV1_PEBDH_Z(auStack_120,param_2 + 0x51,0xffffffff);
  uStack_38 = uStack_40;
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x138),uStack_40);
  __1QString__QEAA_XZ(auStack_120);
  _blockSignals_QObject__QEAA_N_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x138),0);
  _setValue_QSpinBox__QEAAXH_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x148),*(undefined4 *)(param_2 + 0xe8)
            );
  iVar2 = func_0x0001402cb7a0(0x143d766b8);
  if (iVar2 == 5) {
    uStack_1c0 = FUN_140130af0(param_1);
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x150));
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x158));
  }
  else {
    _setValue_QSpinBox__QEAAXH_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x158),
               *(undefined4 *)(param_2 + 0xf0));
  }
  iStack_30 = *(int *)(param_2 + 0xfc);
  if (iStack_30 == 1) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x58),1)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x50),0)
    ;
  }
  else if (iStack_30 == 2) {
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x58),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x50),1)
    ;
  }
  else {
    FUN_141472790(&UNK_142b14660);
  }
  iVar2 = func_0x0001402cb7a0(0x143d766b8);
  if (iVar2 == 1) {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x148),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x40),1);
  }
  else {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x148),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x40),0);
  }
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 != '\0') {
    iStack_2c = *(int *)(param_2 + 0x100);
    if (iStack_2c == 1) {
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x178),1);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x188),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x180),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 400),0);
    }
    else if (iStack_2c == 2) {
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x178),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x188),1);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x180),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 400),0);
    }
    else if (iStack_2c == 3) {
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x178),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x188),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x180),1);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 400),0);
    }
    else if (iStack_2c == 4) {
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x178),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x188),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x180),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 400),1);
    }
    else {
      FUN_141472790(&UNK_142b146b0);
    }
  }
  uVar3 = FUN_140130e50(param_1);
  iVar2 = FUN_1412fae10(uVar3);
  if (iVar2 == 0) {
    uVar3 = FUN_140130e50(param_1);
    iVar2 = FUN_1412fab10(uVar3);
    if (iVar2 == 0) {
      uStack_28 = 0;
      goto code_r0x000140eba13a;
    }
  }
  uStack_28 = 1;
code_r0x000140eba13a:
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x90),uStack_28);
  uVar3 = FUN_140130e50(param_1);
  iVar2 = FUN_1412fae90(uVar3);
  uStack_27 = iVar2 != 0;
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 200),uStack_27);
  uVar3 = FUN_140130e50(param_1);
  iVar2 = FUN_1412faeb0(uVar3);
  uStack_26 = iVar2 != 0;
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xa8),uStack_26);
  cVar1 = func_0x000140ecf360(param_1);
  if (cVar1 == '\x01') {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x40),0);
    cVar1 = FUN_140130ea0(param_1);
    if (cVar1 != '\0') {
      _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x168),0);
    }
  }
  iStack_24 = *(int *)(param_2 + 0xf8) + -1;
  switch(iStack_24) {
  case 0:
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe8),1)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x100),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x108),0);
    break;
  default:
    FUN_141472790(&UNK_142b14710);
    break;
  case 2:
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf0),1)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x100),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x108),0);
    break;
  case 3:
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf8),1)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x100),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x108),0);
    break;
  case 4:
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe0),1)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x100),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x108),0);
    break;
  case 5:
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x100),1);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x108),0);
    break;
  case 6:
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xf8),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe0),0)
    ;
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x100),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x108),1);
  }
  iVar2 = func_0x0001408dfd60(param_2);
  uStack_20 = iVar2 != 0;
  _setChecked_QAbstractButton__QEAAX_N_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x78),uStack_20);
  iVar2 = func_0x0001408dfda0(param_2);
  uStack_1f = iVar2 != 0;
  _setChecked_QAbstractButton__QEAAX_N_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x90),uStack_1f);
  iVar2 = func_0x0001408dfd80(param_2);
  uStack_1e = iVar2 != 0;
  _setChecked_QAbstractButton__QEAAX_N_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x70),uStack_1e);
  iVar2 = func_0x0001408dfdc0(param_2);
  uStack_1d = iVar2 != 0;
  _setChecked_QAbstractButton__QEAAX_N_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 200),uStack_1d);
  iVar2 = func_0x0001408dfde0(param_2);
  uStack_1c = iVar2 != 0;
  _setChecked_QAbstractButton__QEAAX_N_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xa0),uStack_1c);
  iVar2 = func_0x0001408dfe00(param_2);
  uStack_1b = iVar2 != 0;
  _setChecked_QAbstractButton__QEAAX_N_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xa8),uStack_1b);
  iVar2 = func_0x0001408dfda0(param_2);
  if (iVar2 == 2) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x10d8),1);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x10d8),0);
  }
  iVar2 = func_0x0001408dfe00(param_2);
  uStack_1a = iVar2 != 0;
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xb8),uStack_1a);
  uVar3 = FUN_140130e50(param_1);
  iVar2 = FUN_1412faeb0(uVar3);
  uStack_19 = iVar2 != 0;
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xa0),uStack_19);
  uVar3 = FUN_140130e50(param_1);
  iVar2 = FUN_1412faeb0(uVar3);
  uStack_18 = iVar2 != 0;
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 200),uStack_18);
  FUN_1408e00d0(param_2);
  __1QString__QEAA_XZ(auStack_1c8);
  return;
}

