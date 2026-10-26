
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ec1350(longlong *param_1)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined8 uStack_b0;
  longlong lStack_a8;
  int iStack_a0;
  undefined1 auStack_98 [8];
  undefined1 auStack_90 [8];
  undefined8 uStack_88;
  undefined8 uStack_80;
  int iStack_78;
  double dStack_70;
  undefined1 auStack_68 [8];
  undefined1 uStack_60;
  undefined1 auStack_5f [3];
  undefined4 uStack_5c;
  undefined4 auStack_58 [2];
  undefined1 auStack_50 [8];
  undefined1 uStack_48;
  undefined1 auStack_47 [7];
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  double dStack_18;
  double dStack_10;
  
  uStack_40 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_98);
  uStack_b4 = _currentIndex_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x9b8));
  FUN_140eceb60(param_1,uStack_b4);
  iStack_a0 = 0;
  while( true ) {
    iVar2 = _count_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x9b8));
    if (iVar2 <= iStack_a0) break;
    uStack_38 = _itemText_QComboBox__QEBA_AVQString__H_Z
                          (*(undefined8 *)(param_1[0x28] + 0x9b8),auStack_68,iStack_a0);
    uStack_30 = uStack_38;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_98,uStack_38);
    __1QString__QEAA_XZ(auStack_68);
    _toLocal8Bit_QString__QEBA_AVQByteArray__XZ(auStack_98,auStack_90);
    uStack_b0 = _data_QByteArray__QEAAPEADXZ(auStack_90);
    FUN_14146dfd4(uStack_b0,&UNK_142b14c3c,&uStack_b8);
    uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
    lStack_a8 = FUN_140c7ae80(uVar3,uStack_b8);
    if ((lStack_a8 != 0) && (param_1[0x1f] != 0)) {
      func_0x00014085e050(lStack_a8 + 0x2d10,(longlong)iStack_a0 * 0x158 + param_1[0x1f]);
    }
    __1QByteArray__QEAA_XZ(auStack_90);
    iStack_a0 = iStack_a0 + 1;
  }
  cVar1 = FUN_140130ee0(param_1);
  if (cVar1 != '\0') {
    uStack_80 = 0;
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0xa40));
    if (cVar1 == '\0') {
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0xa48));
      if (cVar1 != '\0') {
        uStack_80 = *(undefined8 *)(param_1[0x28] + 0xa48);
        iStack_78 = 1;
      }
    }
    else {
      uStack_80 = *(undefined8 *)(param_1[0x28] + 0xa40);
      iStack_78 = 0;
    }
    uStack_88 = func_0x0001402c7ed0();
    if (iStack_78 == 0) {
      uStack_60 = 0;
      func_0x0001400ab150(uStack_88,&uStack_60);
    }
    else {
      auStack_5f[0] = 1;
      func_0x0001400ab150(uStack_88,auStack_5f);
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0xa00));
      if (cVar1 == '\0') {
        cVar1 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0xa08));
        if (cVar1 != '\0') {
          auStack_58[0] = 3;
          func_0x00014092db70(uStack_88,auStack_58);
        }
      }
      else {
        uStack_5c = 1;
        func_0x00014092db70(uStack_88,&uStack_5c);
      }
      uStack_28 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(param_1[0x28] + 0xa10),auStack_50);
      uStack_20 = uStack_28;
      dStack_70 = (double)_toDouble_QString__QEBANPEA_N_Z(uStack_28,0);
      __1QString__QEAA_XZ(auStack_50);
      if (_DAT_1421249e0 <= dStack_70) {
        if (dStack_70 <= _DAT_142142078) {
          dStack_10 = dStack_70;
        }
        else {
          dStack_10 = _DAT_142142078;
        }
        dStack_18 = dStack_10;
      }
      else {
        dStack_18 = _DAT_1421249e0;
      }
      dStack_70 = dStack_18;
      func_0x000140dbba90(uStack_88,&dStack_70);
      cVar1 = _isChecked_QAbstractButton__QEBA_NXZ(*(undefined8 *)(param_1[0x28] + 0xa28));
      if (cVar1 == '\0') {
        auStack_47[0] = 1;
        func_0x000140e7b850(uStack_88,auStack_47);
      }
      else {
        uStack_48 = 0;
        func_0x000140e7b850(uStack_88,&uStack_48);
      }
    }
  }
  __1QString__QEAA_XZ(auStack_98);
  return;
}

